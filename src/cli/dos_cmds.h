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
extern short cmd_dir(char * path);

extern short cmd_type(short screen, char * path);

#endif
