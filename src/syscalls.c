#include "syscalls.h"
#include "interrupt.h"
#include "dev/channel.h"
#include "dev/block.h"
#include "dev/rtc.h"

/***
 *** Core system calls
 ***/

/*
 * Stop running the current user program
 *
 * Inputs:
 * result = the result code to return to the kernel
 */
void sys_exit(short result) {
    syscall(KFN_EXIT, result);
}

/*
 * Enable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 */
void sys_int_enable_all() {
    syscall(KFN_INT_ENABLE_ALL);
}

/*
 * Disable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 */
void sys_int_disable_all() {
    syscall(KFN_INT_DISABLE_ALL);
}

/*
 * Disable an interrupt by masking it
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
void sys_int_disable(unsigned short n) {
    syscall(KFN_INT_DISABLE, n);
}

/*
 * Enable an interrupt
 *
 * Inputs:
 * n = the number of the interrupt
 */
void sys_int_enable(unsigned short n) {
    syscall(KFN_INT_ENABLE, n);
}

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
void sys_get_info(p_sys_info info) {
    syscall(KFN_SYS_GET_INFO, info);
}

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
p_int_handler sys_int_register(unsigned short n, p_int_handler handler) {
    return (p_int_handler)syscall(KFN_INT_REGISTER, n, handler);
}

/*
 * Return true (non-zero) if an interrupt is pending for the given interrupt
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 *
 * Returns:
 * non-zero if interrupt n is pending, 0 if not
 */
short sys_int_pending(unsigned short n) {
    return syscall(KFN_INT_PENDING, n);
}

/*
 * Acknowledge an interrupt (clear out its pending flag)
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
void sys_int_clear(unsigned short n) {
    syscall(KFN_INT_CLEAR, n);
}

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
short sys_chan_read_b(short channel) {
    return syscall(KFN_CHAN_READ_B, channel);
}

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
short sys_chan_read(short channel, unsigned char * buffer, short size) {
    return syscall(KFN_CHAN_READ, channel, buffer, size);
}

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
short sys_chan_readline(short channel, unsigned char * buffer, short size) {
    return syscall(KFN_CHAN_READ_LINE, channel, buffer, size);
}


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
short sys_chan_write_b(short channel, unsigned char b) {
    return syscall(KFN_CHAN_WRITE_B, channel);
}

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
short sys_chan_write(short channel, unsigned char * buffer, short size) {
    return syscall(KFN_CHAN_WRITE, channel, buffer, size);
}

/*
 * Return the status of the channel device
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  the status of the device
 */
short sys_chan_status(short channel) {
    return syscall(KFN_CHAN_STATUS, channel);
}

/*
 * Ensure that any pending writes to teh device have been completed
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
short sys_chan_flush(short channel) {
    return syscall(KFN_CHAN_FLUSH, channel);
}

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
short sys_chan_seek(short channel, long position, short base) {
    return syscall(KFN_CHAN_SEEK, channel, position, base);
}

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
short sys_chan_ioctrl(short channel, short command, uint8_t * buffer, short size) {
    return syscall(KFN_CHAN_IOCTRL, channel, command, buffer, size);
}

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
short sys_chan_open(short dev, uint8_t * path, short mode) {
    return syscall(KFN_CHAN_OPEN, path, mode);
}

/*
 * Close a channel
 *
 * Inputs:
 * chan = the number of the channel to close
 *
 * Returns:
 * nothing useful
 */
short sys_chan_close(short chan) {
    return syscall(KFN_CHAN_CLOSE, chan);
}

/***
 *** Block device system calls
 ***/

//
// Register a block device driver
//
short sys_bdev_register(p_dev_block device) {
    return syscall(KFN_BDEV_REGISTER, device);
}

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
short sys_bdev_read(short dev, long lba, unsigned char * buffer, short size) {
    return syscall(KFN_BDEV_GETBLOCK, dev, lba, buffer, size);
}

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
short sys_bdev_write(short dev, long lba, const unsigned char * buffer, short size) {
    return syscall(KFN_BDEV_PUTBLOCK, dev, lba, buffer, size);
}

