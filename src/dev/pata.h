/**
 * Low level driver for the PATA hard drive.
 */

#ifndef __PATA_H
#define __PATA_H

#include "types.h"

#define PATA_GET_SECTOR_COUNT   1
#define PATA_GET_SECTOR_SIZE    2
#define PATA_GET_BLOCK_SIZE     3
#define PATA_GET_DRIVE_INFO     4

#define PATA_SECTOR_SIZE        512         // Size of a block on the PATA

#define PATA_STAT_NOINIT        0x01        // PATA hard drive has not been initialized
#define PATA_STAT_PRESENT       0x02        // PATA hard drive is present

//
// Structures
//

typedef struct s_drive_info {
    unsigned short flags;
    char serial_number[18];
    char firmware_version[6];
    char model_name[38];
    unsigned short lba_enabled;
    union u1 {
        struct s1 {
            unsigned short lba_default_lo;
            unsigned short lba_default_hi;
        } lbaw;
        unsigned long lba_default;
    } l;
} t_drive_info, *p_drive_info;

//
// Install the PATA driver
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short pata_install();

//
// Initialize the PATA hard drive
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short pata_init();

//
// Read a block from the PATA hard drive
//
// Inputs:
//  lba = the logical block address of the block to read
//  buffer = the buffer into which to copy the block data
//  size = the size of the buffer.
//
// Returns:
//  number of chars read, any negative number is an error code
//
extern short pata_read(long lba, char * buffer, short size);

//
// Write a block to the PATA hard drive
//
// Inputs:
//  lba = the logical block address of the block to write
//  buffer = the buffer containing the data to write
//  size = the size of the buffer.
//
// Returns:
//  number of chars written, any negative number is an error code
//
extern short pata_write(long lba, const char * buffer, short size);

//
// Return the status of the PATA hard drive
//
// Returns:
//  the status of the device
//
extern short pata_status();

//
// Return any error code of the PATA hard drive
//
// Returns:
//  the error code of the device
//
extern short pata_error();

//
// Ensure that any pending writes to teh device have been completed
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short pata_flush();

//
// Issue a control command to the PATA hard drive
//
// Inputs:
//  command = the number of the command to send
//  buffer = pointer to chars of additional data for the command
//  size = the size of the buffer
//
// Returns:
//  0 on success, any negative number is an error code
//
extern short pata_ioctrl(short command, char * buffer, short size);

#endif
