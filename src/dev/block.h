/**
 * Definitions support low level block device drivers
 */

#ifndef __BLOCK_H
#define __BLOCK_H

#include "errors.h"
#include "constants.h"
#include "sys_macros.h"
#include "sys_types.h"

//
// Preset block device numbers
//

#define BDEV_DEVICES_MAX 8

//
// Initialize the block driver system
//
extern void bdev_init_system();

//
// Register a block device driver
//
extern SYSTEMCALL short bdev_register(p_dev_block device);

//
// Initialize the device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short bdev_init(short dev);

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
extern SYSTEMCALL short bdev_read(short dev, long lba, unsigned char * buffer, short size);

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
extern SYSTEMCALL short bdev_write(short dev, long lba, const unsigned char * buffer, short size);

//
// Return the status of the block device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  the status of the device
//
extern SYSTEMCALL short bdev_status(short dev);

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
extern SYSTEMCALL short bdev_flush(short dev);

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
extern SYSTEMCALL short bdev_ioctrl(short dev, short command, unsigned char * buffer, short size);

#endif
