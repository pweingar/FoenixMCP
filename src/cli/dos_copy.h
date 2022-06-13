/** @file: dos_copy.h
 * Provide the various functions needed for the COPY command
 */

#ifndef __DOS_COPY_H
#define __DOS_COPY_H

/**
 * The DOS COPY command itself:
 * COPY <src path> <dst path>
 */
extern short cmd_copy(short screen, int argc, const char * argv[]);

#endif
