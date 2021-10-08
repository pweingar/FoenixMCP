/*
 * Declare DOS commands
 */

#ifndef __DOS_CMDS_H
#define __DOS_CMDS_H

/*
 * Test the IDE interface by reading the MBR
 */
extern short cmd_testide(short screen, int argc, char * argv[]);

/*
 * Execute a binary file
 */
extern short cmd_run(short screen, int argc, char * argv[]);

/*
 * List the directory at the given path
 */
extern short cmd_dir(short screen, int argc, char * argv[]);

/*
 * Create a directory
 */
extern short cmd_mkdir(short screen, int argc, char * argv[]);

/*
 * Delete a file or directory
 */
extern short cmd_del(short screen, int argc, char * argv[]);

/*
 * Rename a file or directory
 */
extern short cmd_rename(short screen, int argc, char * argv[]);

/*
 * Print the contents of a file to the screen
 */
extern short cmd_type(short screen, int argc, char * argv[]);

/*
 * Load a binary file into memory
 */
extern short cmd_load(short screen, int argc, char * argv[]);

#endif
