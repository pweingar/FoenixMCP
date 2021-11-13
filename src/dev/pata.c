/**
 * Implementation of the PATA hard drive low level driver
 */

#include <string.h>
#include "log.h"
#include "errors.h"
#include "constants.h"
#include "indicators.h"
#include "dev/block.h"
#include "dev/pata.h"
#include "dev/text_screen_iii.h"
#include "dev/rtc.h"
#include "pata_reg.h"

//
// Constants
//

#define PATA_TIMEOUT_JF 20                  /* Timeout in jiffies: 1/60th second */
#define PATA_WAIT_JF    10                  /* Delay in jiffies: 1/60th second */

//
// Variables
//

short g_pata_error = 0;                     // Most recent error code received from the PATA drive
short g_pata_status = PATA_STAT_NOINIT;     // Status of the PATA interface

//
// Code
//

//
// Wait until the PATA drive is no longer busy, or we've run out of time
//
// Returns:
//  0 on success (PATA drive is no longer busy), DEV_TIMEOUT on timeout
//
short pata_wait_not_busy() {
    long target_ticks;
    long ticks;
    char status;

    TRACE("pata_wait_not_busy");

    target_ticks = rtc_get_jiffies() + PATA_TIMEOUT_JF;
    do {
        status = *PATA_CMD_STAT;
        ticks = rtc_get_jiffies();
    } while ((status & PATA_STAT_BSY) && (target_ticks > ticks));

    if (target_ticks <= ticks) {
        log(LOG_ERROR, "pata_wait_not_busy: timeout");
        log_num(LOG_ERROR, "target_ticks: ", (int)target_ticks);
        log_num(LOG_ERROR, "ticks: ", (int)ticks);
        return DEV_TIMEOUT;
    } else {
        return 0;
    }
}

//
// Wait until the PATA drive is ready, or we've run out of time
//
// Returns:
//  0 on success (PATA drive is ready), DEV_TIMEOUT on timeout
//
short pata_wait_ready() {
    long target_ticks;
    long ticks;
    char status;

    TRACE("pata_wait_ready");

    target_ticks = rtc_get_jiffies() + PATA_TIMEOUT_JF;
    do {
        status = *PATA_CMD_STAT;
        ticks = rtc_get_jiffies();
    } while (((status & PATA_STAT_DRDY) == 0) && (target_ticks > ticks));

    if (target_ticks <= ticks) {
        log(LOG_ERROR, "pata_wait_ready: timeout");
        return DEV_TIMEOUT;
    } else {
        return 0;
    }
}

//
// Wait until the PATA drive is ready and not busy, or we've run out of time
//
// Returns:
//  0 on success (PATA drive is ready and not busy), DEV_TIMEOUT on timeout
//
short pata_wait_ready_not_busy() {
    long target_ticks;
    long ticks;
    char status;

    TRACE("pata_wait_ready_not_busy");

    // do {
    //     status = *PATA_CMD_STAT;
    // } while (((status & (PATA_STAT_DRDY | PATA_STAT_BSY)) != PATA_STAT_DRDY) && (count-- > 0));

    target_ticks = rtc_get_jiffies() + PATA_TIMEOUT_JF;
    do {
        while (((*PATA_CMD_STAT & PATA_STAT_DRDY) != PATA_STAT_DRDY) && (target_ticks > ticks)) {
            ticks = rtc_get_jiffies();
        }
    } while (((*PATA_CMD_STAT & PATA_STAT_BSY) == PATA_STAT_BSY) && (target_ticks > ticks));

    if (target_ticks <= ticks) {
        log(LOG_ERROR, "pata_wait_ready_not_busy: timeout");
        log_num(LOG_ERROR, "target_ticks: ", (int)target_ticks);
        log_num(LOG_ERROR, "ticks: ", (int)ticks);
        return DEV_TIMEOUT;
    } else {
        return 0;
    }
}

//
// Wait until the PATA drive is ready to transfer data, or we've run out of time
//
// Returns:
//  0 on success (PATA drive is ready to transfer data), DEV_TIMEOUT on timeout
//
short pata_wait_data_request() {
    long target_ticks;
    long ticks;
    char status;

    TRACE("pata_wait_data_request");

    target_ticks = rtc_get_jiffies() + PATA_TIMEOUT_JF;
    do {
        status = *PATA_CMD_STAT;
        ticks = rtc_get_jiffies();
    } while (((status & PATA_STAT_DRQ) != PATA_STAT_DRQ) && (target_ticks > ticks));

    if (target_ticks <= ticks) {
        log(LOG_ERROR, "pata_wait_data_request: timeout");
        return DEV_TIMEOUT;
    } else {
        return 0;
    }
}

char g_buffer[512];

