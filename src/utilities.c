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

/**
 * Perform a case-insensitive string comparison
 *
 * @param str1 the first string
 * @param str2 the second string
 * @return 0 if str1 = str2, -1 if str1 < str2, 1 if str1 > str2
 */
short strcicmp(char const *str1, char const *str2) {
    for (;; str1++, str2++) {
        short diff = tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
        if (diff != 0 || !*str1) {
            return diff;
        }
    }
}

/**
 * End the string at the first \r or \n
 *
 * @param text the string to alter
 */
void strtrimnl(char * text) {
    char * x;
    for (x = text; *x; x++) {
        if ((*x == '\r') || (*x == '\n')) {
            *x = 0;
            return;
        }
    }
}
