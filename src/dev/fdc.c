/**
 * Definitions support low level FDC device driver
 */

#include "features.h"
#if HAS_FLOPPY

#include <stdio.h>
#include <string.h>
#include "indicators.h"
#include "dev/block.h"
#include "dev/fdc.h"
#include "log.h"
#include "types.h"
#include "timers.h"
#include "fdc.h"
#include "fdc_reg.h"
#include "interrupt.h"
#include "rtc_reg.h"
#include "syscalls.h"

/*
 * Constants
 */

const long fdc_motor_wait = 18;         /* The number of jiffies to wait for the motor to spin up: 300ms */
const long fdc_motor_timeout = 300;     /* The number of jiffies to let the motor spin without activity: 30 seconds */
const long fdc_seek_timeout = 180;      /* 3s timeout for the head to seek */
const long fdc_timeout = 120;           /* The number of jiffies to allow for basic wait loops */

/*
 * Types
 */

enum fdc_command_code {
    FDC_CMD_READ_TRACK = 2,
    FDC_CMD_SPECIFY = 3,
    FDC_CMD_SENSE_DRIVE_STATUS = 4,
    FDC_CMD_WRITE_DATA = 5,
    FDC_CMD_READ_DATA = 6,
    FDC_CMD_RECALIBRATE = 7,
    FDC_CMD_SENSE_INTERRUPT = 8,
    FDC_CMD_WRITE_DELETED_DATA = 9,
    FDC_CMD_READ_ID = 10,
    FDC_CMD_READ_DELETED_TRACK = 12,
    FDC_CMD_FORMAT_TRACK = 13,
    FDC_CMD_DUMPREG = 14,
    FDC_CMD_SEEK = 15,
    FDC_CMD_VERSION = 16,
    FDC_CMD_SCAN_EQUAL = 17,
    FDC_CMD_CONFIGURE = 19
};

/*
 * Enumeration of direction codes for an FDC command transaction
 */
enum fdc_trans_direction {
    FDC_TRANS_NONE = 0,                     /* No data is transferred */
    FDC_TRANS_WRITE,                        /* We are writing to the drive */
    FDC_TRANS_READ                          /* We are reading from the drive */
};

/*
 * Device variables
 */

short g_fdc_stat = FDC_STAT_NOINIT;
static long fdc_motor_off_time = 0;         /* The time (in jiffies) when the motor should turn off */
static short fdc_heads_per_cylinder = 2;    /* How many heads are supported? */
static short fdc_sectors_per_track = 18;    /* How many sectors per track */
static short fdc_cylinders = 80;            /* How many cylinders */
static short fdc_bytes_per_sector = 512;    /* How many bytes are in a sector */
static short fdc_use_dma = 0;               /* If 0: used polled I/O, if anything else, use DMA */

/**
 * Check the current jiffy count and turn off the motor if we've reached the time the motor should be turned off
 * This time gets reset every time we ask for the motor to be turned on
 */
extern void fdc_motor_watchdog();

/*
 * Convert a logical block address to cylinder-head-sector addressing
 *
 * Inputs:
 * lba = the logical block address
 * cylinder = pointer to the integer in which to store the cylinder number
 * head = pointer to the integer in which to store the head number
 * sector = pointer to the integer in which to store the sector number
 */
void lba_2_chs(unsigned long lba, unsigned char * cylinder, unsigned char * head, unsigned char * sector) {
    *cylinder = (unsigned char)(lba / (fdc_heads_per_cylinder * fdc_sectors_per_track));
    *head = (unsigned char)((lba / fdc_sectors_per_track) % fdc_heads_per_cylinder);
    *sector = (unsigned char)((lba % fdc_sectors_per_track) + 1);
}

/**
 * Set whether to use DMA or polled I/O for exchanging data
 *
 * @param dma 0 for polled I/O, anything else for DMA
 */
void fdc_set_dma(short dma) {
    fdc_use_dma = dma;
}

/**
 * Flag that the disk has changed
 */
void fdc_media_change() {
    // Indicate that the disk has changed
    g_fdc_stat = FDC_STAT_NOINIT;
}

/**
 * Check to see if the media has changed
 * If so, recalibrate the drive and flag the change
 */
bool fdc_media_check_change() {
    // Check for the a disk change
    if (*FDC_DIR & 0x80) {
        // The disk has changed... recalibrate and set that it's changed
        fdc_recalibrate();
        fdc_seek(1);
        g_fdc_stat = FDC_STAT_NOINIT;
        return true;
    }
    return false;
}

