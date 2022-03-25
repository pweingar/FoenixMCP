
/**
 * Definitions of major constants
 */

#ifndef __CONSTANTS_H
#define __CONSTANTS_H

/*
 * Miscellaneous definitions
 */

#define FSYS_SECTOR_SZ  512     /* Size of a sector */
#define MAX_PATH_LEN    256     /* Maximum length of a file path */
#define MAX_TRIES_BUSY  100000  /* The maximum number of times to check for an operation to complete (general purpose) */

/*
 * Definitions of special characters
 */

#define CHAR_ESC    '\x1B'  /* Escape character */
#define CHAR_TAB    '\t'    /* Vertical tab */
#define CHAR_CR     '\x0D'  /* Carriage return */
#define CHAR_NL     '\x0A'  /* Linefeed */
#define CHAR_BS     '\b'    /* Backspace */

#endif
