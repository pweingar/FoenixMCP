/*
 * Declare DOS commands
 */

#ifndef __DOS_CMDS_H
#define __DOS_CMDS_H

/*
 * Execute a binary file
 */
extern short cmd_run(short screen, int argc, const char * argv[]);

/*
 * List the directory at the given path
 */
extern short cmd_dir(short screen, int argc, const char * argv[]);

/*
 * Create a directory
 */
extern short cmd_mkdir(short screen, int argc, const char * argv[]);

/*
 * Delete a file or directory
 */
extern short cmd_del(short screen, int argc, const char * argv[]);

/*
 * Copies file(s) to destination
 */
extern short cmd_copy(short screen, int argc, const char * argv[]);

/*
 * Set the current working directory
 */
extern short cmd_cd(short screen, int argc, const char * argv[]);

/*
 * Print the current working directory
 */
extern short cmd_pwd(short screen, int argc, const char * argv[]);

/*
 * Rename a file or directory
 */
extern short cmd_rename(short screen, int argc, const char * argv[]);

/*
 * Print the contents of a file to the screen
 */
extern short cmd_type(short screen, int argc, const char * argv[]);

/*
 * Load a binary file into memory
 */
extern short cmd_load(short screen, int argc, const char * argv[]);

/*
 * Read a sector off a drive
 *
 * DISKREAD <drive #> <sector #>
 */
extern short cmd_diskread(short screen, int argc, const char * argv[]);

/*
 * Fill a sector of a drive with a byte value
 *
 * DISKFILL <drive #> <sector #> <value>
 */
extern short cmd_diskfill(short screen, int argc, const char * argv[]);

/*
 * Set the label of a drive
 *
 * LABEL <drive #> <label>
 *
 */
extern short cmd_label(short screen, int argc, const char * argv[]);

/*
 * Format a drive
 *
 * FORMAT <drive #>
 */
extern short cmd_format(short screen, int argc, const char * argv[]);

/**
 * Command to make a device bootable by writing to the MBR or VBR
 *
 * MKBOOT <drive #> -r --- removes boot record
 * MKBOOT <drive #> -b <boot record path> --- installs boot record
 * MKBOOT <drive #> -s <start file path> --- defines a startup file
 */
extern short cmd_mkboot(short screen, int argc, const char * argv[]);

#endif
