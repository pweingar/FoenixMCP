/** @file: variables.c
 *
 * Define routines to manage system variables
 */

#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "log.h"
#include "utilities.h"
#include "variables.h"

/*
 * Types
 */

typedef struct s_var_entry {
    char * name;
    char * value;
    struct s_var_entry * next;
} t_var_entry, *p_var_entry;

/*
 * Variables
 */

/* Pointer to the first variable */
static p_var_entry var_first_entry = 0;

/**
 * Initialize the system variables
 */
void var_init() {
    var_first_entry = 0;
}

/**
 * Find the entry matching the name
 *
 * @param name the name to find
 * @return a pointer to the entry, or 0 if not found
 */
p_var_entry var_find(const char * name) {
    TRACE("var_find");
    p_var_entry entry = var_first_entry;

    while (entry) {
        if (strcicmp(name, entry->name) == 0) {
            // If found, return it...
            return entry;

        } else {
            // Otherwise, try the next entry
            entry = entry->next;
        }
    }

    return 0;
}

/**
 * Attempt to set the value of an entry
 *
 * NOTE: null value pointer will be translated to an empty string
 *
 * @param entry pointer to the entry to update
 * @param value pointer to the string to copy
 * @return 0 on success, negative number on error
 */
short var_set_value(p_var_entry entry, const char * value) {
    TRACE("var_set_value");

    if (entry->value) {
        // Return the old value, if there is one
        free(entry->value);
    }

    if (value != 0) {
        entry->value = malloc(strlen(value));
        if (entry->value == 0) {
            return ERR_OUT_OF_MEMORY;
        }
        strcpy(entry->value, value);

    } else {
        entry->value = malloc(1);
        if (entry->value == 0) {
            return ERR_OUT_OF_MEMORY;
        }
        entry->value[0] = 0;
    }

    return 0;
}

/**
 * Set the value of a variable
 *
 * @param name the name of the variable to set
 * @param value the value the variable should have
 * @return 0 on success, negative number on error
 */
short var_set(const char *name, const char *value) {
    TRACE("var_set");

    p_var_entry entry = var_find(name);
    short result = 0;

    if (entry) {
        // Try to set the value
        return var_set_value(entry, value);

    } else {
        // Not found... make a new entry
        entry = (p_var_entry)malloc(sizeof(t_var_entry));
        if (entry == 0) {
            return ERR_OUT_OF_MEMORY;
        }
        entry->value = 0;
        entry->name = 0;

        // Make a copy of the name
        entry->name = malloc(strlen(name));
        if (entry->name == 0) {
            return ERR_OUT_OF_MEMORY;
        }
        strcpy(entry->name, name);

        // try to set the value
        result = var_set_value(entry, value);
        if (result) {
            return result;
        }

        // Add the entry to the linked list
        entry->next = var_first_entry;
        var_first_entry = entry;

        // Return success
        return 0;
    }
}

/**
 * Get the value of a variable
 *
 * @param name the name of the variable to set
 * @return pointer to the string on success, 0 if not found
 */
const char * var_get(const char *name) {
    TRACE("var_get");
    p_var_entry entry = var_find(name);
    if (entry) {
        return entry->value;
    } else {
        return 0;
    }
}
