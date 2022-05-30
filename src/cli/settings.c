/*
 * Mechanism for handling the SET command
 *
 * SET <setting name> <setting value>
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "log.h"
#include "errors.h"
#include "settings.h"
#include "simpleio.h"
#include "syscalls.h"
#include "interrupt.h"
#include "rtc_reg.h"
#include "dev/rtc.h"
#include "dev/txt_screen.h"
#include "snd/codec.h"
#include "utilities.h"
#include "vicky_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "dev/kbd_mo.h"
#endif

#define MAX_SETTING_NAME    64
#define MAX_SETTING_HELP    80

/*
 * Structure to hold a setting
 */
typedef struct s_setting {
    char name[MAX_SETTING_NAME];        /* Name of the setting (in upper case) */
    char help[MAX_SETTING_HELP];        /* Help line for this setting */
    cli_setter setter;                  /* The function to set the value of the setting */
    cli_getter getter;                  /* The function to get the value of the setting */
    struct s_setting * next;            /* Pointer to the next registered setting */
} t_setting, *p_setting;

static p_setting cli_first_setting;    /* Pointer to the first registered setting */
static p_setting cli_last_setting;     /* Pointer to the last registered setting */

/*
 * Copy the name to a buffer as an upper case setting name
 *
 * Inputs:
 * name_upper = pointer to the buffer of at least MAX_SETTING_NAME bytes to receive the copy
 * name = the name to copy
 */
void cli_name_upper(char * name_upper, const char *name) {
    short i;

    for (i = 0; (i < MAX_SETTING_NAME) && (name[i] != 0); i++) {
        name_upper[i] = toupper(name[i]);
    }
    name_upper[i] = 0;
}

/*
 * Add a setting
 *
 * Inputs:
 * name = the name of the setting
 * setter = the function to change the value of the setting
 * getter = the function to get the value of a setting
 *
 * Returns:
 * 0 on success, any other number is an error
 */
short cli_set_register(const char * name, const char * help, cli_setter setter, cli_getter getter) {
    p_setting setting = (p_setting)malloc(sizeof(t_setting));
    if (setting == 0) {
        /* Could not allocate the setting record... return an error */
        return -1;

    } else {
        /* Set the fields for the setting */
        cli_name_upper(setting->name, name);
        strncpy(setting->help, help, MAX_SETTING_HELP);
        setting->help[MAX_SETTING_HELP] = '\0';
        setting->setter = setter;
        setting->getter = getter;
        setting->next = 0;

        if (cli_first_setting == 0) {
            /* This is the first and last setting so far */
            cli_first_setting = setting;
            cli_last_setting = setting;

        } else {
            /* This is the new last setting */
            cli_last_setting->next = setting;
            cli_last_setting = setting;
        }

        /* Return success */
        return 0;
    }
}

/*
 * Find the setting that matches the name (case insensitive search)
 *
 * Inputs:
 * name = the setting name to find
 *
 * Returns:
 * pointer to the matching setting (0 if none found)
 */
p_setting cli_find_setting(const char * name) {
    char name_upper[MAX_SETTING_NAME];
    p_setting setting;

    /* Convert the name to upper case */
    cli_name_upper(name_upper, name);

    /* Walk through the list of settings to find a name that matches */
    for (setting = cli_first_setting; setting != 0; setting = setting->next) {
        if (strcmp(setting->name, name_upper) == 0) {
            /* Found the setting... */
            return setting;
        }
    }

    /* Didn't find the setting... */
    return 0;
}

/*
 * Set the value of a setting
 *
 * Inputs:
 * name = the name of the setting to update (case insensitive)
 * value = the value to store in the setting (may be interpreted by the setting)
 *
 * Returns:
 * 0 on success, any other number is an error
 */
short cli_set_value(short channel, const char * name, const char * value) {
    p_setting setting = cli_find_setting(name);
    if (setting == 0) {
        /* Setting not found: set a system variable */
        return sys_var_set(name, value);

    } else {
        /* Attempt to set the value, and return the results */
        return setting->setter(channel, value);
    }
}

/*
 * Get the value of a setting
 *
 * Inputs:
 * name = the name of the setting to update (case insensitive)
 * buffer = the place to copy a string representing the value of the setting
 * size = the number of characters that can be stored in the buffer
 *
 * Returns:
 * 0 on success, any other number is an error
 */
