/* MCP Parallel port device. For now it only supports writting to the serial port, not reading from it. */

#include "features.h"
#if HAS_PARALLEL_PORT

#include <stdint.h>
#include "channel.h"
#include "errors.h"
#include "libfoenix/include/lpt.h"
#include "libfoenix/include/rtc.h"

#define MAX_LPT_JIFFIES     600

static int16_t lpt_open(t_channel * chan, const uint8_t * path, int16_t mode);
static int16_t lpt_close(t_channel * chan);
static int16_t lpt_get_status(p_channel chan);
static int16_t lpt_write_b(p_channel chan, uint8_t b);
static int16_t lpt_write(p_channel chan, const uint8_t * buffer, int16_t size);

/**
 * Install the LPT driver
 */
int16_t lpt_install() {
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
    dev.status = lpt_get_status;
    dev.ioctrl = 0;

    return cdev_register(&dev);
}

/**
 * Open a connection to the printer... all we do is assert the SELECT pin
 */
static int16_t lpt_open(t_channel * chan, const uint8_t * path, int16_t mode) {
    lpt_init(MAX_LPT_JIFFIES);

    lpt_select(true);

    // Write a dummy character to kick everything off
    lpt_write_b(0, '\0');

    return E_OK;
}

/**
 * Close the connection to the printer... all we do is deassert the SELECT pin
 */
static int16_t lpt_close(t_channel * chan) {
    lpt_select(false);
    return E_OK;
}

/**
 * Return the status of the printer
 */
static int16_t lpt_get_status(p_channel chan) {
    // Get the status
    return lpt_status();
}

/**
 * Write a character to the parallel port
 */
static int16_t lpt_write_b(p_channel chan, uint8_t b) {
    /* This write routine is polled I/O. */
    int32_t target_jiffies = 0;

    /* Wait until the printer is not busy */
    target_jiffies = rtc_get_jiffies() + MAX_LPT_JIFFIES;
    while (lpt_busy()) {
        lpt_delay();
        if (target_jiffies < rtc_get_jiffies()) {
            return DEV_TIMEOUT;
        }
    }

    /* Send the byte */
    lpt_data(b);

    /* Strobe the interface */
    lpt_strobe();

    /* Wait until the printer is not busy */
    target_jiffies = rtc_get_jiffies() + MAX_LPT_JIFFIES;
    while (lpt_busy()) {
        lpt_delay();
        if (target_jiffies < rtc_get_jiffies()) {
            return DEV_TIMEOUT;
        }
    }

    uint8_t status = lpt_status();
    if (status & (LPT_STATUS_ERROR | LPT_STATUS_PAPER)) {
        // Online, there's paper, not busy, and not in error
        if (status & LPT_STATUS_PAPER) {
            return DEV_NOMEDIA;
        } else {
            return ERR_GENERAL;
        }
    } else {
        return E_OK;
    }

    return E_OK;
}

/*
 * Write a buffer of bytes to the parallel port
 */
static int16_t lpt_write(p_channel chan, const uint8_t * buffer, int16_t size) {
    int i;
    int16_t result;

    for (i = 0; i < size; i++) {
        result = lpt_write_b(chan, buffer[i]);
        if (result) {
            return result;
        }
    }

    return E_OK;
}

#endif
