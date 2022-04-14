/*
 * Parallel port printer driver
 */

#include "errors.h"
#include "log.h"
#include "dev/lpt.h"
#include "dev/txt_screen.h"
#include "simpleio.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K

#define LPT_DATA_PORT   ((volatile unsigned char *)0xFEC02378)

#define LPT_STAT_PORT   ((volatile unsigned char *)0xFEC02379)
#define LPT_STAT_BUSY   0x80
#define LPT_STAT_ACK    0x40
#define LPT_STAT_PO     0x20
#define LPT_STAT_SELECT 0x10
#define LPT_STAT_ERROR  0x08
#define LPT_STAT_IRQ    0x04

#define LPT_CTRL_PORT   ((volatile unsigned char *)0xFEC0237A)
#define LPT_CTRL_STROBE 0x01
#define LPT_CTRL_AL     0x02
#define LPT_CTRL_INIT   0x04
#define LPT_CTRL_SELECT 0x08
#define LPT_CTRL_IRQE   0x10
#define LPT_CTRL_BI     0x20

#define LPT_INIT_ON     0x04            /* Start the printer initialization process */
#define LPT_INIT_OFF    0x0C            /* Stop the printer initialization process */
#define LPT_STROBE_ON   0x0D            /* Strobe the printer */
#define LPT_STROBE_OFF  0x0C            /* Drop the strobe to the printer */

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
    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT;

    return 0;
}

/**
 * Open a connection to the printer... all we do is assert the SELECT pin
 */
short lpt_open(t_channel * chan, const uint8_t * path, short mode) {
    lpt_initialize();

    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT;
    return 0;
}

/**
 * Close the connection to the printer... all we do is deassert the SELECT pin
 */
short lpt_close(t_channel * chan) {
    *LPT_CTRL_PORT = LPT_CTRL_INIT;
    return 0;
}

/**
 * Write a character to the parallel port
 */
short lpt_write_b(p_channel chan, unsigned char b) {
    /* This write routine is polled I/O. */

    /* Wait until the printer is not busy */
    while ((*LPT_STAT_PORT & LPT_STAT_BUSY) == LPT_STAT_BUSY) {
        lpt_delay();
    }

    /* Send the byte */
    *LPT_DATA_PORT = b;
    sys_chan_write_b(0, b);

    /* Strobe the interface */
    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT;
    lpt_delay();
    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT | LPT_CTRL_STROBE;

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
    dev.status = 0;
    dev.ioctrl = 0;

    return cdev_register(&dev);
}

#endif