short cli_get_value(short channel, const char * name, char * buffer, short size) {
    p_setting setting = cli_find_setting(name);
    if (setting == 0) {
        /* Setting not found: try to get an environment variable */
        const char * value = sys_var_get(name);
        if (value) {
            strncpy(buffer, value, size);
            return 0;

        } else {
            return ERR_NOT_FOUND;
        }

    } else {
        /* Attempt to set the value, and return the results */
        return setting->getter(channel, buffer, size);
    }
}

/*
 * Print help information about the SET command
 */
void cli_set_help(short channel) {
    char message[80];
    p_setting setting;

    sprintf(message, "SET/GET command supported settings:\n");
    sys_chan_write(channel, message, strlen(message));

    for (setting = cli_first_setting; setting != 0; setting = setting->next) {
        sys_chan_write(channel, setting->help, strlen(setting->help));
        sys_chan_write(channel, "\n", 1);
    }
}

/*
 * Command to set the value of a setting
 */
short cli_cmd_set(short channel, int argc, const char * argv[]) {
    char message[80];
    short result;

    if (argc == 3) {
        result = cli_set_value(channel, argv[1], argv[2]);
        if (result != 0) {
            sprintf(message, "Unable to change setting: %s", sys_err_message(result));
            sys_chan_write(channel, message, strlen(message));
            return 0;
        }
        return result;

    } else if ((argc == 2) && (strcmp(argv[1], "HELP") == 0 || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "?") == 0)) {
        cli_set_help(channel);

    } else {
        print(channel, "USAGE: SET <name> <value>\n");
        return -1;
    }
}

/*
 * Command to set the value of a setting
 */
short cli_cmd_get(short channel, int argc, const char * argv[]) {
    char buffer[128];
    short result;

    if (argc == 2) {

        if ((strcmp(argv[1], "HELP") == 0) || (strcmp(argv[1], "help") == 0) || (strcmp(argv[1], "?") == 0)) {
            cli_set_help(channel);

        } else {
            result = cli_get_value(channel, (char*)argv[1], buffer, 128);
            if (result == 0) {
                print(channel, buffer);
                print(channel, "\n");
                return 0;

            } else {
                sprintf(buffer, "Unable to get setting: %s", sys_err_message(result));
                sys_chan_write(channel, buffer, strlen(buffer));
                return result;
            }
        }

    } else {
        print(channel, "USAGE: GET <name>\n");
        return -1;
    }
}

/*
 * SOF setter
 */
short cli_sof_set(short channel, const char * value) {
    char message[80];

    if (strcmp(value, "1") == 0) {
        int_enable(INT_SOF_A);
        sprintf(message, "Start Of Frame interrupt enabled.\n");
    } else if (strcmp(value, "0") == 0) {
        int_disable(INT_SOF_A);
        sprintf(message, "Start Of Frame interrupt disabled.\n");
    } else {
        sprintf(message, "USAGE: SET SOF 0|1\n");
    }

    sys_chan_write(channel, message, strlen(message));
    return 0;
}

/*
 * SOF getter
 */
short cli_sof_get(short channel, char * buffer, short size) {
    return 0;
}

/*
 * RTC setter
 */
short cli_rtc_set(short channel, const char * value) {
    char message[80];

    if (strcmp(value, "1") == 0) {
        rtc_enable_ticks();
        sprintf(message, "RTC interrupt enabled.\n");

    } else if (strcmp(value, "0") == 0) {
        int_disable(INT_RTC);
        sprintf(message, "RTC interrupt disabled.\n");

    } else {
        sprintf(message, "USAGE: SET RTC 0|1\n");
    }

    sys_chan_write(channel, message, strlen(message));
    return 0;
}

/*
 * RTC getter
 */
short cli_rtc_get(short channel, char * buffer, short size) {
    return 0;
}

short atoi_n(char * text, short n) {
    short result = 0;
    short i;

    for (i = 0; i < n; i++) {
        result = result * 10;
        result = result + (text[i] - '0');
    }

    return result;
}

/*
 * DATE setter
 */
short cli_date_set(short channel, const char * date) {
    t_time date_time;
    short i;
    const char * usage = "USAGE: SET DATE yyyy-mm-dd\n";

    rtc_get_time(&date_time);
    date_time.is_24hours = 1;
    date_time.is_pm = 0;

    /* Validate date is correct format */

    for (i = 0; i < 10; i++) {
        if ((i == 4) || (i == 7)) {
            if (date[i] != '-') {
                sys_chan_write(channel, usage, strlen(usage));
                return ERR_GENERAL;
            }
        } else {
            if ((date[i] < '0') || (date[i] > '9')) {
                sys_chan_write(channel, usage, strlen(usage));
                return ERR_GENERAL;
            }
        }
    }

    date_time.year = atoi_n(&((char*)date)[0], 4);
    date_time.month = atoi_n(&((char*)date)[5], 2);
    date_time.day = atoi_n(&((char*)date)[8], 2);

    rtc_set_time(&date_time);

    return 0;
}

