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

#endif
