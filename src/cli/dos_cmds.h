/*
 * Declare DOS commands
 */

#ifndef __DOS_CMDS_H
#define __DOS_CMDS_H

/*
 * Fetch and display the MBR for the specified device:
 *
 * Inputs:
 * drive = "@S" for SDC, "@H" for hard drive
 */
extern short cmd_dump_mbr(char * drive);

/*
 * List the directory at the given path
 */
extern short cmd_dir(short screen, int argc, char * argv[]);

/*
 * Print the contents of a file to the screen
 */
extern short cmd_type(short screen, int argc, char * argv[]);

/*
 * Load a binary file into memory
 */
extern short cmd_load(short screen, int argc, char * argv[]);

#endif
