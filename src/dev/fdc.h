/**
 * Definitions support low level FDC device driver
 */

#ifndef __FDC_H
#define __FDC_H

#if MODEL == MODEL_FOENIX_A2560K

#include "types.h"

/*
 * Definitions for the FDC controller
 */

#define FDC_SECTOR_SIZE         512         /* Size of a block on the FDC */

#define FDC_STAT_NOINIT         0x01        /* FDC has not been initialized */
#define FDC_STAT_PRESENT        0x02        /* FD is present */
#define FDC_STAT_PROTECTED      0x04        /* FD is write-protected */
#define FDC_STAT_MOTOR_ON       0x08        /* FDC spindle motor is on */

#define FDC_CTRL_MOTOR_ON       0x0100      /* IOCTRL command to start spinning the motor */
#define FDC_CTRL_MOTOR_OFF      0x0200      /* IOCTRL command to start spinning the motor */

/*
 * Install the FDC driver
 */
extern short fdc_install();

/*
 * Initialize the FDC
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short fdc_init();

/*
 * Read a block from the FDC
 *
 * Inputs:
 *  lba = the logical block address of the block to read
 *  buffer = the buffer into which to copy the block data
 *  size = the size of the buffer.
 *
 * Returns:
 *  number of bytes read, any negative number is an error code
 */
extern short fdc_read(long lba, unsigned char * buffer, short size);

/*
 * Write a block to the FDC
 *
 * Inputs:
 *  lba = the logical block address of the block to write
 *  buffer = the buffer containing the data to write
 *  size = the size of the buffer.
 *
 * Returns:
 *  number of bytes written, any negative number is an error code
 */
extern short fdc_write(long lba, const unsigned char * buffer, short size);

/*
 * Return the status of the FDC
 *
 * Returns:
 *  the status of the device
 */
extern short fdc_status();

/*
 * Return any error code of the FDC
 *
 * Returns:
 *  the error code of the device
 */
extern short fdc_error();

/*
 * Ensure that any pending writes to teh device have been completed
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short fdc_flush();

/*
 * Issue a control command to the device
 *
 * Inputs:
 *  command = the number of the command to send
 *  buffer = pointer to bytes of additional data for the command
 *  size = the size of the buffer
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short fdc_ioctrl(short command, unsigned char * buffer, short size);

#endif

#endif
