/*
 * Definitions for the MIDI ports
 */

#include <stdint.h>

#include "features.h"
#include "midi_reg.h"
#include "dev/channel.h"
#include "libfoenix/include/midi.h"
#include "errors.h"
#include "simpleio.h"
#include "sys_general.h"
#include "timers.h"

#if HAS_MIDI_PORTS

/** Timeout for waiting on the MIDI interface */
const long midi_timeout = 60;

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
        if ((*MIDI_STAT & MIDI_STAT_TX_BUSY) != 0) {
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
    /* Send the byte */
    *MIDI_CMD = cmd;
    return 0;
}

/**
 * Initilialize the MIDI port
 *
 * @return 0 on success, any other number is an error
 */
short midi_init() {
    unsigned char dummy;
    short result;

    result = midi_command(0xFF);    /* Reset the MIDI port */
    if (result != 0) {
        return result;
    }

    result = midi_command(0x3F);    /* Switch the MIDI port into UART mode */
    if (result != 0) {
        return result;
    }

    /* Wait for the ACK */
    do {
        if (midi_can_read()) {
            dummy = *MIDI_DATA;
        } else {
            // There was a timeout
            return DEV_TIMEOUT;
        }
    } while (dummy != 0xFE);

    return 0;
}

/**
 * Send a byte to the MIDI port
 *
 * @param b the byte to send
 * @return 0 on success, any other number is an error
 */
short midi_write_b(p_channel chan, const unsigned char b) {
    if (midi_can_write()) {
        /* Send the byte */
        *MIDI_DATA = b;
        return 0;
    } else {
        // There was a timeout
        return DEV_TIMEOUT;
    }
}

/**
 * Write a buffer's worth of bytes to the MIDI port
 *
 * @param chan the channel record (ignored)
 * @param buffer the array of bytes to send
 * @param size the number of bytes to send
 */
short midi_write(p_channel chan, const unsigned char * buffer, short size) {
    short i = 0;
    short result = 0;

    for (i = 0; i < size; i++) {
        result = midi_write_b(chan, buffer[i]);
        if (result) {
            return result;
        }
    }

    return 0;
}

/**
 * Get a byte from the MIDI port
 *
 * @param chan the channel record (ignored)
 * @return the byte read, negative on error
 */
short midi_read_b(p_channel chan) {
    if (midi_can_read()) {
        return ((short)*MIDI_DATA & 0x00ff);
    } else {
        // There was a timeout
        return DEV_TIMEOUT;
    }
}

/**
 * Read a number of bytes from the MIDI port into the buffer
 *
 * @param chan the channel record (ignored)
 * @param buffer the byte buffer into which to store the read data
 * @size the number of bytes to read
 */
short midi_read(p_channel chan, unsigned char * buffer, short size) {
    short i = 0;
    short result = 0;

    for (i = 0; i < size; i++) {
        result = midi_read_b(chan);
        if (result < 0) {
            return result;
        } else {
            buffer[i] = (unsigned char)(result & 0xff);
        }
    }

    return size;
}

/**
 * Return the status of the MIDI port
 *
 * @param chan the channel record (ignored)
 * @return the channel status flags
 */
short midi_status(p_channel chan) {
    short status = 0;
    unsigned char midi_stat_value = *MIDI_STAT;

    if ((midi_stat_value & MIDI_STAT_TX_BUSY) != 0) {
        status |= CDEV_STAT_WRITABLE;
    }

    if ((midi_stat_value & MIDI_STAT_RX_EMPTY) == 0) {
        status |= CDEV_STAT_READABLE;
    }

    return status;
}

short midi_open(p_channel chan, const char * path, short mode) {
    return midi_init();
}

/**
 * Install the driver for the MIDI port
 */
short midi_install() {
    short result;
    t_dev_chan dev;

    dev.name = "MIDI";
    dev.number = CDEV_MIDI;
    dev.init = midi_init;
    dev.open = midi_open;
    dev.close = 0;
    dev.read = midi_read;
    dev.readline = 0;
    dev.read_b = midi_read_b;
    dev.write = midi_write;
    dev.write_b = midi_write_b;
    dev.flush = 0;
    dev.seek = 0;
    dev.status = midi_status;
    dev.ioctrl = 0;

    return cdev_register(&dev);
}

#endif