/*
 * Wait for the FDC to ready
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_wait_rqm() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_rqm");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_RQM) != FDC_MSR_RQM) && (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_RQM) == FDC_MSR_RQM) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        log_num(LOG_ERROR, "fdc_wait_rqm: MSR = ", msr);
        return DEV_TIMEOUT;
    }
}

/*
 * Wait for the FDC to not be busy working on a command
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_wait_while_busy() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_while_busy");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_CMDBSY) == FDC_MSR_CMDBSY) &&
             (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_CMDBSY) == 0) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        log_num(LOG_ERROR, "fdc_wait_while_busy: MSR = ", msr);
        return DEV_TIMEOUT;
    }
}


/*
 * Wait for the FDC to be ready for the CPU to send data
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_wait_write() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_write");

    do {
        msr = *FDC_MSR;
    } while (((msr & (FDC_MSR_RQM | FDC_MSR_DIO)) != (FDC_MSR_RQM)) &&
             (target_time > timers_jiffies()));

    if ((msr & (FDC_MSR_RQM | FDC_MSR_DIO)) == (FDC_MSR_RQM)) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        log_num(LOG_ERROR, "fdc_wait_write: MSR = ", msr);
        return DEV_TIMEOUT;
    }
}

/*
 * Wait for the FDC to be ready for the CPU to read data
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_wait_read() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_read");

    do {
        msr = *FDC_MSR;
    } while (((msr & (FDC_MSR_RQM | FDC_MSR_DIO)) != (FDC_MSR_RQM | FDC_MSR_DIO)) &&
             (target_time > timers_jiffies()));

    if ((msr & (FDC_MSR_RQM | FDC_MSR_DIO)) == (FDC_MSR_RQM | FDC_MSR_DIO)) {
        /* Return success */
        return 0;
    } else {
        log_num(LOG_ERROR, "fdc_wait_read: MSR = ", msr);
        /* Return that we hit a timeout */
        return DEV_TIMEOUT;
    }
}

void fdc_delay(int jiffies) {
    unsigned long target_jiffies = timers_jiffies() + jiffies;
    while (target_jiffies > timers_jiffies()) ;
}

/*
 * Get a byte from the floppy drive controller FIFO
 *
 * Inputs:
 * ptr = pointer to the byte position to fill
 *
 * Returns:
 * 0 on success, negative number is an error
 */
short fdc_in(unsigned char *ptr) {
    long target_ticks = timers_jiffies() + fdc_timeout;
    while ((*FDC_MSR & FDC_MSR_RQM) != FDC_MSR_RQM) {
        if (timers_jiffies() >= target_ticks) {
            logmsg(LOG_ERROR, "fdc_in: timeout waiting for RQM");
            return DEV_TIMEOUT;
        }
    }
    *ptr = *FDC_DATA;
}

/*
 * Put a byte to the floppy drive controller FIFO
 *
 * Inputs:
 * x = the value to put
 *
 * Returns:
 * 0 on success, negative number is an error
 */
short fdc_out(unsigned char x) {
    long target_ticks = timers_jiffies() + fdc_timeout;
    while ((*FDC_MSR & FDC_MSR_RQM) != FDC_MSR_RQM) {
        if (timers_jiffies() >= target_ticks) {
            logmsg(LOG_ERROR, "fdc_out: timeout waiting for RQM");
            return DEV_TIMEOUT;
        }
    }
    *FDC_DATA = x;
    return 0;
}

/*
 * Spin up the drive's spindle motor
 */
short fdc_motor_on() {
    TRACE("fdc_motor_on");

    log_num(LOG_TRACE, "FDC_DOR: ", *FDC_DOR);

    if ((*FDC_DOR & FDC_DOR_MOT0) != FDC_DOR_MOT0) {
        /* Motor is not on... turn it on without DMA or RESET */
        *FDC_DOR = FDC_DOR_MOT0 | FDC_DOR_NRESET;

        log_num(LOG_TRACE, "FDC_DOR 2: ", *FDC_DOR);

        if (fdc_wait_rqm()) {
            /* Got a timeout after trying to turn on the motor */
            logmsg(LOG_ERROR, "fdc_motor_on timeout");
            return DEV_TIMEOUT;
        }

        /* Wait a decent time for the motor to spin up */
        long wait_time = timers_jiffies() + fdc_motor_wait;
        while (wait_time > timers_jiffies()) ;
    }

    short needs_handler = 0;
    if (fdc_motor_off_time == 0) {
        needs_handler = 1;
    }

    /* Set a new target time to shut off the motor */
    fdc_motor_off_time = timers_jiffies() + fdc_motor_timeout;

    if (needs_handler) {
        // Register the FDC motor watchdog to monitor for motor timeout
        rtc_register_periodic(RTC_RATE_500ms, fdc_motor_watchdog);
    }

    // /* Flag that the motor is on */
    // g_fdc_stat |= FDC_STAT_MOTOR_ON;

    ind_set(IND_FDC, IND_ON);

    return 0;
}

