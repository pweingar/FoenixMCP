/*
 * Parallel port printer driver
 */

#include "features.h"
#include "errors.h"
#include "log.h"
#include "lpt_reg.h"
#include "dev/lpt.h"
#include "dev/txt_screen.h"
#include "simpleio.h"
#include "sys_general.h"
#include "syscalls.h"

#if HAS_PARALLEL_PORT

#define MAX_LPT_JIFFIES     600

/**
 * Wait a little bit...
 */
void lpt_delay() {
    long target_jiffies = sys_time_jiffies() + 1;
    while (target_jiffies > sys_time_jiffies()) ;
}

/**
 * Initialize the printer... assert the INIT pin to trigger a reset on the printer
 */
short lpt_initialize() {
    int i;

    /* Set the outputs to start the initialization process */
    *LPT_CTRL_PORT = LPT_CTRL_SELECT;
    lpt_delay();

    /* Set the outputs to stop the initialization process */
    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT;

    return 0;
}

/**
 * Open a connection to the printer... all we do is assert the SELECT pin
 */
short lpt_open(t_channel * chan, const uint8_t * path, short mode) {
    lpt_initialize();

    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT;

    // Write a dummy character to kick everything off
    lpt_write_b(0, "\x00", 0);

    return 0;
}

/**
 * Close the connection to the printer... all we do is deassert the SELECT pin
 */
short lpt_close(t_channel * chan) {
    *LPT_CTRL_PORT = LPT_CTRL_mINIT;
    return 0;
}

/**
 * Write a character to the parallel port
 */
short lpt_write_b(p_channel chan, unsigned char b) {
    /* This write routine is polled I/O. */
    long target_jiffies = 0;

    /* Wait until the printer is not busy */
    target_jiffies = sys_time_jiffies() + MAX_LPT_JIFFIES;
    while ((*LPT_STAT_PORT & LPT_STAT_nBUSY) != LPT_STAT_nBUSY) {
        lpt_delay();
        if (target_jiffies < sys_time_jiffies()) {
            return DEV_TIMEOUT;
        }
    }

    /* Send the byte */
    *LPT_DATA_PORT = b;

    /* Strobe the interface */
    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT;
    lpt_delay();
    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT | LPT_CTRL_STROBE;

    /* Wait until the printer is not busy */
    target_jiffies = sys_time_jiffies() + MAX_LPT_JIFFIES;
    while ((*LPT_STAT_PORT & LPT_STAT_nBUSY) != LPT_STAT_nBUSY) {
        lpt_delay();
        if (target_jiffies < sys_time_jiffies()) {
            return DEV_TIMEOUT;
        }
    }

    unsigned char status = *LPT_STAT_PORT;
    if ((status & (LPT_STAT_nERROR | LPT_STAT_PO)) != LPT_STAT_nERROR ) {
        // Online, there's paper, not busy, and not in error
        if (status & LPT_STAT_PO) {
            return DEV_NOMEDIA;
        } else {
            return ERR_GENERAL;
        }
    } else {
        return 0;
    }

    return 0;
}

/*
 * Write a buffer of bytes to the parallel port
 */
short lpt_write(p_channel chan, const uint8_t * buffer, short size) {
    int i;
    short result;

    for (i = 0; i < size; i++) {
        result = lpt_write_b(chan, buffer[i]);
        if (result) {
            return result;
        }
    }

    return 0;
}

/**
 * Return the status of the printer
 */
short lpt_status(p_channel chan) {
    short result = 0;

    // Get the status
    unsigned char stat = *LPT_STAT_PORT;

    // Conver the status bits to be consistent with channels
    if ((stat & LPT_STAT_nERROR) == 0) result |= LPT_STATUS_ERROR;
    if (stat & LPT_STAT_PO) result |= LPT_STATUS_PAPER;
    if (stat & LPT_STAT_SELECT) result |= LPT_STATUS_ONLINE;
    if ((stat & (LPT_STAT_nERROR | LPT_STAT_PO | LPT_STAT_nBUSY | LPT_STAT_SELECT)) == LPT_STAT_nERROR | LPT_STAT_nBUSY | LPT_STAT_SELECT) {
        // Online, there's paper, not busy, and not in error
        result |= LPT_STATUS_WRITABLE;
    }

    return result;
}

/**
 * Install the LPT driver
 */
short lpt_install() {
    t_dev_chan dev;

    dev.name = "LPT";
    dev.number = CDEV_LPT;
    dev.init = 0;
    dev.open = lpt_open;
    dev.close = lpt_close;
    dev.read = 0;
    dev.readline = 0;
    dev.read_b = 0;
    dev.write = lpt_write;
    dev.write_b = lpt_write_b;
    dev.flush = 0;
    dev.seek = 0;
    dev.status = lpt_status;
    dev.ioctrl = 0;

    return cdev_register(&dev);
}

#endif
