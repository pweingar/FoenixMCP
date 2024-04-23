/**
 * Low level driver for the PATA hard drive.
 */

#ifndef __PATA_H
#define __PATA_H

#include <stdint.h>
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
    uint16_t flags;
    char serial_number[18];
    char firmware_version[6];
    char model_name[38];
    uint16_t lba_enabled;
    union u1 {
        struct s1 {
            uint16_t lba_default_lo;
            uint16_t lba_default_hi;
        } lbaw;
        uint32_t lba_default;
    } l;
} t_drive_info, *p_drive_info;


//
// Initialize the PATA hard drive
//
// Returns:
//  0 on success, any negative number is an error code
//
 int16_t pata_init();

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
 int16_t pata_read(int32_t lba, uint8_t * buffer, int16_t size);

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
 int16_t pata_write(int32_t lba, const uint8_t * buffer, int16_t size);

//
// Return the status of the PATA hard drive
//
// Returns:
//  the status of the device
//
 int16_t pata_status();

//
// Return any error code of the PATA hard drive
//
// Returns:
//  the error code of the device
//
 int16_t pata_error();

//
// Ensure that any pending writes to the device have been completed
//
// Returns:
//  0 on success, any negative number is an error code
//
 int16_t pata_flush();

//
// Return info about the connected drive (we only support one)
int16_t pata_identity(p_drive_info drive_info);

#endif