/*
 * Spin down the drive's spindle motor
 */
void fdc_motor_off() {
    TRACE("fdc_motor_off");

    if ((*FDC_DOR & FDC_DOR_MOT0) == FDC_DOR_MOT0) {
        /* Motor is not on... turn it off without DMA or RESET */
        *FDC_DOR = FDC_DOR_NRESET;

        if (fdc_wait_rqm()) {
            /* Got a timeout after trying to turn on the motor */
            return;
        }
    }

    // /* Flag that the motor is off */
    // g_fdc_stat &= ~FDC_STAT_MOTOR_ON;

    // Reset the motor off time to 0, so we know we need to reinstall the watchdog later
    fdc_motor_off_time = 0;

    // Remove the FDC motor watchdog
    rtc_register_periodic(0, 0);

    ind_set(IND_FDC, IND_OFF);
}

/**
 * Check the current jiffy count and turn off the motor if we've reached the time the motor should be turned off
 * This time gets reset every time we ask for the motor to be turned on
 */
void fdc_motor_watchdog() {
    unsigned char flags = *RTC_FLAGS;
    if (timers_jiffies() >= fdc_motor_off_time) {
        fdc_motor_off();
    }
}

/*
 * Issue the SENSE INTERRUPT command
 *
 * Inputs:
 * st0 = pointer to the ST0 location
 * pcn = pointer to the Present Cylinder Number location
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_sense_interrupt_status(unsigned char *st0, unsigned char *pcn) {
    TRACE("fdc_sense_interrupt_status");

    /* Set up "return" values to something neutral */
    *st0 = 0;
    *pcn = 0xff;

    if (fdc_wait_while_busy()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_sense_interrupt_status: fdc_wait_while_busy timeout");
        return DEV_TIMEOUT;
    }

    if (fdc_wait_write()) {
        /* Timed out waiting for permission to write a command byte */
        logmsg(LOG_ERROR, "fdc_sense_interrupt_status: fdc_wait_write timeout");
        return DEV_TIMEOUT;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_SENSE_INTERRUPT;

    if (fdc_wait_rqm()) {
        /* Timed out waiting to receive data */
        logmsg(LOG_ERROR, "fdc_sense_interrupt_status: fdc_wait_rqm timeout 1");
        return DEV_TIMEOUT;
    }

    if (fdc_wait_read()) {
        /* Timed out waiting to receive data */
        logmsg(LOG_ERROR, "fdc_sense_interrupt_status: fdc_wait_read timeout 1");
        return DEV_TIMEOUT;
    }

    /* Read the status byte */
    *st0 = *FDC_DATA;
    log_num(LOG_INFO, "fdc_sense_interrupt_status: ST0=", *st0);

    if (fdc_wait_rqm()) {
        /* Timed out waiting to receive data */
        logmsg(LOG_ERROR, "fdc_sense_interrupt_status: fdc_wait_rqm timeout 1");
        return DEV_TIMEOUT;
    }

    if (fdc_wait_read()) {
        /* Timed out waiting for permission to transfer another byte */
        logmsg(LOG_ERROR, "fdc_sense_interrupt_status: fdc_wait_read timeout 2");
        return DEV_TIMEOUT;
    }

    /* Read the present cylinder number */
    *pcn = *FDC_DATA;

    return 0;
}

/*
 * Issue the SPECIFY command
 *
 * Inputs:
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_specify() {
    TRACE("fdc_specify");

    if (fdc_wait_while_busy()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_specify: fdc_wait_while_busy timeout");
        return DEV_TIMEOUT;
    }

    if (fdc_wait_write() < 0) {
        /* Timed out waiting for permission to write a command byte */
        logmsg(LOG_ERROR, "fdc_specify: fdc_wait_write timeout 1");
        return DEV_TIMEOUT;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_SPECIFY;

    if (fdc_wait_write() < 0) {
        /* Timed out waiting for permission to write a command byte */
        logmsg(LOG_ERROR, "fdc_specify: fdc_wait_write timeout 2");
        return DEV_TIMEOUT;
    }

    /* Send the seek rate time and head unload times */
    *FDC_DATA = 0xC0;

    if (fdc_wait_write() < 0) {
        /* Timed out waiting for permission to write a command byte */
        logmsg(LOG_ERROR, "fdc_specify: fdc_wait_write timeout 3");
        return DEV_TIMEOUT;
    }

    /* Set head load time to maximum, and no DMA */
    unsigned char hlt_ndma = 0;
    if (fdc_use_dma) {
        hlt_ndma = 0x0A;
    } else {
        hlt_ndma = 0x0B;
    }
    log_num(LOG_INFO, "FDC specify: ", hlt_ndma);
    *FDC_DATA = hlt_ndma;

    return 0;
}

