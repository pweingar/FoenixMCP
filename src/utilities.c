/**
 * @file utilities.c
 *
 * Define some handy macros and utility functions
 */

#include <utilities.h>

/**
 * Re-entrant version of strtok_r, because VBCC does not provide it
 *
 * @param source the string to tokenize
 * @param delimiter the string containing the delimiter
 * @param saveptr a pointer to the character pointer to use to save the state of the tokenizer
 * @return the pointer to the next token, 0 if none
 */
char * strtok_r(char * source, const char * delimiter, char ** saveptr) {
    char * x = *saveptr;
    char * y;

    /* Skip over leading delimiters */
    for (x = *saveptr; *x && (*x == delimiter[0]); x++) {

    }

    /* If we reached the end of the string, return NULL */
    if (*x == 0) {
        return 0;
    }

    for (y = x; *y && (*y != delimiter[0]); y++) {

    }

    /* If we reached the end of the string, return x */
    if (*y == 0) {
        *saveptr = y;
        return x;
    }

    /* Otherwise, make that position in the source string NULL, and return x */
    *y = 0;
    *saveptr = y + 1;
    return x;
}
