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
#include "sys_general.h"
#include "dev/channel.h"
#include "dev/block.h"
#include "dev/fsys.h"
#include "dev/rtc.h"
#include "dev/txt_screen.h"

/*
 * Define the machine-specific system call function prefix
 */

#if MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS
#define SYSTEMCALL __attribute__((simple_call))
#else
#define SYSTEMCALL
#endif

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
#define KFN_CHAN_SWAP           0x1C    /* Swap the channel ID assignment of two channels */
#define KFN_CHAN_DEVICE         0x1D    /* Get the number of the device associated with the channel */


/* Block device system calls */

#define KFN_BDEV_GETBLOCK       0x20    /* Read a block from a block device */
#define KFN_BDEV_PUTBLOCK       0x21    /* Write a block to a block device */
#define KFN_BDEV_FLUSH          0x22    /* Ensure that any pending write on a block device is committed */
#define KFN_BDEV_STATUS         0x23    /* Get the status of a block device */
#define KFN_BDEV_IOCTRL         0x24    /* Send a command to a block device (device dependent functionality) */
#define KFN_BDEV_REGISTER       0x25    /* Register a block device driver */
#define KFN_STAT                0x2F    /* Check for file existance and return file information */

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
#define KFN_MEM_GET_RAMTOP      0x41    /* Get the upper limit of the top of system RAM */
#define KFN_MEM_RESERVE         0x42    /* Reserve a block of memory at the top of system RAM */
#define KFN_ELEVATE             0x43    /* Switch the user process to a full privilege */
#define KFN_VAR_SET             0x44    /* Set the value of a system variable */
#define KFN_VAR_GET             0x45    /* Get the value of a system variable */

/* Misc calls */

#define KFN_TIME_JIFFIES        0x50    /* Gets the current time code (increments since boot) */
#define KFN_TIME_SETRTC         0x51    /* Set the real time clock date-time */
#define KFN_TIME_GETRTC         0x52    /* Get the real time clock date-time */
#define KFN_KBD_SCANCODE        0x53    /* Get the next scan code from the keyboard */
#define KFN_KBD_LAYOUT          0x54    /* Set the translation tables for the keyboard */
#define KFN_ERR_MESSAGE         0x55    /* Return an error description, given an error number */

/* Text Device Calls */

#define KFN_TEXT_INIT_SCREEN    0x60    /* Reset a screen to its default mode */
#define KFN_TXT_GET_CAPS        0x61    /* Get the capabilities of a screen */
#define KFN_TXT_SET_MODE        0x62    /* Set the display mode of a screen */
#define KFN_TEXT_SETSIZES       0x63    /* Adjusts the screen size based on the current graphics mode */
#define KFN_TXT_SET_RESOLUTION  0x64    /* Set the base display resolution for a screen */
#define KFN_TXT_SET_BORDER      0x65    /* Set the size of the border */
#define KFN_TXT_SET_BORDERCOLOR 0x66    /* Set the border color */
#define KFN_TXT_SET_FONT        0x67    /* Set the font for the screen's text mode (if applicable) */
#define KFN_TXT_SET_CURSOR      0x68    /* Set the text-mode cursor look */
#define KFN_TXT_SET_REGION      0x69    /* Sets the clipping/scrolling region for further text operations */
#define KFN_TXT_GET_REGION      0x6A    /* Gets the current clipping/scrolling region */
#define KFN_TXT_SET_COLOR       0x6B    /* Sets the foreground and background text colors */
#define KFN_TXT_GET_COLOR       0x6C    /* Gets the foreground and background text colors */
#define KFN_TXT_SET_XY          0x6D    /* Sets the cursor's position */
#define KFN_TXT_GET_XY          0x6E    /* Gets the cursor's position */
#define KFN_TXT_SCROLL          0x6F    /* Scroll the current region */
// #define KFN_TXT_FILL            0x70    /* Fill the current region */
#define KFN_TXT_SET_CURSOR_VIS  0x71    /* Set cursor visibility */
#define KFN_TXT_GET_SIZES       0x72    /* Get the screen size (visible text cells and total pixel resolution) */

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
extern SYSTEMCALL void sys_exit(short result);

/*
 * Enable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 */
extern SYSTEMCALL void sys_int_enable_all();

/*
 * Disable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 */
extern SYSTEMCALL void sys_int_disable_all();