/*
 * Issue the CONFIGURE command
 *
 * Inputs:
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_configure() {
    TRACE("fdc_configure");

    if (fdc_wait_while_busy()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_configure: fdc_wait_while_busy timeout");
        return DEV_TIMEOUT;
    }

    if (fdc_wait_write()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_configure: fdc_wait_write timeout 1");
        return DEV_TIMEOUT;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_CONFIGURE;

    if (fdc_wait_write()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_configure: fdc_wait_write timeout 2");
        return DEV_TIMEOUT;
    }

    /* Send a padding byte of 0 */
    *FDC_DATA = 0x00;

    if (fdc_wait_write()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_configure: fdc_wait_write timeout 3");
        return DEV_TIMEOUT;
    }

    /* Implied seek, enable FIFO, enable POLL, FIFO threshold = 16 bytes */
    *FDC_DATA = 0x4F;

    if (fdc_wait_write()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_configure: fdc_wait_write timeout 4");
        return DEV_TIMEOUT;
    }

    /* Send track precompensation = 0 */
    *FDC_DATA = 0x00;

    return 0;
}

/*
 * Issue the VERSION command
 *
 * Inputs:
 * version = pointer to the byte to hold the version
 *
 * Returns:
 * 0 on success, DEV_TIMEOUT on timeout
 */