//
// Identify the PATA drive
//
// Inputs:
//  drive_info = pointer to a s_drive_info
//
// Returns:
//  0 on success, any negative number is an error code
//
short pata_identity(p_drive_info drive_info) {
    char * buffer;
    unsigned short *wptr;
    char * cptr;
    short i;
    short count;
    TRACE("pata_identity");

    *PATA_HEAD = 0xe0;          // Drive 0, lBA enabled, Head 0
    *PATA_SECT_CNT = 1;
    *PATA_SECT_SRT = 0;
    *PATA_CLDR_LO = 0;
    *PATA_CLDR_HI = 0;

    // Issue identity command
    *PATA_CMD_STAT = PATA_CMD_IDENTITY;
    if (pata_wait_not_busy()) {
        return DEV_TIMEOUT;
    }

    // TODO: Wait ~500ns

    if (pata_wait_ready_not_busy()) {
        return DEV_TIMEOUT;
    }

    TRACE("copying data");

    // Copy the data... let the compiler and the FPGA worry about endianess
    wptr = (unsigned short *)g_buffer;
    for (i = 0; i < 512; ) {
        unsigned short data = *PATA_DATA_16;
        g_buffer[i++] = data & 0xff;
        g_buffer[i++] = (data >> 8) & 0xff;
    }

    TRACE("data copied");

    wptr = (unsigned short *)buffer;
    drive_info->flags = g_buffer[1] << 16 | g_buffer[0];
    drive_info->lba_enabled = g_buffer[99] << 16 | g_buffer[98];
    drive_info->l.lbaw.lba_default_lo = g_buffer[121] << 8 | g_buffer[120];
    drive_info->l.lbaw.lba_default_hi = g_buffer[123] << 8 | g_buffer[122];

    // Copy the serial number (need to swap chars)
    memcpy(&(drive_info->serial_number), g_buffer + 22, sizeof(drive_info->serial_number));

    // Copy the firmware version (need to swap chars)
    memcpy(&(drive_info->firmware_version), g_buffer + 46, sizeof(drive_info->firmware_version));

    // Copy the model name (need to swap chars)
    memcpy(&(drive_info->model_name), g_buffer + 54, sizeof(drive_info->model_name));

    return 0;
}

