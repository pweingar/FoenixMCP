#include "syscalls.h"

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
