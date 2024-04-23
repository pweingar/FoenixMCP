/*
 * Definitions for the MIDI ports
 */

#include <stdint.h>

#include "features.h"
#if HAS_MIDI_PORTS

#include "midi_reg.h"
#include "libfoenix/include/midi.h"
#include "errors.h"
#include "simpleio.h"
#include "rtc.h"


/** Timeout for waiting on the MIDI interface */
const long midi_timeout = 60;

static int16_t midi_command(uint8_t cmd);

/**
 * Initilialize the MIDI port
 *
 * @return 0 on success, any other number is an error
 */
int16_t midi_init() {
    uint8_t dummy;
    int16_t result;

    result = midi_command(0xFF);    /* Reset the MIDI port */
    if (result != 0) {
        return result;
    }

    result = midi_command(0x3F);    /* Switch the MIDI port of the SuperIO into UART mode */
    if (result != 0) {
        return result;
    }

    /* Wait for the ACK */
    do {
        if (midi_wait_can_read()) {
            dummy = *MIDI_DATA;
        } else {
            return DEV_TIMEOUT;
        }
    } while (dummy != 0xFE);

    return E_OK;
}

/**
 * Wait for data to be ready to read...
 *
 * @return 1 on success, 0 if there is a timeout
 */
int16_t midi_wait_can_read() {
    long target = rtc_get_jiffies() + midi_timeout;
    do {
        if ((*MIDI_STAT & MIDI_STAT_RX_EMPTY) == 0) {
            // There is data waiting
            return 1;
        }
    } while (target > rtc_get_jiffies());

    /* Time out */
    return 0;
}

/**
 * Wait for the MIDI transmiter to be empty...
 *
 * @return 1 on success, 0 if there is a timeout
 */
int16_t midi_wait_can_write() {
    long target = rtc_get_jiffies() + midi_timeout;
    do {
        if (midi_can_write()) {
            return 1;
        }
    } while (target > rtc_get_jiffies());

    /* Time out */
    return 0;
}

/**
 * Send a command to the MPU-401
 */
static int16_t midi_command(uint8_t cmd) {
    *MIDI_CMD = cmd;
    return E_OK;
}

/**
 * Returns non-0 if data has been received
 */
int16_t midi_can_read() {
    return !(*MIDI_STAT & MIDI_STAT_RX_EMPTY);
}

/**
 * Returns non-0 if ready to send
 */
int16_t midi_can_write() {
    return ~(*MIDI_STAT & MIDI_STAT_TX_BUSY);
}

void midi_write(uint8_t b) {
    *MIDI_DATA = b;
}

uint8_t midi_read() {
    return *MIDI_DATA;
}

#endif