/*
 * Disable an interrupt by masking it
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
extern SYSTEMCALL void sys_int_disable(unsigned short n);

/*
 * Enable an interrupt
 *
 * Inputs:
 * n = the number of the interrupt
 */
extern SYSTEMCALL void sys_int_enable(unsigned short n);

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
extern SYSTEMCALL p_int_handler sys_int_register(unsigned short n, p_int_handler handler);

/*
 * Return true (non-zero) if an interrupt is pending for the given interrupt
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 *
 * Returns:
 * non-zero if interrupt n is pending, 0 if not
 */
extern SYSTEMCALL short sys_int_pending(unsigned short n);

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
extern SYSTEMCALL void sys_get_info(p_sys_info info);

/*
 * Acknowledge an interrupt (clear out its pending flag)
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
extern SYSTEMCALL void sys_int_clear(unsigned short n);

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
extern SYSTEMCALL short sys_chan_read_b(short channel);

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
extern SYSTEMCALL short sys_chan_read(short channel, unsigned char * buffer, short size);

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
extern SYSTEMCALL short sys_chan_readline(short channel, unsigned char * buffer, short size);

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
extern SYSTEMCALL short sys_chan_write_b(short channel, unsigned char b);

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
extern SYSTEMCALL short sys_chan_write(short channel, const unsigned char * buffer, short size);

/*
 * Return the status of the channel device
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  the status of the device
 */
extern SYSTEMCALL short sys_chan_status(short channel);

/*
 * Ensure that any pending writes to teh device have been completed
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern SYSTEMCALL short sys_chan_flush(short channel);

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
extern SYSTEMCALL short sys_chan_seek(short channel, long position, short base);

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
extern SYSTEMCALL short sys_chan_ioctrl(short channel, short command, uint8_t * buffer, short size);

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
extern SYSTEMCALL short sys_chan_open(short dev, const uint8_t * path, short mode);

/*
 * Close a channel
 *
 * Inputs:
 * chan = the number of the channel to close
 *
 * Returns:
 * nothing useful
 */
extern SYSTEMCALL short sys_chan_close(short chan);

/**
 * Swap the channel ID assignments for two channels
 *
 * Before call: channel1 = "Channel A", channel2 = "Channel B"
 * After call: channel1 = "Channel B", channel2 = "Channel A"
 *
 * @param channel1 the ID of one of the channels
 * @param channel2 the ID of the other channel
 * @return 0 on success, any other number is an error
 */
extern SYSTEMCALL short sys_chan_swap(short channel1, short channel2);

/**
 * Return the device associated with the channel
 *
 * @param channel the ID of the channel to query
 * @return the ID of the device associated with the channel, negative number for error
 */
extern SYSTEMCALL short sys_chan_device(short channel);

/*
 * Compute the size information for the text screen based on the current settings in VICKY
 * These settings are needed to correctly position text on the screen.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
extern SYSTEMCALL void sys_text_setsizes(short chan);

/***
 *** Block device system calls
 ***/

//
// Register a block device driver
//
extern SYSTEMCALL short sys_bdev_register(p_dev_block device);

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
extern SYSTEMCALL short sys_bdev_read(short dev, long lba, unsigned char * buffer, short size);

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
extern SYSTEMCALL short sys_bdev_write(short dev, long lba, const unsigned char * buffer, short size);

//
// Return the status of the block device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  the status of the device
//
extern SYSTEMCALL short sys_bdev_status(short dev);

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
extern SYSTEMCALL short sys_bdev_flush(short dev);

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
extern SYSTEMCALL short sys_bdev_ioctrl(short dev, short command, unsigned char * buffer, short size);


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
extern SYSTEMCALL short sys_fsys_open(const char * path, short mode);

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * fd = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
extern SYSTEMCALL short sys_fsys_close(short fd);

/**
 * Attempt to open a directory for scanning
 *
 * Inputs:
 * path = the path to the directory to open
 *
 * Returns:
 * the handle to the directory if >= 0. An error if < 0
 */
extern SYSTEMCALL short sys_fsys_opendir(const char * path);

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * fd = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
extern SYSTEMCALL short sys_fsys_close(short fd);

/**
 * Attempt to open a directory for scanning
 *
 * Inputs:
 * path = the path to the directory to open
 *
 * Returns:
 * the handle to the directory if >= 0. An error if < 0
 */
extern SYSTEMCALL short sys_fsys_opendir(const char * path);