short fdc_version(unsigned char *version) {
    TRACE("fdc_version");

    if (fdc_wait_while_busy()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_version: fdc_wait_while_busy timeout");
        return DEV_TIMEOUT;
    }

    if (fdc_wait_write()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_version: fdc_wait_write timeout 1");
        return DEV_TIMEOUT;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_VERSION;

    if (fdc_wait_write()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_version: fdc_wait_write timeout 2");
        return DEV_TIMEOUT;
    }

    /* Get the version */
    *version = *FDC_DATA;

    return 0;
}

/*
 * Reset the FDC
 */
short fdc_reset() {
    unsigned char st0;
    unsigned char pcn;
    short i;
    long target_time;

    TRACE("fdc_reset");

    /* Reset the controller */
    *FDC_DOR = 0;
    target_time = timers_jiffies() + 2;
    while (target_time > timers_jiffies());

    unsigned char dor = 0;
    if (fdc_use_dma) {
        dor = FDC_DOR_NRESET | FDC_DOR_DMAEN;
    } else {
        dor = FDC_DOR_NRESET;
    }
    log_num(LOG_INFO, "FDC DOR: ", dor);
    *FDC_DOR = dor;


    /* Default precompensation, data rate for 1.44MB */
    *FDC_DSR = 0x80;

    target_time = timers_jiffies() + 10;
    while (target_time > timers_jiffies()) ;

    /* Set data rate for 1.44 MB */
    *FDC_CCR = 0;

    /* Issue the sense interrupt command 4 times. */
    for (i = 0; i < 4; i++) {
        st0 = 0;
        pcn = 0;
        if (fdc_sense_interrupt_status(&st0, &pcn) < 0) {
            /* Time out on sense interrupt */
            logmsg(LOG_ERROR, "fdc_sense_interrupt_status timeout");
            return DEV_TIMEOUT;
        }

        log_num(LOG_INFO, "fdc_reset ST0: ", st0);
        log_num(LOG_INFO, "fdc_reset PCN: ", pcn);

        if (st0 == 0xC0) {
            break;
        }
    }

    /* Configure the FDC */
    if (fdc_configure() < 0) {
        /* Timeout on sense interrupt */
        logmsg(LOG_ERROR, "fdc_configure timeout");
        return DEV_TIMEOUT;
    }

    /* Specify seek and head times, and turn off DMA */
    if (fdc_specify() < 0) {
        /* Timeout on sense interrupt */
        logmsg(LOG_ERROR, "fdc_specify timeout");
        return DEV_TIMEOUT;
    }

    g_fdc_stat = 0;

    fdc_motor_on();

    return 0;
}

/*
 * Send a human-readable version of the transaction to the log
 */
void fdc_log_transaction(p_fdc_trans trans) {
    char buffer[80];
    short i;

    sprintf(buffer, "Command: %02X\n", trans->command);
    sys_chan_write(0, buffer, strlen(buffer));

    for (i = 0; i < trans->parameter_count; i++) {
        sprintf(buffer, "Parameter %d: %02X\n", i, trans->parameters[i]);
        sys_chan_write(0, buffer, strlen(buffer));
    }
}

/*
 * Issue a command to the floppy drive controller using DMA
 *
 * This routine supports transactions with variable number of parameters and results
 * It can also support commands with an execution phase (read or write) and those
 * without an exectution phase.
 *
 * Inputs:
 * transaction = a pointer to an s_fdc_trans structure, containing the information
 *               needed for the transaction
 *
 * Returns:
 * 0 on success, negative number on error
 */
short fdc_command_dma(p_fdc_trans transaction) {
    volatile unsigned char * fdc_dma_buffer = (unsigned char *)0xFEC02400;
    short abort = 0;
    short i;
    short result = 0;
    unsigned char msr;
    unsigned long target_jiffies;

    TRACE("fdc_command");

    // fdc_log_transaction(transaction);

    if (fdc_wait_while_busy()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_command: fdc_wait_while_busy timeout");
        return DEV_TIMEOUT;
    }

    result = fdc_out(transaction->command);      /* Send the command byte */
    if (result < 0) {
        logmsg(LOG_ERROR, "fdc_command: timeout sending command");
        return result;
    }

    for (i = 0; i < transaction->parameter_count; i++) {
        if ((result = fdc_out(transaction->parameters[i])) < 0) {
            logmsg(LOG_ERROR, "fdc_command: timeout sending parameters");
            return result;
        }
    }

    /* Check to see if there is an execution phase...
     * that is, there is data to transfer one way or the other
     */
    switch (transaction->direction) {
        case FDC_TRANS_WRITE:
            /* We're writing to the FDC */
            break;

        case FDC_TRANS_READ:
            /* Wait for DMA to complete */
            target_jiffies = timers_jiffies() + fdc_timeout;
            while ((*PENDING_GRP1 & SPIO_FDC_INT16) == 0) {
                if (timers_jiffies() < target_jiffies) {
                    return DEV_TIMEOUT;
                }
            }

            /* Copy the data from the DMA buffer */
            for (i = 0; i < 512; i++) {
                transaction->data[i] = fdc_dma_buffer[i];
            }

            break;

        default:
            break;
    }

    /* Result phase: read the result bytes */

    fdc_delay(30);

    for (i = 0; i < transaction->result_count; i++) {
        if ((result = fdc_in(&transaction->results[i])) < 0) {
            logmsg(LOG_ERROR, "fdc_command: timeout getting results");
            return result;
        }
    }

    /* Wait until the FDC is not busy */

    result = fdc_wait_while_busy();

    return result;
}

/*
 * Issue a command to the floppy drive controller
 *
 * This routine supports transactions with variable number of parameters and results
 * It can also support commands with an execution phase (read or write) and those
 * without an exectution phase.
 *
 * Inputs:
 * transaction = a pointer to an s_fdc_trans structure, containing the information
 *               needed for the transaction
 *
 * Returns:
 * 0 on success, negative number on error
 */
short fdc_command(p_fdc_trans transaction) {
    short abort = 0;
    short i;
    short result = 0;
    unsigned char msr;
    unsigned long target_jiffies;

    TRACE("fdc_command");

    // fdc_log_transaction(transaction);

    if (fdc_wait_while_busy()) {
        /* Timed out waiting for the FDC to be free */
        logmsg(LOG_ERROR, "fdc_command: fdc_wait_while_busy timeout");
        return DEV_TIMEOUT;
    }

    result = fdc_out(transaction->command);      /* Send the command byte */
    if (result < 0) {
        logmsg(LOG_ERROR, "fdc_command: timeout sending command");
        return result;
    }

    for (i = 0; i < transaction->parameter_count; i++) {
        if ((result = fdc_out(transaction->parameters[i])) < 0) {
            logmsg(LOG_ERROR, "fdc_command: timeout sending parameters");
            return result;
        }
    }

    /* Check to see if there is an execution phase...
     * that is, there is data to transfer one way or the other
     */
    switch (transaction->direction) {
        case FDC_TRANS_WRITE:
            /* We're writing to the FDC */
            fdc_wait_rqm();
            if (*FDC_MSR & FDC_MSR_NONDMA) {
                for (i = 0; (i < transaction->data_count); i++) {
                    if ((result = fdc_out(transaction->data[i])) < 0) {
                        logmsg(LOG_ERROR, "fdc_command: timeout writing data");
                        return result;
                    }
                }
            }
            break;

        case FDC_TRANS_READ:
            /* We're reading from the FDC */
            for (i = 0; (i < transaction->data_count); i++) {
                if ((result = fdc_in(&transaction->data[i])) < 0) {
                    logmsg(LOG_ERROR, "fdc_command: timeout getting data");
                    return result;
                }
            }
            break;

        default:
            break;
    }

    /* Result phase: read the result bytes */

    fdc_delay(2);

    for (i = 0; i < transaction->result_count; i++) {
        if ((result = fdc_in(&transaction->results[i])) < 0) {
            logmsg(LOG_ERROR, "fdc_command: timeout getting results");
            return result;
        }
    }

    /* Wait until the FDC is not busy */

    result = fdc_wait_while_busy();

    return result;
}

/*
 * Move the read/write head to the indicated cylinder
 */
short fdc_seek(unsigned char cylinder) {
    long target_jiffies;
    t_fdc_trans trans;
    unsigned char st0, pcn, msr;
    short result, i;

    TRACE("fdc_seek");

    fdc_motor_on();

    trans.retries = FDC_DEFAULT_RETRIES;
    trans.command = FDC_CMD_SEEK;                           /* SEEK command */
    trans.direction = FDC_TRANS_NONE;                       /* We're not going to transfer data */
    trans.parameters[0] = 0;                                /* Set head and drive # */
    trans.parameters[1] = cylinder & 0x00ff;
    trans.parameter_count = 2;                              /* Sending 2 parameter bytes */

    trans.data = 0;                                         /* We don't use a buffer */
    trans.data_count = 0;

    trans.result_count = 0;                                 /* Expect 0 result bytes */

    while (trans.retries > 0) {
        result = fdc_command(&trans);                       /* Issue the transaction */
        if ((result == 0)) {
            break;
        } else {
            log_num(LOG_ERROR, "fdc_seek: retry ", result);
        }
        fdc_init();
        trans.retries--;
    }

    if (trans.retries == 0) {
        /* If we got here, we exhausted our retry attempts */
        return ERR_GENERAL;
    }

    target_jiffies = timers_jiffies() + fdc_seek_timeout;
    while (target_jiffies > timers_jiffies());

    if (fdc_sense_interrupt_status(&st0, &pcn)) {
        return DEV_TIMEOUT;
    }

    if ((pcn == cylinder)) {
        return 0;
    } else {
        return ERR_GENERAL;
    }
}

/*
 * Recalibrate the read/write head
 */
short fdc_recalibrate() {
    long target_jiffies;
    t_fdc_trans trans;
    unsigned char st0, pcn, msr;
    short result, i;

    TRACE("fdc_recalibrate");

    fdc_motor_on();

    trans.retries = FDC_DEFAULT_RETRIES;
    trans.command = FDC_CMD_RECALIBRATE;                    /* RECALIBRATE command */
    trans.direction = FDC_TRANS_NONE;                       /* We're not going to transfer data */
    trans.parameters[0] = 0;                                /* Set head and drive # */
    trans.parameter_count = 1;                              /* Sending 1 parameter byte */

    trans.data = 0;                                         /* We don't use a buffer */
    trans.data_count = 0;

    trans.result_count = 0;                                 /* Expect 0 result bytes */

    while (trans.retries > 0) {
        result = fdc_command(&trans);                       /* Issue the transaction */
        if ((result == 0)) {
            break;
        } else {
            log_num(LOG_INFO, "fdc_recalibrate: retry ", result);
        }
        fdc_init();
        trans.retries--;
    }

    if (trans.retries == 0) {
        /* If we got here, we exhausted our retry attempts */
        return ERR_GENERAL;
    }

    target_jiffies = timers_jiffies() + fdc_seek_timeout;
    while (target_jiffies > timers_jiffies());

    if (fdc_sense_interrupt_status(&st0, &pcn)) {
        return DEV_TIMEOUT;
    }

    if (pcn == 0) {
        return 0;
    } else {
        return ERR_GENERAL;
    }
}

/*
 * Recalibrate the read/write head
 *
 * Returns:
 * status code (ST3) if positive, error code if negative
 */
short fdc_sense_status() {
    long target_jiffies;
    t_fdc_trans trans;
    unsigned char st0, pcn, msr;
    short result, i;

    TRACE("fdc_sense_status");

    trans.retries = FDC_DEFAULT_RETRIES;
    trans.command = FDC_CMD_SENSE_DRIVE_STATUS;             /* SENSE STATUS command */
    trans.direction = FDC_TRANS_NONE;                       /* We're not going to transfer data */
    trans.parameters[0] = 0;                                /* Set head and drive # */
    trans.parameter_count = 1;                              /* Sending 1 parameter byte */

    trans.data = 0;                                         /* We don't use a buffer */
    trans.data_count = 0;

    trans.result_count = 1;                                 /* Expect 1 result bytes */

    while (trans.retries > 0) {
        result = fdc_command(&trans);                       /* Issue the transaction */
        if ((result == 0)) {
            break;
        } else {
            log_num(LOG_INFO, "fdc_sense_status: retry ", result);
        }
        fdc_init();
        trans.retries--;
    }

    if (trans.retries == 0) {
        /* If we got here, we exhausted our retry attempts */
        return ERR_GENERAL;
    }

    return (short)trans.results[0];
}

extern short fdc_cmd_asm(__reg("d1") short cmd,
                         __reg("d2") short argc, __reg("a1") uint8_t * argv,
                         __reg("a2") uint8_t * buffer,
                         __reg("d3") short resultc, __reg("a3") uint8_t * results);

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
short fdc_read(long lba, unsigned char * buffer, short size) {
    t_fdc_trans trans;
    unsigned char head, cylinder, sector;
    short result, i;
    char message[80];

    TRACE("fdc_read");

    lba_2_chs((unsigned long)lba, &cylinder, &head, &sector);

    // Signal that we need the motor on and check if the media has changed
    fdc_motor_on();
    fdc_media_check_change();

    trans.retries = 1; //FDC_DEFAULT_RETRIES;
    trans.command = 0x40 | FDC_CMD_READ_DATA;               /* MFM read command */
    trans.direction = FDC_TRANS_READ;                       /* We're going to read from the drive */
    trans.parameters[0] = (head == 1) ? 0x04 : 0x00;        /* Set head and drive # */
    trans.parameters[1] = cylinder & 0x00ff;
    trans.parameters[2] = head & 0x0001;
    trans.parameters[3] = sector & 0x00ff;
    trans.parameters[4] = 2;
    trans.parameters[5] = sector & 0x00ff;                  /* End of Track... fdc_sectors_per_track; */
    trans.parameters[6] = 0x1B;                             /* GPL = 0x1B */
    trans.parameters[7] = 0xFF;                             /* DTL = 0xFF */
    trans.parameter_count = 8;                              /* Sending 8 parameter bytes */

    trans.data = buffer;                                    /* Transfer sector data to buffer */
    trans.data_count = size;

    trans.result_count = 7;                                 /* Expect 7 result bytes */

    while (trans.retries > 0) {
        if (fdc_use_dma) {
            result = fdc_command_dma(&trans);               /* Issue the transaction */
            log_num(LOG_INFO, "fdc_command_dma: ", result);
        } else {
            result = fdc_command(&trans);
            log_num(LOG_INFO, "fdc_cmd: ", result);
        }

        if ((result == 0)) { //} && ((trans.results[0] & 0xC0) == 0)) {
            sprintf(message, "fdc_read: success? ST0=%02X ST1=%02X ST2=%02X C=%02X H=%02X R=%02X N=%02X",
                trans.results[0], trans.results[1], trans.results[2], trans.results[3], trans.results[4], trans.results[5], trans.results[6]);
            logmsg(LOG_INFO, message);
            return size;
        } else {
            sprintf(message, "fdc_read: retry ST0=%02X ST1=%02X ST2=%02X C=%02X H=%02X R=%02X N=%02X",
                trans.results[0], trans.results[1], trans.results[2], trans.results[3], trans.results[4], trans.results[5], trans.results[6]);
            logmsg(LOG_ERROR, message);
        }
        fdc_init();
        trans.retries--;
    }

    /* If we got here, we exhausted our retry attempts */
    return DEV_CANNOT_READ;
}

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
short fdc_write(long lba, const unsigned char * buffer, short size) {
    t_fdc_trans trans;
    unsigned char head, cylinder, sector;
    short result, i;
    char message[80];

    TRACE("fdc_read");

    lba_2_chs((unsigned long)lba, &cylinder, &head, &sector);

    // Signal that we need the motor on and check if the media has changed
    fdc_motor_on();
    fdc_media_check_change();

    trans.retries = 1; //FDC_DEFAULT_RETRIES;
    trans.command = 0x40 | FDC_CMD_WRITE_DATA;              /* MFM read command */
    trans.direction = FDC_TRANS_WRITE;                      /* We're going to read from the drive */
    trans.parameters[0] = (head == 1) ? 0x04 : 0x00;        /* Set head and drive # */
    trans.parameters[1] = cylinder & 0x00ff;
    trans.parameters[2] = head & 0x0001;
    trans.parameters[3] = sector & 0x00ff;
    trans.parameters[4] = 2;
    trans.parameters[5] = sector & 0x00ff;                  /* End of Track... fdc_sectors_per_track; */
    trans.parameters[6] = 0x1B;                             /* GPL = 0x1B */
    trans.parameters[7] = 0xFF;                             /* DTL = 0xFF */
    trans.parameter_count = 8;                              /* Sending 8 parameter bytes */

    trans.data = buffer;                                    /* Transfer sector data to buffer */
    trans.data_count = size;

    trans.result_count = 7;                                 /* Expect 7 result bytes */

    while (trans.retries > 0) {
        if (fdc_use_dma) {
            result = fdc_command_dma(&trans);               /* Issue the transaction */
            log_num(LOG_INFO, "fdc_command_dma: ", result);
        } else {
            result = fdc_command(&trans);
            log_num(LOG_INFO, "fdc_cmd: ", result);
        }

        if ((trans.results[1] & 0x02) != 0) {
            logmsg(LOG_ERROR, "Disk is write protected");
            g_fdc_stat |= FDC_STAT_PROTECTED;
            return DEV_WRITEPROT;
        }

        if ((result == 0)) { //} && ((trans.results[0] & 0xC0) == 0)) {
            sprintf(message, "fdc_write: success? ST0=%02X ST1=%02X ST2=%02X C=%02X H=%02X R=%02X N=%02X",
                trans.results[0], trans.results[1], trans.results[2], trans.results[3], trans.results[4], trans.results[5], trans.results[6]);
            logmsg(LOG_INFO, message);
            return size;
        } else {
            sprintf(message, "fdc_write: retry ST0=%02X ST1=%02X ST2=%02X C=%02X H=%02X R=%02X N=%02X",
                trans.results[0], trans.results[1], trans.results[2], trans.results[3], trans.results[4], trans.results[5], trans.results[6]);
            logmsg(LOG_ERROR, message);
        }
        fdc_init();
        trans.retries--;
    }

    /* If we got here, we exhausted our retry attempts */
    return DEV_CANNOT_WRITE;
}

/*
 * Return the status of the FDC
 *
 * Returns:
 *  the status of the device
 */
short fdc_status() {
    return g_fdc_stat;
}

/*
 * Return any error code of the FDC
 *
 * Returns:
 *  the error code of the device
 */
short fdc_error() {
    return 0;
}

/*
 * Ensure that any pending writes to teh device have been completed
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
short fdc_flush() {
    return 0;
}

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
short fdc_ioctrl(short command, unsigned char * buffer, short size) {
    switch (command) {
        case FDC_CTRL_MOTOR_ON:
            // Turn on the spindle motor
            return fdc_motor_on();

        case FDC_CTRL_MOTOR_OFF:
            // Turn off the spindle motor
            fdc_motor_off();
            return 0;

        case FDC_CTRL_CHECK_CHANGE:
            // Check to see if the disk has changed
            fdc_motor_on();
            return fdc_media_check_change();

        default:
            return 0;
    }
}

/*
 * Initialize the FDC
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
short fdc_init() {
    if (fdc_reset() < 0) {
        logmsg(LOG_ERROR, "Unable to reset the FDC");
        return DEV_TIMEOUT;
    }

    // Recalibrate the drive
    if (fdc_recalibrate()) {
        logmsg(LOG_ERROR, "Unable to recalibrate the drive");
        return ERR_GENERAL;
    }

    return 0;
}

/*
 * Install the FDC driver
 */
short fdc_install() {
    t_dev_block bdev;

    TRACE("fdc_install");

    bdev.number = BDEV_FDC;
    bdev.name = "FDC";
    bdev.init = fdc_init;
    bdev.read = fdc_read;
    bdev.write = fdc_write;
    bdev.status = fdc_status;
    bdev.flush = fdc_flush;
    bdev.ioctrl = fdc_ioctrl;

    g_fdc_stat = FDC_STAT_PRESENT & FDC_STAT_NOINIT;

    return bdev_register(&bdev);
}

#endif
