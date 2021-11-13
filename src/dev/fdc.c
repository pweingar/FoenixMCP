/**
 * Definitions support low level FDC device driver
 */

#if MODEL == MODEL_FOENIX_A2560K

#include "log.h"
#include "types.h"
#include "timers.h"
#include "fdc.h"
#include "fdc_reg.h"

/*
 * Constants
 */

const long fdc_motor_wait = 18;         /* The number of jiffies to wait for the motor to spin up: 300ms */
const long fdc_motor_timeout = 120;     /* The number of jiffies to let the motor spin without activity: 2 seconds */
const long fdc_timeout = 10;            /* The number of jiffies to allow for basic wait loops */

/*
 * Types
 */

enum fdc_command {
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
 * Device variables
 */

static unsigned char fdc_stat = 0;
static long fdc_motor_off_time = 0;     /* The time (in jiffies) when the motor should turn off */

/*
 * Wait for the FDC to be ready for a transfer from the CPU
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_check_rqm() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_check_rqm");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_RQM) == 0) && (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_RQM) == FDC_MSR_RQM) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}

/*
 * Wait while drive #0 is busy...
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_wait_drv0_busy() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_drv0_busy");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_DRV0BSY) == FDC_MSR_DRV0BSY) && (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_DRV0BSY) == 0) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}

/*
 * Wait until the CPU can read data from the FDC
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_can_read_data() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_can_read_data");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_DIO) == 0) && (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_DIO) == FDC_MSR_DIO) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}

/*
 * Wait until the CPU can write data to the FDC
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_wait_can_write() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_can_write");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_DIO) == FDC_MSR_DIO) && (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_DIO) == 0) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}

/*
 * Wait while the FDC is busy with a command...
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_wait_cmd_busy() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char msr;

    TRACE("fdc_wait_cmd_busy");

    do {
        msr = *FDC_MSR;
    } while (((msr & FDC_MSR_CMDBSY) == FDC_MSR_CMDBSY) && (target_time > timers_jiffies()));

    if ((msr & FDC_MSR_CMDBSY) == 0) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}

/*
 * Wait until the FDC has a byte ready to ready
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_can_read() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char master_status;

    TRACE("fdc_can_read");

    do {
        master_status = *FDC_MSR & (FDC_MSR_DIO | FDC_MSR_RQM);
    } while ((target_time > timers_jiffies()) && (master_status != (FDC_MSR_RQM | FDC_MSR_DIO)));

    if ((master_status & FDC_MSR_DIO) != 0) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}


/*
 * Wait until the FDC is clear to receive a byte
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_can_write() {
    long target_time = timers_jiffies() + fdc_timeout;
    unsigned char master_status;

    TRACE("fdc_can_write");

    do {
        master_status = *FDC_MSR & (FDC_MSR_DIO | FDC_MSR_RQM);
    } while ((target_time > timers_jiffies()) && (master_status != FDC_MSR_RQM));

    if ((master_status & FDC_MSR_DIO) == 0) {
        /* Return success */
        return 0;
    } else {
        /* Return that we hit a timeout */
        return -1;
    }
}

/*
 * Spin up the drive's spindle motor
 */
short fdc_motor_on() {
    TRACE("fdc_motor_on");

    log_num(LOG_TRACE, "FDC_DOR: ", *FDC_DOR);

    // if ((*FDC_DOR & FDC_DOR_MOT0) != FDC_DOR_MOT0) {
    //     /* Motor is not on... turn it on without DMA or RESET */
        *FDC_DOR = FDC_DOR_MOT0 | FDC_DOR_NRESET;

        log_num(LOG_TRACE, "FDC_DOR 2: ", *FDC_DOR);

        if (fdc_check_rqm()) {
            /* Got a timeout after trying to turn on the motor */
            log(LOG_ERROR, "fdc_motor_on timeout");
            return -1;
        }

        /* Wait a decent time for the motor to spin up */
        long wait_time = timers_jiffies() + fdc_motor_wait;
        while (wait_time > timers_jiffies()) ;
    // }

    /* Set a new target time to shut off the motor */
    fdc_motor_off_time = timers_jiffies() + fdc_motor_timeout;

    /* Flag that the motor is on */
    fdc_stat |= FDC_STAT_MOTOR_ON;

    return 0;
}

/*
 * Spin down the drive's spindle motor
 */
void fdc_motor_off() {
    TRACE("fdc_motor_off");

    if ((*FDC_DOR & FDC_DOR_MOT0) == FDC_DOR_MOT0) {
        /* Motor is not on... turn it on without DMA or RESET */
        *FDC_DOR = FDC_DOR_NRESET;

        if (fdc_check_rqm()) {
            /* Got a timeout after trying to turn on the motor */
            return;
        }
    }

    /* Flag that the motor is off */
    fdc_stat &= ~FDC_STAT_MOTOR_ON;
}

