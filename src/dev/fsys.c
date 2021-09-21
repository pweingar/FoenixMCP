/**
 * Kernel calls for file system access
 *
 * This version will be a wrapper around the FATfs file system.
 * In theory, there could be other implementations.
 *
 */

#include <string.h>
#include "log.h"
#include "syscalls.h"
#include "fsys.h"
#include "fatfs/ff.h"
#include "dev/channel.h"
#include "errors.h"

#define MAX_DRIVES      8       /* Maximum number of drives */
#define MAX_DIRECTORIES 8       /* Maximum number of open directories */
#define MAX_FILES       8       /* Maximum number of open files */

/**
 * Module variables
 */

FATFS g_drive[MAX_DRIVES];                  /* File system for each logical drive */
unsigned char g_dir_state[MAX_DIRECTORIES]; /* Whether or not a directory record is allocated */
DIR g_directory[MAX_DIRECTORIES];           /* The directory information records */
unsigned char g_fil_state[MAX_FILES];       /* Whether or not a file descriptor is allocated */
FIL g_file[MAX_FILES];                      /* The file descriptors */
t_dev_chan g_file_dev;                      /* The descriptor to use for the file channels */


/**
 * Convert a FATFS FRESULT code to the Foenix kernel's internal error codes
 *
 * Inputs:
 * r = the FRESULT code from a FATFS call
 *
 * Output:
 * The corresponding Foenix kernel error code (0 on success)
 */
short fatfs_to_foenix(FRESULT r) {
    if (r == 0) {
        return 0;
    } else {
        /* TODO: flesh this out */
        return -1;
    }
}

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
short fsys_open(const char * path, short mode) {
    p_channel chan = 0;
    short i, fd = -1;

    /* Allocate a file handle */

    for (i = 0; i < MAX_FILES; i++) {
        if (g_fil_state[i] == 0) {
            g_fil_state[i] = 1;
            fd = i;
            break;
        }
    }

    if (fd < 0) {
        return ERR_OUT_OF_HANDLES;
    }

    /* Allocate a channel */

    chan = chan_alloc();
    if (chan) {
        chan->dev = CDEV_FILE;
        FRESULT result = f_open(&g_file[fd], path, mode);
        if (result == 0) {
            chan->data[0] = fd * 0xff;      /* file handle in the channel data block */
            return fd;
        } else {
            /* There was an error... deallocate the channel and file descriptor */
            g_fil_state[fd] = 0;
            chan_free(chan);
            return fatfs_to_foenix(result);
        }

    } else {
        /* We couldn't allocate a channel... return our file descriptor */
        g_fil_state[fd] = 0;
        return ERR_OUT_OF_HANDLES;
    }
}

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * c = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
short fsys_close(short c) {
    p_channel chan = 0;
    short fd = 0;
    FRESULT result;

    chan = chan_get_record(c);          /* Get the channel record */
    fd = chan->data[0];                 /* Get the file descriptor number */

    result = f_close(&g_file[fd]);      /* Close the file in FATFS */
    chan_free(chan);                    /* Return the channel to the pool */
    g_fil_state[fd] = 0;                /* Return the file descriptor to the pool. */

    return fatfs_to_foenix(result);
}

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
short fsys_opendir(const char * path) {
    return -1;
}

/**
 * Close a previously open directory
 *
 * Inputs:
 * dir = the directory handle to close
 *
 * Returns:
 * 0 on success, negative number on error
 */
short fsys_closedir(short dir) {
    return -1;
}

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
short fsys_readdir(short dir, p_file_info file) {
    return -1;
}

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
short fsys_findfirst(const char * path, const char * pattern, p_file_info file) {
    return -1;
}

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
short fsys_findnext(short dir, p_file_info file) {
    return -1;
}

/**
 * Create a directory
 *
 * Inputs:
 * path = the path of the directory to create.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
short fsys_mkdir(const char * path) {
    return -1;
}

/**
 * Delete a file or directory
 *
 * Inputs:
 * path = the path of the file or directory to delete.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
short fsys_delete(const char * path) {
    return -1;
}

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
short fsys_rename(const char * old_path, const char * new_path) {
    return -1;
}

/**
 * Change the current working directory (and drive)
 *
 * Inputs:
 * path = the path that should be the new current
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
short fsys_setcwd(const char * path) {
    return -1;
}

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
short fsys_getcwd(char * path, short size) {
    return -1;
}

short fchan_init() {
    /* Nothing needed here */
    return 0;
}

/**
 * Channel driver routines for files.
 */
FIL * fchan_to_file(t_channel * chan) {
    short fd;

    fd = chan->data[0];         /* Get the file descriptor number */
    if (fd < MAX_FILES) {
        return &g_file[fd];     /* Return the pointer to the file descriptor */
    } else {
        return 0;               /* Return NULL if fd is out of range */
    }
}

/**
 * Read a a buffer from the device
 */
short fchan_read(t_channel * chan, unsigned char * buffer, short size) {
    FIL * file;
    FRESULT result;
    int total_read;

    file = fchan_to_file(chan);
    if (file) {
        result = f_read(file, buffer, size, &total_read);
        if (result == FR_OK) {
            return (short)total_read;
        } else {
            return fatfs_to_foenix(result);
        }
    }

    return ERR_BADCHANNEL;
}