/*
 * DATE getter
 */
short cli_date_get(short channel, char * value, short size) {
    t_time time;

    rtc_get_time(&time);
    sprintf(value, "%04d-%02d-%02d\n", time.year, time.month, time.day);

    return 0;
}

/*
 * TIME setter
 */
short cli_time_set(short channel, const char * time) {
    t_time date_time;
    short i;
    const char * usage = "USAGE: SET TIME HH:MM:SS\n";

    rtc_get_time(&date_time);
    date_time.is_24hours = 1;
    date_time.is_pm = 0;

    /* Validate time is correct format */

    for (i = 0; i < 8; i++) {
        if ((i == 2) || (i == 5)) {
            if (time[i] != ':') {
                sys_chan_write(channel, usage, strlen(usage));
                return ERR_GENERAL;
            }
        } else {
            if ((time[i] < '0') || (time[i] > '9')) {
                sys_chan_write(channel, usage, strlen(usage));
                return ERR_GENERAL;
            }
        }
    }

    date_time.hour = atoi_n(&((char*)time)[0], 2);
    date_time.minute = atoi_n(&((char*)time)[3], 2);
    date_time.second = atoi_n(&((char*)time)[6], 2);

    rtc_set_time(&date_time);

    return 0;
}

/*
 * TIME getter
 */
short cli_time_get(short channel, char * value, short size) {
    t_time time;

    rtc_get_time(&time);
    sprintf(value, "%02d:%02d:%02d\n", time.hour, time.minute, time.second);

    return 0;
}

short cli_set_font(short screen, const char * path) {
    const unsigned long load_address = 0x10000;
    unsigned long jump_address = 0;
    char message[80];
    t_file_info filinfo;

    short result = sys_fsys_stat(path, &filinfo);
    if (result == 0) {
        if (filinfo.size == 256 * 8) {
            // It's a simple 8x8 font
            result = sys_fsys_load(path, load_address, &jump_address);
            if (result == 0) {
                sys_txt_set_font(screen, 8, 8, (unsigned char *)load_address);
                return 0;
            } else {
                return result;
            }

        } else {
            // It's 8x16.. check to make sure this device can support that
            p_txt_capabilities txt_caps = sys_txt_get_capabilities(screen);
            if (txt_caps != 0) {
                short supports_8x16 = 0;
                int i;
                for (i = 0; i < txt_caps->font_size_count; i++) {
                    p_extent font_size = &(txt_caps->font_sizes[i]);
                    if ((font_size->width == 8) && (font_size->height == 16)) {
                        // 8x16 is supported... load the font
                        result = sys_fsys_load(path, load_address, &jump_address);
                        if (result == 0) {
                            sys_txt_set_font(screen, 8, 16, (unsigned char *)load_address);
                            return 0;
                        } else {
                            return result;
                        }
                    }
                }

                // 8x16 is not a supported font size
                return ERR_NOT_SUPPORTED;

            } else {
                return -1;
            }

        }
    } else {
        return result;
    }
}

/**
 * Font@0 setter -- SET FONT@0 <path>
 *
 * Sets the font for the main screen (channel B)
 */
short cli_font0_set(short screen, const char * value) {
    // Load the font into memory
    short result = cli_set_font(0, value);
    return result;
}

/**
 * Font@0 getter -- GET FONT@0
 */
short cli_font0_get(short screen, char * value, short size) {
    value[0] = 0;
    return 0;
}

/**
 * Font@1 setter -- SET FONT@1 <path>
 *
 * Sets the font for the secondary screen (channel A or EVID)
 */
short cli_font1_set(short screen, const char * value) {
    // Load the font into memory
    short result = cli_set_font(1, value);
    return result;
}

/**
 * Font@0 getter -- GET FONT@0
 */
short cli_font1_get(short screen, char * value, short size) {
    value[0] = 0;
    return 0;
}

/*
 * Volume setter -- SET VOLUME <value>
 */
short cli_volume_set(short channel, const char * value) {
    unsigned char volume = (unsigned char)cli_eval_number(value);
    codec_set_volume(volume);
    return 0;
}