//
// Initialize the PATA hard drive
//
// Returns:
//  0 on success, any negative number is an error code
//
short pata_init() {
    short result;

    TRACE("pata_init");

    // Issue intialize command
    *PATA_CMD_STAT = PATA_CMD_INIT;
    if (pata_wait_not_busy()) {
        return DEV_TIMEOUT;
    }

    *PATA_HEAD = 0xA0;          // Drive 0, lBA enabled, Head 0
    *PATA_SECT_CNT = 1;
    *PATA_SECT_SRT = 0;
    *PATA_CLDR_LO = 0;
    *PATA_CLDR_HI = 0;

    if (pata_wait_ready_not_busy()) {
        return DEV_TIMEOUT;
    }

    // Mark that the drive is initialized and present
    g_pata_status = PATA_STAT_PRESENT;

    return 0;
}

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
short pata_read(long lba, unsigned char * buffer, short size) {
    short i;
    unsigned short *wptr;
    TRACE("pata_read");
    log_num(LOG_VERBOSE, "pata_read lba: ", lba);

    /* Turn on the HDD LED */
    // ind_set(IND_HDC, IND_ON);

    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    *PATA_HEAD = ((lba >> 24) & 0x07) | 0xe0;       // Upper 3 bits of LBA, Drive 0, LBA mode.
    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    *PATA_SECT_CNT = 1;                             // Read one sector (make this an option?)
    *PATA_SECT_SRT = lba & 0xff;                    // Set the rest of the LBA
    *PATA_CLDR_LO = (lba >> 8) & 0xff;
    *PATA_CLDR_HI = (lba >> 16) & 0xff;

    *PATA_CMD_STAT = PATA_CMD_READ_SECTOR;          // Issue the READ command 0xE4; //

    // TODO: Wait ~500ns

    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    if (pata_wait_data_request()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    // Copy the data... let the compiler and the FPGA worry about endianess
    for (i = 0, wptr = (unsigned short *)buffer; i < size; i += 2) {
        *wptr++ = *PATA_DATA_16;
    }

    /* Turn off the HDD LED */
    // ind_set(IND_HDC, IND_OFF);

    return i;
}

short pata_flush_cache() {
    long target_ticks;
    short i;
    unsigned short *wptr;
    unsigned char status;
    TRACE("pata_write");

    if (pata_wait_ready_not_busy()) {
        return DEV_TIMEOUT;
    }

    *PATA_HEAD = 0xe0;                          // Drive 0

    if (pata_wait_ready_not_busy()) {
        return DEV_TIMEOUT;
    }

    *PATA_SECT_SRT = 0;                    // Set the rest of the LBA
    *PATA_CLDR_LO = 0;
    *PATA_CLDR_HI = 0;

    if (pata_wait_ready_not_busy()) {
        return DEV_TIMEOUT;
    }

    *PATA_CMD_STAT = 0xE7; // PATA_CMD_FLUSH_CACHE;

    // Give the controller some time (100ms?)...
    target_ticks = rtc_get_jiffies() + PATA_WAIT_JF;
    while (target_ticks > rtc_get_jiffies()) ;

    if (pata_wait_ready_not_busy()) {
        return DEV_TIMEOUT;
    }

    status = *PATA_CMD_STAT;
    if ((status & PATA_STAT_DF) != 0){
        log(LOG_ERROR, "pata_flush_cache: device fault");
        return -1;
    }

    if ((status & PATA_STAT_ERR) != 0) {
        log(LOG_ERROR, "pata_flush_cache: error");
        return -1;
    }

    return 0;
}

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
short pata_write(long lba, const unsigned char * buffer, short size) {
    long target_ticks;
    short i;
    unsigned short *wptr;
    unsigned char status;
    TRACE("pata_write");

    /* Turn on the HDD LED */
    // ind_set(IND_HDC, IND_ON);

    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    *PATA_HEAD = ((lba >> 24) & 0x07) | 0xe0;       // Upper 3 bits of LBA, Drive 0, LBA mode.

    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    *PATA_SECT_CNT = 1;                             // Read one sector (make this an option?)
    *PATA_SECT_SRT = lba & 0xff;                    // Set the rest of the LBA
    *PATA_CLDR_LO = (lba >> 8) & 0xff;
    *PATA_CLDR_HI = (lba >> 16) & 0xff;

    *PATA_CMD_STAT = PATA_CMD_WRITE_SECTOR;         // Issue the WRITE command

    // Give the controller some time (100ms?)...
    target_ticks = rtc_get_jiffies() + PATA_WAIT_JF;
    while (target_ticks > rtc_get_jiffies()) ;

    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    // Copy the data... let the compiler and the FPGA worry about endianess
    for (i = 0, wptr = (unsigned short *)buffer; i < size; i += 2) {
        *PATA_DATA_16 = *wptr++;
    }

    // Give the controller some time...
    for (i = 0; i < 32000; i++) ;

    if (pata_wait_ready_not_busy()) {
        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return DEV_TIMEOUT;
    }

    // Give the controller some time...
    for (i = 0; i < 32000; i++) ;

    status = *PATA_CMD_STAT;
    if ((status & PATA_STAT_DF) != 0){
        log(LOG_ERROR, "pata_write: device fault");

        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return -1;
    }

    if ((status & PATA_STAT_ERR) != 0) {
        log(LOG_ERROR, "pata_write: error");

        /* Turn off the HDD LED */
        // ind_set(IND_HDC, IND_OFF);

        return -1;
    }

    /* Turn off the HDD LED */
    // ind_set(IND_HDC, IND_OFF);

    return size;
}

//
// Return the status of the PATA hard drive
//
// Returns:
//  the status of the device
//
short pata_status() {
    TRACE("pata_status");
    return g_pata_status;
}

//
// Return any error code of the PATA hard drive
//
// Returns:
//  the error code of the device
//
short pata_error() {
    TRACE("pata_error");
    return g_pata_error;
}

//
// Ensure that any pending writes to teh device have been completed
//
// Returns:
//  0 on success, any negative number is an error code
//
short pata_flush() {
    TRACE("pata_flush");
    return 0;
}

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
short pata_ioctrl(short command, unsigned char * buffer, short size) {
    short result;
    long *p_long;
    unsigned short *p_word;
    long *p_lba_word;
    t_drive_info drive_info;
    p_drive_info p_info;

    TRACE("pata_ioctrl");

    switch (command) {
        case PATA_GET_SECTOR_COUNT:
            p_lba_word = (long *)buffer;
            result = pata_identity(&drive_info);
            if (result != 0) {
                return result;
            }

            *p_lba_word = drive_info.l.lba_default;
            break;

        case PATA_GET_SECTOR_SIZE:
            // Return the size of a sector... always 512
            p_word = (unsigned short *)buffer;
            *p_word = PATA_SECTOR_SIZE;
            break;

        case PATA_GET_BLOCK_SIZE:
            // This isn't a flash device... return 1
            p_long = (long *)buffer;
            *p_long = 1;
            break;

        case PATA_GET_DRIVE_INFO:
            p_info = (p_drive_info *)buffer;
            result = pata_identity(p_info);
            if (result != 0) {
                return result;
            }
            break;

        default:
            return 0;
    }

    return 0;
}

//
// Install the PATA driver
//
// Returns:
//  0 on success, any negative number is an error code
//
short pata_install() {
    t_dev_block bdev;

    TRACE("pata_install");

    g_pata_error = 0;
    g_pata_status = PATA_STAT_NOINIT;

    // Check if drive is installed
    // if ((*DIP_BOOTMODE & HD_INSTALLED) == 0) {
        bdev.number = BDEV_HDC;
        bdev.name = "HDD";
        bdev.init = pata_init;
        bdev.read = pata_read;
        bdev.write = pata_write;
        bdev.status = pata_status;
        bdev.flush = pata_flush;
        bdev.ioctrl = pata_ioctrl;

        g_pata_status = PATA_STAT_PRESENT & PATA_STAT_NOINIT;

        return bdev_register(&bdev);
    // } else {
    //     return 0;
    // }
}
