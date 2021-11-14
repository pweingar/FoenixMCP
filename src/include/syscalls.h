/*
 * Declarations for calling into the Foenix/MCP kernel
 *
 * Code calling into the MCP should include this or a similar set of bindings
 * The kernel will use this to allow for system calls to be replaced if needed
 * by other code.
 *
 */

#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include "types.h"
#include "interrupt.h"
#include "dev/channel.h"
#include "dev/block.h"
#include "dev/fsys.h"
#include "dev/rtc.h"

/*
 * Syscall function numbers
 */

/* Core calls */

#define KFN_EXIT                0x00    /* Quick the current program and return to the command line */
#define KFN_WARMBOOT            0x01    /* Do a soft re-initialization */
#define KFN_INT_REGISTER        0x02    /* Set a handler for an exception / interrupt */
#define KFN_INT_ENABLE          0x03    /* Enable an interrupt */
#define KFN_INT_DISABLE         0x04    /* Disable an interrupt */
#define KFN_INT_ENABLE_ALL      0x05    /* Enable all interrupts */
#define KFN_INT_DISABLE_ALL     0x06    /* Disable all interrupts */
#define KFN_INT_CLEAR           0x07    /* Clear (acknowledge) an interrupt */
#define KFN_INT_PENDING         0x08    /* Return true if the interrupt is pending */
#define KFN_SYS_GET_INFO        0x09    /* Get information about the computer */

/* Channel system calls */

#define KFN_CHAN_READ           0x10    /* Read bytes from an input channel */
#define KFN_CHAN_READ_B         0x11    /* Read a byte from an input channel */
#define KFN_CHAN_READ_LINE      0x12    /* Read a line from an input channel */
#define KFN_CHAN_WRITE          0x13    /* Write bytes to an output channel */
#define KFN_CHAN_WRITE_B        0x14    /* Write a byte to an output channel */
#define KFN_CHAN_FLUSH          0x15    /* Ensure that any pending write on a channel is committed */
#define KFN_CHAN_SEEK           0x16    /* Set the input/output cursor on a channel to a given position */
#define KFN_CHAN_STATUS         0x17    /* Get the status of a channel */
#define KFN_CHAN_IOCTRL         0x18    /* Send a command to a channel (channel dependent functionality) */
#define KFN_CHAN_REGISTER       0x19    /* Register a channel device driver */
#define KFN_CHAN_OPEN           0x1A    /* Open a channel device */
#define KFN_CHAN_CLOSE          0x1B    /* Close an open channel (not for files) */
#define KFN_TEXT_SETSIZES       0x1C    /* Adjusts the screen size based on the current graphics mode */


/* Block device system calls */

#define KFN_BDEV_GETBLOCK       0x20    /* Read a block from a block device */
#define KFN_BDEV_PUTBLOCK       0x21    /* Write a block to a block device */
#define KFN_BDEV_FLUSH          0x22    /* Ensure that any pending write on a block device is committed */
#define KFN_BDEV_STATUS         0x23    /* Get the status of a block device */
#define KFN_BDEV_IOCTRL         0x24    /* Send a command to a block device (device dependent functionality) */
#define KFN_BDEV_REGISTER       0x25    /* Register a block device driver */

/* File/Directory system calls */

#define KFN_OPEN                0x30    /* Open a file as a channel */
#define KFN_CLOSE               0x31    /* Close a file channel */
#define KFN_OPENDIR             0x32    /* Open a directory for reading */
#define KFN_CLOSEDIR            0x33    /* Close an open directory */
#define KFN_READDIR             0x34    /* Read the next directory entry in an open directory */
#define KFN_FINDFIRST           0x35    /* Find the first entry in a directory that matches a pattern */
#define KFN_FINDNEXT            0x36    /* Find the next entry in a directory that matches a pattern */
#define KFN_DELETE              0x37    /* Delete a file */
#define KFN_RENAME              0x38    /* Rename a file */
#define KFN_MKDIR               0x39    /* Create a directory */
#define KFN_LOAD                0x3A    /* Load a file into memory */
#define KFN_GET_LABEL           0x3B    /* Read the label of a volume */
#define KFN_SET_LABEL           0x3C    /* Set the label of a volume */
#define KFN_SET_CWD             0x3D    /* Set the current working directory */
#define KFN_GET_CWD             0x3E    /* Get the current working directory */
#define KFN_LOAD_REGISTER       0x3F    /* Register a file type handler for executable binaries */

/* Process and memory calls */

#define KFN_RUN                 0x40    /* Load an execute a binary file */

/* Misc calls */

#define KFN_TIME_JIFFIES        0x50    /* Gets the current time code (increments since boot) */
#define KFN_TIME_SETRTC         0x51    /* Set the real time clock date-time */
#define KFN_TIME_GETRTC         0x52    /* Get the real time clock date-time */
#define KFN_KBD_SCANCODE        0x53    /* Get the next scan code from the keyboard */
#define KFN_KBD_LAYOUT          0x54    /* Set the translation tables for the keyboard */
#define KFN_ERR_MESSAGE         0x55    /* Return an error description, given an error number */

