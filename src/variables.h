/** @file: variables.h
 *
 * Define routines to manage system variables
 */

#ifndef __VARIABLES_H

/**
 * Initialize the system variables
 */
extern void var_init();

/**
 * Set the value of a variable
 *
 * @param name the name of the variable to set
 * @param value the value the variable should have
 * @return 0 on success, negative number on error
 */
extern short var_set(const char *name, const char *value);

/**
 * Get the value of a variable
 *
 * @param name the name of the variable to set
 * @return pointer to the string on success, 0 if not found
 */
extern const char * var_get(const char *name);

#endif
