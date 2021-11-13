/*
 * Mechanism for handling the SET command
 *
 * SET <setting name> <setting value>
 */

#ifndef __SETTINGS_H
#define __SETTINGS_H

/* Pointer to a function to set a value */
typedef short (*cli_setter)(short, const char *);

/* Pointer to a function to get a value */
typedef short (*cli_getter)(short, char * buffer, short size);

/*
 * Initialize the settings tables
 */
extern void cli_set_init();

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
extern short cli_set_register(const char * name, cli_setter setter, cli_getter getter);

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
extern short cli_set_value(const char * name, const char * value);

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
extern short cli_get_value(const char * name, char * buffer, short size);

/*
 * Command to set the value of a setting
 */
extern short cli_cmd_set(short screen, int argc, char * argv[]);

/*
 * Command to set the value of a setting
 */
extern short cli_cmd_get(short screen, int argc, char * argv[]);

#endif