/*
 * Call into the kernel (provided by assembly)
 */
extern int32_t syscall(int32_t function, ...);

/***
 *** Core system calls
 ***/

/*
 * Quit the current user process
 *
 * NOTE: at the moment, this relaunches the CLI. In the future, this
 *       may cause execution to return to the program that started
 *       the user process.
 *
 * Inputs:
 * result = the code to return to the kernel
 */
extern void sys_exit(int result);

/*
 * Enable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 */
extern void sys_int_enable_all();

/*
 * Disable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 */
extern void sys_int_disable_all();

/*
 * Disable an interrupt by masking it
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
extern void sys_int_disable(unsigned short n);

/*
 * Enable an interrupt
 *
 * Inputs:
 * n = the number of the interrupt
 */
extern void sys_int_enable(unsigned short n);

/*
 * Register a handler for a given interrupt.
 *
 * Inputs:
 * n = the number of the interrupt
 * handler = pointer to the interrupt handler to register
 *
 * Returns:
 * the pointer to the previous interrupt handler
 */
extern p_int_handler sys_int_register(unsigned short n, p_int_handler handler);

/*
 * Return true (non-zero) if an interrupt is pending for the given interrupt
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 *
 * Returns:
 * non-zero if interrupt n is pending, 0 if not
 */
extern short sys_int_pending(unsigned short n);

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
extern void sys_get_info(p_sys_info info);

/*
 * Acknowledge an interrupt (clear out its pending flag)
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
extern void sys_int_clear(unsigned short n);

/***
 *** Channel system calls
 ***/

/*
 * Read a single byte from the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  the value read (if negative, error)
 */
extern short sys_chan_read_b(short channel);

/*
 * Read bytes from the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *  buffer = the buffer into which to copy the channel data
 *  size = the size of the buffer.
 *
 * Returns:
 *  number of bytes read, any negative number is an error code
 */
extern short sys_chan_read(short channel, unsigned char * buffer, short size);

/*
 * Read a line of text from the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *  buffer = the buffer into which to copy the channel data
 *  size = the size of the buffer
 *
 * Returns:
 *  number of bytes read, any negative number is an error code
 */
extern short sys_chan_readline(short channel, unsigned char * buffer, short size);

/*
 * Write a single byte to the device
 *
 * Inputs:
 *  channel = the number of the channel
 *  b = the byte to write
 *
 * Returns:
 *  0 on success, a negative value on error
 */
extern short sys_chan_write_b(short channel, unsigned char b);

/*
 * Write a byte to the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *  b = the byte to write
 *
 * Returns:
 *  number of bytes written, any negative number is an error code
 */
extern short sys_chan_write(short channel, unsigned char * buffer, short size);

/*
 * Return the status of the channel device
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  the status of the device
 */
extern short sys_chan_status(short channel);

/*
 * Ensure that any pending writes to teh device have been completed
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short sys_chan_flush(short channel);

/*
 * Attempt to set the position of the channel cursor (if supported)
 *
 * Inputs:
 *  channel = the number of the channel
 *  position = the position of the cursor
 *  base = whether the position is absolute or relative to the current position
 *
 * Returns:
 *  0 = success, a negative number is an error.
 */
extern short sys_chan_seek(short channel, long position, short base);

/*
 * Issue a control command to the device
 *
 * Inputs:
 *  channel = the number of the channel
 *  command = the number of the command to send
 *  buffer = pointer to bytes of additional data for the command
 *  size = the size of the buffer
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short sys_chan_ioctrl(short channel, short command, uint8_t * buffer, short size);

/*
 * Open a channel
 *
 * Inputs:
 * dev = the device number to have a channel opened
 * path = a "path" describing how the device is to be open
 * mode = is the device to be read, written, both? (0x01 = READ flag, 0x02 = WRITE flag, 0x03 = READ and WRITE)
 *
 * Returns:
 * the number of the channel opened, negative number on error
 */
extern short sys_chan_open(short dev, uint8_t * path, short mode);

/*
 * Close a channel
 *
 * Inputs:
 * chan = the number of the channel to close
 *
 * Returns:
 * nothing useful
 */
extern short sys_chan_close(short chan);

/*
 * Compute the size information for the text screen based on the current settings in VICKY
 * These settings are needed to correctly position text on the screen.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
extern void text_setsizes(short chan);

/***
 *** Block device system calls
 ***/

//
// Register a block device driver
//
extern short sys_bdev_register(p_dev_block device);

//
// Read a block from the device
//
// Inputs:
//  dev = the number of the device
//  lba = the logical block address of the block to read
//  buffer = the buffer into which to copy the block data
//  size = the size of the buffer.
//
// Returns:
//  number of bytes read, any negative number is an error code
//
extern short sys_bdev_read(short dev, long lba, unsigned char * buffer, short size);

