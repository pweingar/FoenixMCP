/*
 * Mechanism for handling the SET command
 *
 * SET <setting name> <setting value>
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "settings.h"
#include "simpleio.h"
#include "syscalls.h"
#include "interrupt.h"
#include "rtc_reg.h"

#define MAX_SETTING_NAME    64

/*
 * Structure to hold a setting
 */
typedef struct s_setting {
    char name[MAX_SETTING_NAME];        /* Name of the setting (in upper case) */
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
short cli_set_register(const char * name, cli_setter setter, cli_getter getter) {
    p_setting setting = (p_setting)malloc(sizeof(t_setting));
    if (setting == 0) {
        /* Could not allocate the setting record... return an error */
        return -1;

    } else {
        /* Set the fields for the setting */
        cli_name_upper(setting->name, name);
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
 * Command to set the value of a setting
 */
short cli_cmd_set(short channel, int argc, char * argv[]) {
    short result;

    if (argc == 3) {
        result = cli_set_value(channel, argv[1], argv[2]);
        if (result != 0) {
            print(channel, "Unknown setting name.\n");
            return 0;
        }
        return result;

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
        result = cli_get_value(channel, argv[1], buffer, 128);
        if (result == 0) {
            print(channel, buffer);
            print(channel, "\n");
            return 0;

        } else {
            print(channel, "Unable to get value.\n");
            return result;
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
        unsigned char flags = *RTC_FLAGS;
        *RTC_ENABLES = RTC_PIE;
        int_enable(INT_RTC);
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

/*
 * Initialize the settings tables
 */
void cli_set_init() {
    cli_first_setting = 0;
    cli_last_setting = 0;

    cli_set_register("SOF", cli_sof_set, cli_sof_get);      /* Register "SOF" setting */
    cli_set_register("RTC", cli_rtc_set, cli_rtc_get);      /* Register "RTC" setting */

}