/**
 * Read a line of text from the device
 */
short fchan_readline(t_channel * chan, unsigned char * buffer, short size) {
    FIL * file;
    char * result;
    int total_read;

    file = fchan_to_file(chan);
    if (file) {
        result = f_gets(buffer, size, file);
        if (result) {
            return strlen(buffer);
        } else {
            return fatfs_to_foenix(f_error(file));
        }
    }

    return ERR_BADCHANNEL;
}

/**
 * read a single byte from the device
 */
short fchan_read_b(t_channel * chan) {
    return 0;
}

/**
 * Write a buffer to the device
 */
short fchan_write(p_channel chan, const unsigned char * buffer, short size) {
    FIL * file;
    FRESULT result;
    int total_written;

    file = fchan_to_file(chan);
    if (file) {
        result = f_write(file, buffer, size, &total_written);
        if (result == FR_OK) {
            return (short)total_written;
        } else {
            return fatfs_to_foenix(result);
        }
    }

    return ERR_BADCHANNEL;
}

/**
 * Write a single unsigned char to the device
 */
short fchan_write_b(t_channel * chan, const unsigned char b) {
    FIL * file;
    FRESULT result;
    int total_written;
    unsigned char buffer[1];

    file = fchan_to_file(chan);
    if (file) {
        buffer[0] = b;
        result = f_write(file, buffer, 1, &total_written);
        if (result == FR_OK) {
            return (short)total_written;
        } else {
            return fatfs_to_foenix(result);
        }
    }

    return ERR_BADCHANNEL;
}

/**
 * Get the status of the device
 */
short fchan_status(t_channel * chan) {
    FIL * file;
    FRESULT result;
    int total_written;

    file = fchan_to_file(chan);
    if (file) {
        short status = 0;

        if (f_eof(file)) {
            status |= CDEV_STAT_EOF;
        }

        if (f_error(file) != FR_OK) {
            status |= CDEV_STAT_ERROR;
        }

        return status;
    }

    return ERR_BADCHANNEL;
}

/**
 * Ensure that any pending writes to the device have been completed
 */
short fchan_flush(t_channel * chan) {
    FIL * file;
    FRESULT result;
    int total_written;

    file = fchan_to_file(chan);
    if (file) {
        result = f_sync(file);
        return fatfs_to_foenix(result);
    }

    return ERR_BADCHANNEL;
}

/**
 * attempt to move the "cursor" position in the channel
 */
short fchan_seek(t_channel * chan, long position, short base) {
    FIL * file;
    FRESULT result;
    int total_written;

    file = fchan_to_file(chan);
    if (file) {
        if (base == CDEV_SEEK_ABSOLUTE) {
            result = f_lseek(file, position);
            return fatfs_to_foenix(result);
        } else if (base == CDEV_SEEK_RELATIVE) {
            long current = f_tell(file);
            result = f_lseek(file, current + position);
            return fatfs_to_foenix(result);
        }
    }

    return ERR_BADCHANNEL;
}

/**
 * Issue a control command to the device
 */
short fchan_ioctrl(t_channel * chan, short command, unsigned char * buffer, short size) {
    return 0;
}

/*
 * Mount, or remount the block device
 *
 * For the hard drive, this need be called only once, but for removable
 * devices, this should be called whenever the media has changed.
 *
 * Inputs:
 * bdev = the number of the block device to mount or re-mount
 *
 * Returns:
 * 0 on success, any other number is an error
 */
short fsys_mount(short bdev) {
    FRESULT fres;
    char drive[3];
    drive[0] = '0' + (char)bdev;
    drive[1] = ':';
    drive[2] = 0;

    fres = f_mount(&g_drive[bdev], drive, 0);
    if (fres != FR_OK) {
        DEBUG("Unable to mount drive:");
        DEBUG(drive);
        return fres;
    } else {
        return 0;
    }
}

/**
 * Initialize the file system
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
short fsys_init() {
    int i;

    /* Mark all directories as available */
    for (i = 0; i < MAX_DIRECTORIES; i++) {
        g_dir_state[i] = 0;
    }

    /* Mark all file descriptors as available */
    for (i = 0; i < MAX_FILES; i++) {
        g_fil_state[i] = 0;
    }

    /* Mount all logical drives that are present */

    for (i = 0; i < MAX_DRIVES; i++) {
        short res = sys_bdev_status((short)i);
        if (res >= 0) {
            fsys_mount(i);
        }
    }

    /* Register the channel driver for files. */

    g_file_dev.number = CDEV_FILE;
    g_file_dev.name = "FILE";
    g_file_dev.init = fchan_init;
    g_file_dev.ioctrl = fchan_ioctrl;
    g_file_dev.read = fchan_read;
    g_file_dev.read_b = fchan_read_b;
    g_file_dev.readline = fchan_readline;
    g_file_dev.write = fchan_write;
    g_file_dev.write_b = fchan_write_b;
    g_file_dev.seek = fchan_seek;
    g_file_dev.status = fchan_status;
    g_file_dev.flush = fchan_flush;

    cdev_register(&g_file_dev);

    return 0;
}
