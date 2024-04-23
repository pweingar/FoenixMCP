/* MIDI device for MCP */

#include <stdint.h>
#include "channel.h"
#include "errors.h"
#include "log.h"
#include "libfoenix/include/midi.h"

static int16_t midi_open(p_channel chan, const uint8_t * path, int16_t mode);
static int16_t midi_write_b(p_channel chan, const uint8_t b);
static int16_t write(p_channel chan, const uint8_t * buffer, int16_t size);
static int16_t read(p_channel chan, uint8_t * buffer, int16_t size);
static int16_t midi_read_b(p_channel chan);
static int16_t midi_status(p_channel chan);

/**
 * Install the driver for the MIDI port
 */
int16_t midi_install() {
    int16_t result;
    t_dev_chan dev;

    dev.name = "MIDI";
    dev.number = CDEV_MIDI;
    dev.init = midi_init;
    dev.open = midi_open;
    dev.close = 0;
    dev.read = read;
    dev.readline = 0;
    dev.read_b = midi_read_b;
    dev.write = write;
    dev.write_b = midi_write_b;
    dev.flush = 0;
    dev.seek = 0;
    dev.status = midi_status;
    dev.ioctrl = 0;

    return cdev_register(&dev);
}

static int16_t midi_open(p_channel chan, const uint8_t * path, int16_t mode) {
    DEBUG("midi_open");
    return midi_init();
}

/**
 * Send a byte to the MIDI port
 *
 * @param b the byte to send
 * @return 0 on success, any other number is an error
 */
static int16_t midi_write_b(p_channel chan, const uint8_t b) {
    if (midi_wait_can_write()) {
        /* Send the byte */
        midi_write(b);
        return E_OK;
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
static int16_t write(p_channel chan, const uint8_t * buffer, int16_t size) {
    int16_t i = 0;
    int16_t result = 0;

    for (i = 0; i < size; i++) {
        result = midi_write_b(chan, buffer[i]);
        if (result) {
            return result;
        }
    }

    return E_OK;
}

/**
 * Read a number of bytes from the MIDI port into the buffer
 *
 * @param chan the channel record (ignored)
 * @param buffer the byte buffer into which to store the read data
 * @size the number of bytes to read
 */
static int16_t read(p_channel chan, uint8_t * buffer, int16_t size) {
    int16_t i = 0;
    int16_t result = 0;

    for (i = 0; i < size; i++) {
        DEBUG("*");
        result = midi_read_b(chan);
        if (result < 0) {
            return result;
        } else {
            buffer[i] = (uint8_t)(result & 0xff);
        }
    }

    return size;
}

/**
 * Get a byte from the MIDI port
 *
 * @param chan the channel record (ignored)
 * @return the byte read, negative on error
 */
static int16_t midi_read_b(p_channel chan) {
    if (midi_wait_can_read()) {
        return ((int16_t)midi_read() & 0x00ff);
    } else {
        // There was a timeout
        return DEV_TIMEOUT;
    }
}

/**
 * Return the status of the MIDI port
 *
 * @param chan the channel record (ignored)
 * @return the channel status flags
 */
static int16_t midi_status(p_channel chan) {
    int16_t status = 0;

    if (midi_wait_can_write())
        status |= CDEV_STAT_WRITABLE;

    if (midi_can_read())
        status |= CDEV_STAT_READABLE;

    return status;
}