/**
 * Definitions support low level block device drivers
 */

#ifndef __BLOCK_H
#define __BLOCK_H

#include "types.h"
#include "errors.h"

//
// Preset block device numbers
//

#define BDEV_DEVICES_MAX 8

#define BDEV_SDC 0
#define BDEV_FDC 1
#define BDEV_HDC 2

//
// Structure defining a block device's functions
//

typedef struct s_dev_block {
    short number;           // The number of the device (assigned by registration)
    char * name;            // The name of the device
    FUNC_V_2_S init;        // short init() -- Initialize the device
    FUNC_LBS_2_S read;      // short read(long lba, byte * buffer, short size) -- Read a block from the device
    FUNC_LcBS_2_S write;    // short write(long lba, byte * buffer, short size) -- Write a block to the device
    FUNC_V_2_S status;      // short status() -- Get the status of the device
    FUNC_V_2_S flush;       // short flush() -- Ensure that any pending writes to teh device have been completed
    FUNC_SBS_2_S ioctrl;    // short ioctrl(short command, byte * buffer, short size)) -- Issue a control command to the device
} t_dev_block, *p_dev_block;

//
// Initialize the block driver system
//
extern void bdev_init_system();

//
// Register a block device driver
//
extern short bdev_register(p_dev_block device);

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
extern short bdev_read(short dev, long lba, unsigned char * buffer, short size);

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
extern short bdev_write(short dev, long lba, const unsigned char * buffer, short size);

//
// Return the status of the block device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  the status of the device
//
extern short bdev_status(short dev);

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short bdev_flush(short dev);

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
extern short bdev_ioctrl(short dev, short command, unsigned char * buffer, short size);

#endif
