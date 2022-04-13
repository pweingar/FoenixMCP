/*
 * Definitions for the MIDI ports
 */

#include "midi_reg.h"
#include "dev/midi.h"
#include "errors.h"
#include "simpleio.h"
#include "sys_general.h"
#include "timers.h"

#if MODEL == MODEL_FOENIX_A2560K

/** Timeout for waiting on the MIDI interface */
const long midi_timeout = 10;

/**
 * Wait for data to be ready to read...
 *
 * @return 1 on success, 0 if there is a timeout
 */
short midi_can_read() {
    long target = timers_jiffies() + midi_timeout;
    do {
        if ((*MIDI_STAT & MIDI_STAT_RX_EMPTY) == 0) {
            // There is data waiting
            return 1;
        }
    } while (target > timers_jiffies());

    // We have waited too long
    return 0;
}

/**
 * Wait for the MIDI transmiter to be empty...
 *
 * @return 1 on success, 0 if there is a timeout
 */
short midi_can_write() {
    long target = timers_jiffies() + midi_timeout;
    do {
        if ((*MIDI_STAT & MIDI_STAT_TX_BUSY) == 0) {
            // The transmit buffer is empty
            return 1;
        }
    } while (target > timers_jiffies());

    // We have waited too long
    return 0;
}

/**
 * Send a command to the MPU-401
 *
 * @param cmd the command byte
 * @return 0 on success, any other number is an error
 */
short midi_command(unsigned char cmd) {
    if (midi_can_write()) {
        *MIDI_CMD = cmd;
        return 0;
    } else {
        // We got a timeout
        return DEV_TIMEOUT;
    }
}

/**
 * Initilialize the MIDI port
 *
 * @return 0 on success, any other number is an error
 */
short midi_init() {
    unsigned char dummy;
    short result;

    print(0, "midi_init: ");

    // result = midi_command(0xFF);    /* Reset the MIDI port */
    // if (result != 0) {
    //     return result;
    // }

    // /* Wait for the ACK */
    // for (dummy = 0; dummy != 0xFE; ) {
    //     if (midi_can_read()) {
    //         dummy = *MIDI_DATA;
    //     } else {
    //         // There was a timeout
    //         print(0, "X\n");
    //         return DEV_TIMEOUT;
    //     }
    // }

    print(0, "1");

    result = midi_command(0x3F);    /* Switch the MIDI port into UART mode */
    if (result != 0) {
        return result;
    }

    // /* Wait for the ACK */
    // do {
    //     if (midi_can_read()) {
    //         dummy = *MIDI_DATA;
    //         print_hex_8(0, dummy);
    //         print(0, "\n");
    //     } else {
    //         // There was a timeout
    //         print(0, "X\n");
    //         return DEV_TIMEOUT;
    //     }
    // } while (dummy != 0xFE);

    print(0, "2\n");

    return 0;
}

/**
 * Send a byte to the MIDI port
 *
 * @param b the byte to send
 * @return 0 on success, any other number is an error
 */
short midi_put(unsigned char b) {
    if (midi_can_write()) {
        /* Send the byte */
        *MIDI_DATA = b;
        return 0;
    }

    // There was a timeout
    return DEV_TIMEOUT;
}

/**
 * Get a byte from the MIDI port
 *
 * @return the byte read
 */
char midi_get_poll() {
    if (midi_can_read()) {
        return *MIDI_DATA;
    } else {
        // There was a timeout
        return DEV_TIMEOUT;
    }
}

#endif