/**
 * Close a previously open directory
 *
 * Inputs:
 * dir = the directory handle to close
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern SYSTEMCALL short sys_fsys_closedir(short dir);

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
extern SYSTEMCALL short sys_fsys_readdir(short dir, p_file_info file);

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
extern SYSTEMCALL short sys_fsys_findfirst(const char * path, const char * pattern, p_file_info file);

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
extern SYSTEMCALL short sys_fsys_findnext(short dir, p_file_info file);

/*
 * Get the label for the drive holding the path
 *
 * Inputs:
 * path = path to the drive
 * label = buffer that will hold the label... should be at least 35 bytes
 */
extern SYSTEMCALL short sys_fsys_get_label(const char * path, char * label);

/*
 * Set the label for the drive holding the path
 *
 * Inputs:
 * drive = drive number
 * label = buffer that holds the label
 */
extern SYSTEMCALL short sys_fsys_set_label(short drive, const char * label);

/**
 * Create a directory
 *
 * Inputs:
 * path = the path of the directory to create.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern SYSTEMCALL short sys_fsys_mkdir(const char * path);

/**
 * Delete a file or directory
 *
 * Inputs:
 * path = the path of the file or directory to delete.
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern SYSTEMCALL short sys_fsys_delete(const char * path);

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
extern SYSTEMCALL short sys_fsys_rename(const char * old_path, const char * new_path);

/**
 * Change the current working directory (and drive)
 *
 * Inputs:
 * path = the path that should be the new current
 *
 * Returns:
 * 0 on success, negative number on failure.
 */
extern SYSTEMCALL short sys_fsys_set_cwd(const char * path);

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
extern SYSTEMCALL short sys_fsys_get_cwd(char * path, short size);

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
extern SYSTEMCALL short sys_fsys_load(const char * path, long destination, long * start);

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
extern SYSTEMCALL short sys_fsys_register_loader(const char * extension, p_file_loader loader);

/**
 * Check to see if the file is present.
 * If it is not, return a file not found error.
 * If it is, populate the file info record
 *
 * @param path the path to the file to check
 * @param file pointer to a file info record to fill in, if the file is found.
 * @return 0 on success, negative number on error
 */
extern SYSTEMCALL short sys_fsys_stat(const char * path, p_file_info file);

/**
 * Memory
 */

/**
 * Return the top of system RAM... the user program must not use any
 * system memory from this address and above.
 *
 * @return the address of the first byte of reserved system RAM (one above the last byte the user program can use)
 */
extern SYSTEMCALL unsigned long sys_mem_get_ramtop();

/**
 * Reserve a block of memory at the top of system RAM.
 *
 * @param bytes the number of bytes to reserve
 * @return address of the first byte of the reserved block
 */
extern SYSTEMCALL unsigned long sys_mem_reserve(unsigned long bytes);

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
extern SYSTEMCALL long sys_time_jiffies();

/*
 * Set the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record containing the correct time
 */
extern SYSTEMCALL void sys_rtc_set_time(p_time time);

/*
 * Get the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record in which to put the current time
 */
extern SYSTEMCALL void sys_rtc_get_time(p_time time);

/*
 * Return the next scan code from the keyboard... 0 if nothing pending
 */
extern SYSTEMCALL unsigned short sys_kbd_scancode();

/*
 * Return an error message given an error number
 */
extern SYSTEMCALL const char * sys_err_message(short err_number);

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
extern SYSTEMCALL short sys_kbd_layout(const char * tables);

/**
 * Load and execute an executable file
 *
 * @param path the path to the executable file
 * @param argc the number of arguments passed
 * @param argv the array of string arguments
 * @return the return result of the program
 */
extern SYSTEMCALL short sys_proc_run(const char * path, int argc, char * argv[]);

/**
 * Set the value of a variable
 *
 * @param name the name of the variable to set
 * @param value the value the variable should have
 * @return 0 on success, negative number on error
 */
extern SYSTEMCALL short sys_var_set(const char *name, const char *value);

/**
 * Get the value of a variable
 *
 * @param name the name of the variable to set
 * @return pointer to the string on success, 0 if not found
 */
extern SYSTEMCALL const char * sys_var_get(const char *name);

//
// Text screen calls
//

/**
 * Gets the description of a screen's capabilities
 *
 * @param screen the number of the text device
 *
 * @return a pointer to the read-only description (0 on error)
 */
