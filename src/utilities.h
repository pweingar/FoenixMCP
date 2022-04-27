/**
 * @file utilities.h
 *
 * Define some handy macros and utility functions
 */

#ifndef __UTILITIES_H
#define __UTILITIES_H

/** Return the minimum value of x or y */
#define min(x, y)   ((x < y) ? x : y)

/** Return the maximum value of x or y */
#define max(x, y)   ((x < y) ? y : x)

/** Return the absolute value of x */
#define abs(x)      ((x >= 0) ? x : 0 - x)

/**
 * Re-entrant version of strtok_r, because VBCC does not provide it
 *
 * @param source the string to tokenize
 * @param delimiter the string containing the delimiter
 * @param saveptr a pointer to the character pointer to use to save the state of the tokenizer
 * @return the pointer to the next token, 0 if none
 */
extern char * strtok_r(char * source, const char * delimiter, char ** saveptr);

#endif
