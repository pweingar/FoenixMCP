/**
 * Definitions support low level FDC device driver
 */

#ifndef __FDC_H
#define __FDC_H

#include "sys_types.h"

/*
 * Definitions for the FDC controller
 */

#define FDC_SECTOR_SIZE         512         /* Size of a block on the FDC */
#define FDC_MAX_PARAMS          10          /* Maximum number of parameters/result bytes in a transaction */
#define FDC_DEFAULT_RETRIES     3           /* Default number of times we'll retry a transaction */

#define FDC_STAT_NOINIT         0x01        /* FDC has not been initialized */
#define FDC_STAT_PRESENT        0x02        /* FD is present */
#define FDC_STAT_PROTECTED      0x04        /* FD is write-protected */
#define FDC_STAT_MOTOR_ON       0x08        /* FDC spindle motor is on */

#define FDC_CTRL_MOTOR_ON       0x0001      /* IOCTRL command to start spinning the motor */
#define FDC_CTRL_MOTOR_OFF      0x0002      /* IOCTRL command to start spinning the motor */
#define FDC_CTRL_CHECK_CHANGE   0x0003      /* IOCTRL command to check to see if the disk has changed */

/*
 * Structure to keep track of the information about a transaction with the floppy drive
 */
typedef struct s_fdc_trans {
    short retries;                              /* The number of retries that may be attempted on the transaction */
    unsigned char command;                      /* The command code for the transaction */
    unsigned char parameters[FDC_MAX_PARAMS];   /* The parameters to send as part of the transaction */
    short parameter_count;                      /* The number of parameters to send as part of the command */
    unsigned char results[FDC_MAX_PARAMS];      /* The parameters to send as part of the transaction */
    short result_count;                         /* The number of parameters to send as part of the command */
    unsigned char *data;                        /* Pointer to the data buffer to read or write to the FDC */
    short data_count;                           /* Number of data bytes to transfer */
    short direction;                            /* 0 = no data, 1 = data is written to the FDC, 2 = data is read from the FDC */
} t_fdc_trans, *p_fdc_trans;


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

/**
 * Set whether to use DMA or polled I/O for exchanging data
 *
 * @param dma 0 for polled I/O, anything else for DMA
 */
extern void fdc_set_dma(short dma);

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

/*
 * Move the read/write head to the indicated cylinder
 */
extern short fdc_seek(unsigned char cylinder);

/*
 * Recalibrate the read/write head
 */
extern short fdc_recalibrate();

#endif