//
// Return the status of the block device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  the status of the device
//
short sys_bdev_status(short dev) {
    return syscall(KFN_BDEV_STATUS, dev);
}

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
short sys_bdev_flush(short dev) {
    return syscall(KFN_BDEV_FLUSH, dev);
}

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
short sys_bdev_ioctrl(short dev, short command, unsigned char * buffer, short size) {
    return syscall(KFN_BDEV_IOCTRL, dev, command, buffer, size);
}

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
short sys_fsys_open(const char * path, short mode) {
    return (short)syscall(KFN_OPEN, path, mode);
}

/**
 * Close access to a previously open file.
 *
 * Inputs:
 * fd = the channel ID for the file
 *
 * Returns:
 * 0 on success, negative number on failure
 */
short sys_fsys_close(short fd) {
    return (short)syscall(KFN_CLOSE, fd);
}

/**
 * Attempt to open a directory for scanning
 *
 * Inputs:
 * path = the path to the directory to open
 *
 * Returns:
 * the handle to the directory if >= 0. An error if < 0
 */
short sys_fsys_opendir(const char * path) {
    return (short)syscall(KFN_OPENDIR, path);
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
short sys_fsys_closedir(short dir) {
    return (short)syscall(KFN_CLOSEDIR, dir);
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
short sys_fsys_readdir(short dir, p_file_info file) {
    return (short)syscall(KFN_READDIR, dir, file);
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
short sys_fsys_findfirst(const char * path, const char * pattern, p_file_info file) {
    return (short)syscall(KFN_FINDFIRST, path, pattern, file);
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
short sys_fsys_findnext(short dir, p_file_info file) {
    return (short)syscall(KFN_FINDNEXT, dir, file);
}

/*
 * Get the label for the drive holding the path
 *
 * Inputs:
 * path = path to the drive
 * label = buffer that will hold the label... should be at least 35 bytes
 */
short sys_fsys_get_label(char * path, char * label) {
    return (short)syscall(KFN_GET_LABEL, path, label);
}

/*
 * Set the label for the drive holding the path
 *
 * Inputs:
 * drive = drive number
 * label = buffer that holds the label
 */
short sys_fsys_set_label(short drive, const char * label) {
    return (short)syscall(KFN_SET_LABEL, drive, label);
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
short sys_fsys_mkdir(const char * path) {
    return (short)syscall(KFN_MKDIR, path);
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
short sys_fsys_delete(const char * path) {
    return (short)syscall(KFN_DELETE, path);
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
short sys_fsys_rename(const char * old_path, const char * new_path) {
    return (short)syscall(KFN_RENAME, old_path, new_path);
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
short sys_fsys_set_cwd(const char * path) {
    return (short)syscall(KFN_SET_CWD, path);
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
short sys_fsys_get_cwd(char * path, short size) {
    return (short)syscall(KFN_GET_CWD, path, size);
}

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
short sys_fsys_load(const char * path, long destination, long * start) {
    return (short)syscall(KFN_LOAD, path, destination, start);
}

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
short sys_fsys_register_loader(const char * extension, p_file_loader loader) {
    return (short)syscall(KFN_LOAD_REGISTER, extension, loader);
}

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
extern long sys_time_jiffies() {
    return syscall(KFN_TIME_JIFFIES);
}

/*
 * Set the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record containing the correct time
 */
void sys_rtc_set_time(p_time time) {
    syscall(KFN_TIME_SETRTC, time);
}

/*
 * Get the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record in which to put the current time
 */
void sys_rtc_get_time(p_time time) {
    syscall(KFN_TIME_GETRTC, time);
}

/*
 * Return the next scan code from the keyboard... 0 if nothing pending
 */
unsigned short sys_kbd_scancode() {
    return syscall(KFN_KBD_SCANCODE);
}

/*
 * Return an error message given an error number
 */
const char * sys_err_message(short err_number) {
    return (const char *)syscall(KFN_ERR_MESSAGE, (short)err_number);
}

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
short sys_kbd_layout(const char * tables) {
    return syscall(KFN_KBD_LAYOUT, tables);
}
