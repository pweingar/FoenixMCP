
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

/*
 * Definitions of special characters
 */

#define CHAR_ESC    '\x1B'  /* Escape character */
#define CHAR_TAB    '\t'    /* Vertical tab */
#define CHAR_CR     '\x0D'  /* Carriage return */
#define CHAR_NL     '\x0A'  /* Linefeed */
#define CHAR_BS     '\b'    /* Backspace */

/*
 * File access mode and open method flags
 */
#define	FSYS_READ			0x01
#define	FSYS_WRITE		    0x02
#define	FSYS_OPEN_EXISTING  0x00
#define	FSYS_CREATE_NEW		0x04
#define	FSYS_CREATE_ALWAYS	0x08
#define	FSYS_OPEN_ALWAYS	0x10
#define	FSYS_OPEN_APPEND	0x30

/*
 * File attribute bits for directory entry
 */
#define	FSYS_AM_RDO         0x01	/* Read only */
#define	FSYS_AM_HID         0x02	/* Hidden */
#define	FSYS_AM_SYS         0x04	/* System */
#define FSYS_AM_DIR         0x10	/* Directory */
#define FSYS_AM_ARC         0x20	/* Archive */

#endif