//
// Write a block from the device
//
// Inputs:
//  dev = the number of the device
//  lba = the logical block address of the block to write
//  buffer = the buffer containing the data to write
//  size = the size of the buffer.
//
// Returns:
//  number of bytes written, any negative number is an error code
//
extern short sys_bdev_write(short dev, long lba, const unsigned char * buffer, short size);

//
// Return the status of the block device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  the status of the device
//
extern short sys_bdev_status(short dev);

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short sys_bdev_flush(short dev);

//
// Issue a control command to the device
//
// Inputs:
//  dev = the number of the device
//  command = the number of the command to send
//  buffer = pointer to bytes of additional data for the command
//  size = the size of the buffer
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short sys_bdev_ioctrl(short dev, short command, unsigned char * buffer, short size);


/*
 * File System Calls
 */

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
extern short sys_fsys_open(const char * path, short mode);

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * fd = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
extern short sys_fsys_close(short fd);

/**
 * Attempt to open a directory for scanning
 *
 * Inputs:
 * path = the path to the directory to open
 *
 * Returns:
 * the handle to the directory if >= 0. An error if < 0
 */
extern short sys_fsys_opendir(const char * path);

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * fd = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
extern short sys_fsys_close(short fd);

/**
 * Attempt to open a directory for scanning
 *
 * Inputs:
 * path = the path to the directory to open
 *
 * Returns:
 * the handle to the directory if >= 0. An error if < 0
 */
extern short sys_fsys_opendir(const char * path);

/**
 * Close a previously open directory
 *
 * Inputs:
 * dir = the directory handle to close
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern short sys_fsys_closedir(short dir);

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
extern short sys_fsys_readdir(short dir, p_file_info file);

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
extern short sys_fsys_findfirst(const char * path, const char * pattern, p_file_info file);

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
extern short sys_fsys_findnext(short dir, p_file_info file);

/*
 * Get the label for the drive holding the path
 *
 * Inputs:
 * path = path to the drive
 * label = buffer that will hold the label... should be at least 35 bytes
 */
extern short sys_fsys_get_label(const char * path, char * label);

/*
 * Set the label for the drive holding the path
 *
 * Inputs:
 * drive = drive number
 * label = buffer that holds the label
 */
extern short sys_fsys_set_label(short drive, const char * label);

/**
 * Create a directory
 *
 * Inputs:
 * path = the path of the directory to create.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short sys_fsys_mkdir(const char * path);

/**
 * Delete a file or directory
 *
 * Inputs:
 * path = the path of the file or directory to delete.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short sys_fsys_delete(const char * path);

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
extern short sys_fsys_rename(const char * old_path, const char * new_path);

/**
 * Change the current working directory (and drive)
 *
 * Inputs:
 * path = the path that should be the new current
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern short sys_fsys_setcwd(const char * path);

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
extern short sys_fsys_getcwd(char * path, short size);

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
extern short sys_fsys_load(const char * path, long destination, long * start);

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
extern short sys_fsys_register_loader(const char * extension, p_file_loader loader);

/*
 * Miscellaneous
 */

/*
 * Get the number of jiffies since the system last booted.
 *
 * NOTE: a jiffie is 1/60 of a second. This timer will not be
 *       100% precise, so it should be used for timeout purposes
 *       where precision is not critical.
 *
 * Returns:
 * the number of jiffies since the last reset
 */
extern long sys_time_jiffies();

/*
 * Set the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record containing the correct time
 */
extern void sys_rtc_set_time(p_time time);

/*
 * Get the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record in which to put the current time
 */
extern void sys_rtc_get_time(p_time time);

/*
 * Return the next scan code from the keyboard... 0 if nothing pending
 */
extern unsigned short sys_kbd_scancode();

/*
 * Return an error message given an error number
 */
extern const char * sys_err_message(short err_number);

/*
 * Set the keyboard translation tables
 *
 * The translation tables provided to the keyboard consist of eight
 * consecutive tables of 128 characters each. Each table maps from
 * the MAKE scan code of a key to its appropriate 8-bit character code.
 *
 * The tables included must include, in order:
 * - UNMODIFIED: Used when no modifier keys are pressed or active
 * - SHIFT: Used when the SHIFT modifier is pressed
 * - CTRL: Used when the CTRL modifier is pressed
 * - CTRL-SHIFT: Used when both CTRL and SHIFT are pressed
 * - CAPSLOCK: Used when CAPSLOCK is down but SHIFT is not pressed
 * - CAPSLOCK-SHIFT: Used when CAPSLOCK is down and SHIFT is pressed
 * - ALT: Used when only ALT is presse
 * - ALT-SHIFT: Used when ALT is pressed and either CAPSLOCK is down
 *   or SHIFT is pressed (but not both)
 *
 * Inputs:
 * tables = pointer to the keyboard translation tables
 */
extern short sys_kbd_layout(const char * tables);

#endif