/*
 * Volume getter -- GET VOLUME
 */
short cli_volume_get(short channel, char * value, short size) {
    sprintf(value, "%d", (short)codec_get_volume());
    return 0;
}

/*
 * Set the keyboard layout given a keyboard layout file -- SET LAYOUT <path>
 */
short cli_layout_set(short channel, const char * value) {
    short result, fd;
    char buffer[1024];      /* Temporary storage for the translation tables */

    /* Try to open the file */
    fd = sys_fsys_open(value, 1);
    if (fd >= 0) {
        /* Try to read the data */
        result = sys_chan_read(fd, buffer, 1024);

        if (result > 0) {
            /* If we got something, set the layout */
            result = sys_kbd_layout(buffer);
        }

        sys_fsys_close(fd);
        return result;
    } else {
        return fd;
    }
}

/*
 * Get the keyboard layout given a keyboard layout file -- GET LAYOUT
 */
short cli_layout_get(short channel, char * value, short size) {
    value[0] = 0;
    return 0;
}

#if MODEL == MODEL_FOENIX_A2560K

unsigned short kbd_mo_color = 0x0000;

short cli_keycolor_set(short channel, const char * value) {
    kbd_mo_color = (unsigned short)cli_eval_number(value);
    kbdmo_set_led_matrix_fill(kbd_mo_color);
    return 0;
}

short cli_keycolor_get(short channel, char * value, short size)  {
    sprintf(value, "%04x", kbd_mo_color);
    return 0;
}

#endif

/**
 * Set the number of the text screen to use for interactions
 */
short cli_screen_set(short channel, const char * value) {
    char message[80];
    t_sys_info info;
    short screen = (short)cli_eval_number(value);

    sys_get_info(&info);
    if (screen < info.screens) {
        cli_txt_screen_set(screen);
    } else {
        sprintf(message, "Screen #%d not present.\n", screen);
        print(channel, message);
    }
    return 0;
}

/**
 * Get the number of the text screen to use for interactions
 */
short cli_screen_get(short channel, char * value, short size) {
    sprintf(value, "%d", cli_txt_screen_get());
    return 0;
}

/**
 * Set the path to the command processor "CLI" for built-in command processor
 */
short cli_shell_set(short channel, const char * value) {
    if (strcicmp(value, "cli") == 0) {
        cli_command_set(0);
    } else {
        cli_command_set(value);
    }
    return 0;
}

/**
 * Get the path to the command processor "CLI" for built-in command processor
 */
short cli_shell_get(short channel, char * value, short size) {
    char tmp[255];

    // Get the path...
    cli_command_get(tmp);

    // If it's empty... return that CLI is the current setting
    if (strlen(tmp) == 0) {
        strncpy(value, "cli", size);
    } else {
        strncpy(value, tmp, size);
    }
    return 0;
}


/*
 * Initialize the settings tables
 */
void cli_set_init() {
    t_sys_info info;

    cli_first_setting = 0;
    cli_last_setting = 0;

    sys_get_info(&info);

    cli_set_register("DATE", "DATE yyyy-mm-dd -- set the date in the realtime clock", cli_date_set, cli_date_get);

    if (info.screens == 1) {
        cli_set_register("FONT", "FONT <path> -- set a font for the display", cli_font0_set, cli_font0_get);
    } else {
        cli_set_register("FONT@0", "FONT@0 <path> -- set a font for the display #0", cli_font0_set, cli_font0_get);
        cli_set_register("FONT@1", "FONT@1 <path> -- set a font for the display #1", cli_font1_set, cli_font1_get);
    }

    cli_set_register("KEYBOARD", "KEYBOARD <path> -- set the keyboard layout", cli_layout_set, cli_layout_get);

#if MODEL == MODEL_FOENIX_A2560K
    cli_set_register("KEYCOLOR", "KEYCOLOR 0x0RGB -- set the keyboard color", cli_keycolor_set, cli_keycolor_get);
#endif

    if (info.screens > 1) {
        cli_set_register("SCREEN", "SCREEN <0 - 1> -- set the channel number to use for interactions", cli_screen_set, cli_screen_get);
    }

    cli_set_register("SHELL", "SHELL <path> -- set the path for the command processor shell", cli_shell_set, cli_shell_get);
    cli_set_register("TIME", "TIME HH:MM:SS -- set the time in the realtime clock", cli_time_set, cli_time_get);
}
