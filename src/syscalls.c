#include "syscalls.h"
#include "interrupt.h"
#include "dev/channel.h"
#include "dev/block.h"

/***
 *** Core system calls
 ***/

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
