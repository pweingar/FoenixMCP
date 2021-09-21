/**
 * Kernel calls for file system access
 */

#ifndef __FSYS_H
#define __FSYS_H

#include "types.h"

#define MAX_PATH_LEN 256

/**
 * Type for directory information about a file
 */
typedef struct s_file_info {
    long size;
    unsigned short date;
    unsigned short time;
    unsigned char attributes;
    char name[MAX_PATH_LEN];
} t_file_info, * p_file_info;

/**
 * Initialize the file system
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short fsys_init();

/**
 * Attempt to open a file given the path to the file and the mode.
 *
 * Inputs:
 * path = the ASCIIZ string containing the path to the file.
 * mode = the mode (e.g. r/w/create)
 *
 * Returns:
 * the channel ID for the open file (negative if error)
 */
extern short fsys_open(const char * path, short mode);

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * fd = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
extern short fsys_close(short fd);

/**
 * N.B.: fsys_open returns a channel ID, and fsys_close accepts a channel ID.
 * read and write access, seek, eof status, etc. will be handled by the channel
 * calls.
 */

/**
 * Attempt to open a directory for scanning
 *
 * Inputs:
 * path = the path to the directory to open
 *
 * Returns:
 * the handle to the directory if >= 0. An error if < 0
 */
extern short fsys_opendir(const char * path);

/**
 * Close a previously open directory
 *
 * Inputs:
 * dir = the directory handle to close
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern short fsys_closedir(short dir);

/**
 * Attempt to read an entry from an open directory
 *
 * Inputs:
 * dir = the handle of the open directory
 * file = pointer to the t_file_info structure to fill out.
 *
 * Returns:
 * 0 on success, negative number on failure
 */
extern short fsys_readdir(short dir, p_file_info file);

/**
 * Open a directory given the path and search for the first file matching the pattern.
 *
 * Inputs:
 * path = the path to the directory to search
 * pattern = the file name pattern to search for
 * file = pointer to the t_file_info structure to fill out
 *
 * Returns:
 * the directory handle to use for subsequent calls if >= 0, error if negative
 */
extern short fsys_findfirst(const char * path, const char * pattern, p_file_info file);

/**
 * Open a directory given the path and search for the first file matching the pattern.
 *
 * Inputs:
 * dir = the handle to the directory (returned by fsys_findfirst) to search
 * file = pointer to the t_file_info structure to fill out
 *
 * Returns:
 * 0 on success, error if negative
 */
extern short fsys_findnext(short dir, p_file_info file);

/**
 * Create a directory
 *
 * Inputs:
 * path = the path of the directory to create.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short fsys_mkdir(const char * path);

/**
 * Delete a file or directory
 *
 * Inputs:
 * path = the path of the file or directory to delete.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short fsys_delete(const char * path);

/**
 * Rename a file or directory
 *
 * Inputs:
 * old_path = the current path to the file
 * new_path = the new path for the file
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short fsys_rename(const char * old_path, const char * new_path);

/**
 * Change the current working directory (and drive)
 *
 * Inputs:
 * path = the path that should be the new current
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short fsys_setcwd(const char * path);

/**
 * Get the current working drive and directory
 *
 * Inputs:
 * path = the buffer in which to store the directory
 * size = the size of the buffer in bytes
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short fsys_getcwd(char * path, short size);

#endif
