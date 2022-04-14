/*
 * Parallel port printer driver
 */

#include "log.h"
#include "dev/lpt.h"
#include "dev/txt_screen.h"
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

void lpt_initialize() {
    int i;

    /* Set the outputs to start the initialization process */
    *LPT_CTRL_PORT = LPT_CTRL_SELECT;
    lpt_delay();

    /* Set the outputs to stop the initialization process */
    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT;
}

/*
 * Write a character to the parallel port
 */
short lpt_write_b(p_channel chan, unsigned char b) {
    /* This write routine is polled I/O. */
    /* TODO: convert it to interrupt driven */

    /* Wait until the printer is not busy */
    while ((*LPT_STAT_PORT & LPT_STAT_BUSY) == LPT_STAT_BUSY) {
        lpt_delay();
    }

    /* Send the byte */
    *LPT_DATA_PORT = b;

    /* Strobe the interface */
    //unsigned char ctrl = *LPT_CTRL_PORT;
    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT ;
    lpt_delay();
    *LPT_CTRL_PORT = LPT_CTRL_INIT | LPT_CTRL_SELECT | LPT_CTRL_STROBE;

    return 0;                           /* Return success */
}

/*
 * Write a buffer of bytes to the parallel port
 */
short lpt_write(p_channel chan, unsigned char * buffer, short size) {
    int i;
    short result;

    for (i = 0; i < size; i++) {
        result = lpt_write_b(chan, buffer[i]);
        if (result < 0) {
            return result;
        }
    }

    return 0;
}

/**
 * Install the LPT driver
 */
short lpt_install() {
    return 0;
}

#endif