/*
 * Issue the SENSE INTERRUPT command
 *
 * Inputs:
 * st0 = pointer to the ST0 location
 * pcn = pointer to the Present Cylinder Number location
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_sense_interrupt_status(unsigned char *st0, unsigned char *pcn) {
    TRACE("fdc_sense_interrupt_status");

    /* Set up "return" values to something neutral */
    *st0 = 0;
    *pcn = 0xff;

    if (fdc_wait_cmd_busy() < 0) {
        /* Timed out waiting for the FDC to be free */
        log(LOG_ERROR, "fdc_wait_cmd_busy: timeout");
        return -1;
    }

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to write a command byte */
        log(LOG_ERROR, "fdc_check_rqm 1: timeout");
        return -1;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_SENSE_INTERRUPT;

    if (fdc_can_read_data() < 0) {
        /* Timed out waiting to receive data */
        log(LOG_ERROR, "fdc_can_read_data: timeout");
        return -1;
    }

    /* Read the status byte */
    *st0 = *FDC_DATA;

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        log(LOG_ERROR, "fdc_check_rqm 2: timeout");
        return -1;
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
 * 0 on success, -1 on timeout
 */
short fdc_specify() {
    TRACE("fdc_specify");

    if (fdc_wait_cmd_busy() < 0) {
        /* Timed out waiting for the FDC to be free */
        return -1;
    }

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to write a command byte */
        return -1;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_SPECIFY;

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        return -1;
    }

    /* Send the seek rate time and head load times */
    *FDC_DATA = 0xCF;

    if (fdc_can_write() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        return -1;
    }

    /* Set head unload time to maximum, and no DMA */
    *FDC_DATA = 0x01;

    return 0;
}

/*
 * Issue the CONFIGURE command
 *
 * Inputs:
 *
 * Returns:
 * 0 on success, -1 on timeout
 */
short fdc_configure() {
    TRACE("fdc_configure");

    if (fdc_wait_cmd_busy() < 0) {
        /* Timed out waiting for the FDC to be free */
        return -1;
    }

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to write a command byte */
        return -1;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_CONFIGURE;

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        return -1;
    }

    /* Send a padding byte of 0 */
    *FDC_DATA = 0x00;

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        return -1;
    }

    /* Implied seek, enable FIFO, enable POLL, FIFO threshold = 4 bytes */
    *FDC_DATA = 0x44;

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        return -1;
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
 * 0 on success, -1 on timeout
 */
short fdc_version(unsigned char *version) {
    TRACE("fdc_version");

    if (fdc_wait_cmd_busy() < 0) {
        /* Timed out waiting for the FDC to be free */
        log(LOG_ERROR, "fdc_version fdc_wait_cmd_busy timed out");
        return -1;
    }

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to write a command byte */
        log(LOG_ERROR, "fdc_version fdc_check_rqm 1 timed out");
        return -1;
    }

    /* Send command */
    *FDC_DATA = FDC_CMD_VERSION;

    if (fdc_check_rqm() < 0) {
        /* Timed out waiting for permission to transfer another byte */
        log(LOG_ERROR, "fdc_version fdc_check_rqm 2 timed out");
        return -1;
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
    target_time = timers_jiffies() + fdc_timeout;
    *FDC_DOR = 0;
    while (target_time > timers_jiffies()) ;
    *FDC_DOR = FDC_DOR_NRESET;

    /* Default precompensation, data rate for 1.44MB */
    *FDC_DSR = 0;

    /* Set data rate for 1.44 MB */
    *FDC_CCR = 0;

    /* Issue the sense interrupt command 4 times. */
    for (i = 0; i < 4; i++) {
        st0 = 0;
        pcn = 0;
        if (fdc_sense_interrupt_status(&st0, &pcn) < 0) {
            /* Timeout on sense interrupt */
            log(LOG_ERROR, "fdc_sense_interrupt_status timeout");
            return -1;
        }

        log_num(LOG_INFO, "ST0: ", st0);
        log_num(LOG_INFO, "PCN: ", pcn);

        if (st0 = 0xC0) {
            break;
        }
    }

    /* Configure the FDC */
    if (fdc_configure() < 0) {
        /* Timeout on sense interrupt */
        log(LOG_ERROR, "fdc_configure timeout");
        return -1;
    }

    /* Specify seek and head times, and turn off DMA */
    if (fdc_specify() < 0) {
        /* Timeout on sense interrupt */
        log(LOG_ERROR, "fdc_specify timeout");
        return -1;
    }

    fdc_motor_on();

    return 0;
}

/*
 * Install the FDC driver
 */
short fdc_install() {
    fdc_stat = FDC_STAT_NOINIT;

    return 0;
}

/*
 * Initialize the FDC
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
short fdc_init() {
    unsigned char version;

    if (fdc_version(&version) < 0) {
        log(LOG_ERROR, "Unable to get FDC version");
        return -1;
    }

    log_num(LOG_INFO, "FDC version: ", version);

    if (fdc_reset() < 0) {
        log(LOG_ERROR, "Unable to reset the FDC");
        return -1;
    }

    fdc_stat &= ~FDC_STAT_NOINIT;
    return 0;
}

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
    return 0;
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
    return 0;
}

/*
 * Return the status of the FDC
 *
 * Returns:
 *  the status of the device
 */
short fdc_status() {
    return fdc_stat;
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
            return fdc_motor_on();

        case FDC_CTRL_MOTOR_OFF:
            fdc_motor_off();
            return 0;

        default:
            return 0;
    }
}

#endif