extern SYSTEMCALL const p_txt_capabilities sys_txt_get_capabilities(short screen);

/**
 * Set the display mode for the screen
 *
 * @param screen the number of the text device
 * @param mode a bitfield of desired display mode options
 *
 * @return 0 on success, any other number means the mode is invalid for the screen
 */
extern SYSTEMCALL short sys_txt_set_mode(short screen, short mode);

/**
 * Set the position of the cursor to (x, y) relative to the current region
 * If the (x, y) coordinate is outside the region, it will be clipped to the region.
 * If y is greater than the height of the region, the region will scroll until that relative
 * position would be within view.
 *
 * @param screen the number of the text device
 * @param x the column for the cursor
 * @param y the row for the cursor
 */
extern SYSTEMCALL void sys_txt_set_xy(short screen, short x, short y);

/**
 * Get the position of the cursor (x, y) relative to the current region
 *
 * @param screen the number of the text device
 * @param position pointer to a t_point record to fill out
 */
extern SYSTEMCALL void sys_txt_get_xy(short screen, p_point position);

/**
 * Get the current region.
 *
 * @param screen the number of the text device
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
extern SYSTEMCALL short sys_txt_get_region(short screen, p_rect region);

/**
 * Set a region to restrict further character display, scrolling, etc.
 * Note that a region of zero size will reset the region to the full size of the screen.
 *
 * @param screen the number of the text device
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
extern SYSTEMCALL short sys_txt_set_region(short screen, p_rect region);

/**
 * Set the default foreground and background colors for printing
 *
 * @param screen the number of the text device
 * @param foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param background the Text LUT index of the new current background color (0 - 15)
 */
extern SYSTEMCALL void sys_txt_set_color(short screen, unsigned char foreground, unsigned char background);

/*
 * Get the foreground and background color for printing
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * foreground = pointer to the foreground color number
 * background = pointer to the background color number
 */
extern SYSTEMCALL void sys_txt_get_color(short screen, unsigned char * foreground, unsigned char * background);

/**
 * Set if the cursor is visible or not
 *
 * @param screen the screen number 0 for channel A, 1 for channel B
 * @param is_visible TRUE if the cursor should be visible, FALSE (0) otherwise
 */
extern SYSTEMCALL void sys_txt_set_cursor_visible(short screen, short is_visible);

/**
 * Load a font as the current font for the screen
 *
 * @param screen the number of the text device
 * @param width width of a character in pixels
 * @param height of a character in pixels
 * @param data pointer to the raw font data to be loaded
 */
extern SYSTEMCALL short sys_txt_set_font(short screen, short width, short height, unsigned char * data);

/**
 * Get the display resolutions
 *
 * @param screen the screen number 0 for channel A, 1 for channel B
 * @param text_size the size of the screen in visible characters (may be null)
 * @param pixel_size the size of the screen in pixels (may be null)
 */
extern SYSTEMCALL void sys_txt_get_sizes(short screen, p_extent text_size, p_extent pixel_size);

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param screen the number of the text device
 * @param width the horizontal size of one side of the border (0 - 32 pixels)
 * @param height the vertical size of one side of the border (0 - 32 pixels)
 */
extern SYSTEMCALL void sys_txt_set_border(short screen, short width, short height);

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param screen the number of the text device
 * @param red the red component of the color (0 - 255)
 * @param green the green component of the color (0 - 255)
 * @param blue the blue component of the color (0 - 255)
 */
extern SYSTEMCALL void sys_txt_set_border_color(short screen, unsigned char red, unsigned char green, unsigned char blue);

/**
 * Print a character to the current cursor position in the current color
 *
 * Most character codes will result in a glyph being displayed at the current
 * cursor position, advancing the cursor one spot. There are some exceptions that
 * will be treated as control codes:
 *
 * 0x08 - BS - Move the cursor back one position, erasing the character underneath
 * 0x09 - HT - Move forward to the next TAB stop
 * 0x0A - LF - Move the cursor down one line (line feed)
 * 0x0D - CR - Move the cursor to column 0 (carriage return)
 *
 * @param screen the number of the text device
 * @param c the character to print
 */
extern SYSTEMCALL void sys_txt_put(short screen, char c);

/**
 * Print an ASCII Z string to the screen
 *
 * @param screen the number of the text device
 * @param c the ASCII Z string to print
 */
extern SYSTEMCALL void sys_txt_print(short screen, const char * message);

#endif
