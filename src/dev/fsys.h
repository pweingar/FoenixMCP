/**
 * Kernel calls for file system access
 */

#ifndef __FSYS_H
#define __FSYS_H

#include "constants.h"
#include "types.h"

#define DEFAULT_CHUNK_SIZE  256

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

/*
 * Pointer type for file loaders
 *
 * short loader(short chan, destination, start);
 */

typedef short (*p_file_loader)(short chan, long destination, long * start);

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
 * Check to see if the file is present.
 * If it is not, return a file not found error.
 * If it is, populate the file info record
 *
 * Inputs:
 * path = the path to the file to check
 * file = pointer to a file info record to fill in, if the file is found.
 */
extern short fsys_stat(const char * path, p_file_info file);

/*
 * Get the label for the drive holding the path
 *
 * Inputs:
 * path = path to the drive
 * label = buffer that will hold the label... should be at least 35 bytes
 */
extern short fsys_getlabel(char * path, char * label);

/*
 * Set the label for the drive holding the path
 *
 * Inputs:
 * drive = drive number
 * label = buffer that holds the label
 */
extern short fsys_setlabel(short drive, const char * label);

/*
 * Format a drive
 *
 * Inputs:
 * drive = drive number
 * label = the label to apply to the drive
 */
extern short fsys_mkfs(short drive, char * label);

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
extern short fsys_set_cwd(const char * path);

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
extern short fsys_get_cwd(char * path, short size);

/*
 * Load a file into memory at the designated destination address.
 *
 * If destination = 0, the file must be in a recognized binary format
 * that specifies its own loading address.
 *
 * Inputs:
 * path = the path to the file to load
 * destination = the destination address (0 for use file's address)
 * start = pointer to the long variable to fill with the starting address
 *         (0 if not an executable, any other number if file is executable
 *         with a known starting address)
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern short fsys_load(const char * path, long destination, long * start);

/*
 * Register a file loading routine
 *
 * A file loader, takes a channel number to load from and returns a
 * short that is the status of the load.
 *
 * Inputs:
 * extension = the file extension to map to
 * loader = pointer to the file load routine to add
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern short fsys_register_loader(const char * extension, p_file_loader loader);

#endif
