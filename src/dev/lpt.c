/*
 * Parallel port printer driver
 */

#include "log.h"
#include "dev/lpt.h"
#include "dev/text_screen_iii.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K

#define LPT_DATA_PORT   ((volatile unsigned char *)0x00C02378)

#define LPT_STAT_PORT   ((volatile unsigned char *)0x00C02379)
#define LPT_STAT_BUSY   0x80
#define LPT_STAT_ACK    0x40
#define LPT_STAT_PO     0x20
#define LPT_STAT_SELECT 0x10
#define LPT_STAT_ERROR  0x08
#define LPT_STAT_IRQ    0x04

#define LPT_CTRL_PORT   ((volatile unsigned char *)0x00C0237A)
#define LPT_CTRL_STROBE 0x01
#define LPT_CTRL_AL     0x02
#define LPT_CTRL_INIT   0x04
#define LPT_CTRL_SELECT 0x08
#define LPT_CTRL_IRQE   0x10
#define LPT_CTRL_BI     0x20

#define LPT_INIT_ON     0x08            /* Start the printer initialization process */
#define LPT_INIT_OFF    0x0C            /* Stop the printer initialization process */
#define LPT_STROBE_ON   0x0F            /* Strobe the printer */
#define LPT_STROBE_OFF  0x0E            /* Drop the strobe to the printer */

short lpt_delay() {
    int i;
    short x;
    for (i = 0, x = 0; i < 10; i++) {
        x++;
    }
    return x;
}

/*
 * Install the LPT driver
 */
short lpt_install() {
    return 0;
}

void lpt_initialize() {
    int i;

    /* Set the outputs to start the initialization process */
    *LPT_CTRL_PORT = LPT_INIT_ON;

    /* Wait 50 micro seconds */
    lpt_delay();

    /* Set the outputs to stop the initialization process */
    *LPT_CTRL_PORT = LPT_INIT_OFF;

    lpt_delay();
}

short lpt_wait_busy() {
    unsigned char stat = 0;
    do {
        stat = *LPT_STAT_PORT;
        if ((stat & LPT_STAT_ERROR) == 0) {
            // There was an error...
            DEBUG("LPT: lpt_wait_busy error");
            return -1;
        } else if (stat & LPT_STAT_PO) {
            // Out of paper
            DEBUG("LPT: lpt_wait_busy out of paper");
            return -1;
        }
    } while ((stat & LPT_STAT_BUSY) == 0);

    return 0;
}

short lpt_wait_ack() {
    unsigned char stat = 0;
    short counter = 0;

    do {
        stat = *LPT_STAT_PORT;
    } while ((counter++ < 32000) && ((stat & LPT_STAT_ACK) != 0));

    return 0;
}

/*
 * Write a character to the parallel port
 */
short lpt_write_b(p_channel chan, unsigned char b) {
    /* This write routine is polled I/O. */
    /* TODO: convert it to interrupt driven */

    /* Wait until the printer is not busy */
    if (lpt_wait_busy()) {
        // If we got an error, return an error
        DEBUG("LPT: Error writing");
        return -1;
    }

    *LPT_DATA_PORT = b;                 /* Send the byte */
    *LPT_CTRL_PORT = LPT_STROBE_ON;     /* Strobe the interface */

    lpt_delay();

    /* Wait for the printer to acknowledge */
    if (lpt_wait_ack()) {
        // If we got an error, return an error
        *LPT_CTRL_PORT = LPT_STROBE_OFF;    /* Drop the strobe */
        return -1;
    }

    *LPT_CTRL_PORT = LPT_STROBE_OFF;    /* Drop the strobe */

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

#endif
