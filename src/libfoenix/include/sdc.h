/**
 * Definitions support low level SDC device driver
 */

#ifndef __SDC_H
#define __SDC_H

#include <stdint.h>
#include "types.h"

//
// Definitions for GABE's internal SD card controller
//

#define SDC_SECTOR_SIZE         512         // Size of a block on the SDC

#define SDC_STAT_NOINIT         0x01        // SD has not been initialized
#define SDC_STAT_PRESENT        0x02        // SD is present
#define SDC_STAT_PROTECTED      0x04        // SD is write-protected


//
// Initialize the SDC
//
// Returns:
//  0 on success, any negative number is an error code
//
int16_t sdc_init();

//
// Attempt to reset the SD controller
//
void sdc_reset();

//
// Read a block from the SDC
//
// Inputs:
//  lba = the logical block address of the block to read
//  buffer = the buffer into which to copy the block data
//  size = the size of the buffer.
//
// Returns:
//  number of bytes read, any negative number is an error code
//
int16_t sdc_read(int32_t lba, uint8_t * buffer, int16_t size);

//
// Write a block to the SDC
//
// Inputs:
//  lba = the logical block address of the block to write
//  buffer = the buffer containing the data to write
//  size = the size of the buffer.
//
// Returns:
//  number of bytes written, any negative number is an error code
//
int16_t sdc_write(int32_t lba, const uint8_t * buffer, int16_t size);

//
// Return the status of the SDC
//
// Returns:
//  the status of the device
//
int16_t sdc_status();

//
// Return any error code of the SDC
//
// Returns:
//  the error code of the device
//
int16_t sdc_error();

//
// Ensure that any pending writes to teh device have been completed
//
// Returns:
//  0 on success, any negative number is an error code
//
int16_t sdc_flush();

int16_t sdc_sector_count();

#endif
