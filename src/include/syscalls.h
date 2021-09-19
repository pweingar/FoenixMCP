/*
 * Declarations for calling into the Foenix/MCP kernel
 *
 * Code calling into the MCP should include this or a similar set of bindings
 * The kernel will use this to allow for system calls to be replaced if needed
 * by other code.
 *
 */

#ifndef __SYSCALLS_M68K_H
#define __SYSCALLS_M68K_H

#include "types.h"

/*
 * Syscall function numbers
 */

/* Miscellaneous control calls */

#define KFN_EXIT                0x00    /* Quick the current program and return to the command line */
#define KFN_WARMBOOT            0x01    /* Do a soft re-initialization */
#define KFN_SETHANDLER          0x02    /* Set a handler for an exception / interrupt */
#define KFN_SETINTERRUPT        0x03    /* Enable / Disable an interrupt */

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
#define KFN_TEXT_SETSIZES       0x1A    /* Adjusts the screen size based on the current graphics mode */

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
#define KFN_SAVE                0x3B    /* Save a block of memory to a file */

/* Process system calls */

#define KFN_RUN                 0x40    /* Load an execute a binary file */
#define KFN_LOAD_REGISTER       0x41    /* Register a file type handler for executable binaries */

/* Timer calls */

#define KFN_DELAY               0x50    /* Block for a certain amount of time */
#define KFN_SET_ALARM           0x51    /* Set an alarm for a certain amount of time */
#define KFN_GET_TIMECODE        0x52    /* Gets the current time code (increments since boot) */
#define KFN_SET_DATETIME        0x53    /* Set the real time clock date-time */
#define KFN_GET_DATETIME        0x54    /* Get the real time clock date-time */

/*
 * Call into the kernel (provided by assembly)
 */
extern int32_t syscall(int32_t function, ...);

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


#endif
