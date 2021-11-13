/*
 * Mechanism for handling the SET command
 *
 * SET <setting name> <setting value>
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "errors.h"
#include "settings.h"
#include "simpleio.h"
#include "syscalls.h"
#include "interrupt.h"
#include "rtc_reg.h"
#include "dev/rtc.h"
#include "dev/text_screen_iii.h"
#include "snd/codec.h"
#include "vicky_general.h"

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
        strncpy(setting->help, help);
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
        /* Setting not found... */
        return ERR_NOT_FOUND;

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
        /* Setting not found... */
        return ERR_NOT_FOUND;

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

    return 0;
}

/*
 * Command to set the value of a setting
 */
short cli_cmd_set(short channel, int argc, char * argv[]) {
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

    } else if ((argc == 2) && ((strcmp(argv[1], "HELP") == 0) || (strcmp(argv[1], "help") == 0) || (strcmp(argv[1], "?") == 0)) {
        cli_set_help(channel);

    } else {
        print(channel, "USAGE: SET <name> <value>\n");
        return -1;
    }
}

/*
 * Command to set the value of a setting
 */
short cli_cmd_get(short channel, int argc, char * argv[]) {
    char buffer[128];
    short result;

    if (argc == 2) {

        if ((strcmp(argv[1], "HELP") == 0) || (strcmp(argv[1], "help") == 0) || (strcmp(argv[1], "?") == 0)) {
            cli_set_help(channel);

        } else {
            result = cli_get_value(channel, argv[1], buffer, 128);
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
                return -1;
            }
        } else {
            if ((date[i] < '0') || (date[i] > '9')) {
                sys_chan_write(channel, usage, strlen(usage));
                return -1;
            }
        }
    }

    date_time.year = atoi_n(&date[0], 4);
    date_time.month = atoi_n(&date[5], 2);
    date_time.day = atoi_n(&date[8], 2);

    rtc_set_time(&date_time);

    return 0;
}

/*
 * DATE getter
 */
short cli_date_get(short channel, char * value) {
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
                return -1;
            }
        } else {
            if ((time[i] < '0') || (time[i] > '9')) {
                sys_chan_write(channel, usage, strlen(usage));
                return -1;
            }
        }
    }

    date_time.hour = atoi_n(&time[0], 2);
    date_time.minute = atoi_n(&time[3], 2);
    date_time.second = atoi_n(&time[6], 2);

    rtc_set_time(&date_time);

    return 0;
}

/*
 * TIME getter
 */
short cli_time_get(short channel, char * value) {
    t_time time;

    rtc_get_time(&time);
    sprintf(value, "%02d:%02d:%02d\n", time.hour, time.minute, time.second);

    return 0;
}

/*
 * Font setter -- SET FONT <path>
 */
short cli_font_set(short screen, const char * value) {
    /* Open the file */
    short chan = fsys_open(value, 0x01);
    if (chan > 0) {
        int i;
        for (i = 0; i < 8 * 256; i++) {
            short b = sys_chan_read_b(chan);
            if (b >= 0) {
                VICKY_TXT_FONT_A[i] = (unsigned char)b;
            } else {
                char message[80];

                /* Reset the text screen */
                text_init();
                fsys_close(chan);

                sprintf(message, "Unable to read font file: %d\n", b);
                sys_chan_write(screen, message, strlen(message));
                return -1;
            }
        }

        fsys_close(chan);
    } else {
        print(screen, "Could not load font file.\n");
        return -1;
    }
}

/*
 * Font setter -- GET FONT <path>
 */
short cli_font_get(short channel, char * value) {
    /* We don't keep the font path */
    *value = 0;
    return 0;
}

/*
 * Volume setter -- SET VOLUME <value>
 */
short cli_volume_set(short channel, char * value) {
    unsigned char volume = (unsigned char)cli_eval_number(value);
    codec_set_volume(volume);
    return 0;
}

/*
 * Volume getter -- GET VOLUME
 */
short cli_volume_get(short channel, char * value) {
    sprintf(value, "%d", codec_get_volume());
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
short cli_layout_get(short channel, char * value) {
    return 0;
}

/*
 * Initialize the settings tables
 */
void cli_set_init() {
    cli_first_setting = 0;
    cli_last_setting = 0;

    cli_set_register("DATE", "DATE yyyy-mm-dd -- set the date in the realtime clock", cli_date_set, cli_date_get);
    // cli_set_register("RTC", "RTC 1|0 -- Enable or disable the realtime clock interrupt", cli_rtc_set, cli_rtc_get);
    // cli_set_register("SOF", "SOF 1|0 -- Enable or disable the Start of Frame interrupt", cli_sof_set, cli_sof_get);
    cli_set_register("FONT", "FONT <path> -- set a font for the display", cli_font_set, cli_font_get);
    cli_set_register("KEYBOARD", "KEYBOARD <path> -- set the keyboard layout", cli_layout_set, cli_layout_get);
    cli_set_register("TIME", "TIME HH:MM:SS -- set the time in the realtime clock", cli_time_set, cli_time_get);
    cli_set_register("VOLUME", "VOLUME <0 - 255> -- set the master volume", cli_volume_set, cli_volume_get);
}
