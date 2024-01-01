/* COM ports (serial) devices for MCP */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "log.h"
#include "utilities.h"
#include "dev/channel.h"
#include "libfoenix/include/uart.h"
#include "libfoenix/include/uart_reg.h" // FIXME we shouldn't depend on it

static int16_t uart_open(p_channel chan, const uint8_t * spec, int16_t mode);
static int16_t uart_read(p_channel chan, uint8_t * buffer, int16_t size);
static int16_t uart_readline(p_channel chan, uint8_t * buffer, int16_t size);
static int16_t uart_read_b(p_channel chan);
static int16_t uart_write(p_channel chan, const uint8_t * buffer, int16_t size);
static int16_t uart_write_b(p_channel chan, uint8_t c);
static int16_t uart_status(p_channel chan);
static int16_t cdev_to_uart(int16_t dev);

/**
 * Install the UART driver
 */
int16_t uart_install() {
    t_dev_chan dev;
    int16_t result = 0;

    dev.name = "COM1";
    dev.number = CDEV_COM1;
    dev.init = 0;
    dev.open = uart_open;
    dev.close = 0;
    dev.read = uart_read;
    dev.readline = uart_readline;
    dev.read_b = uart_read_b;
    dev.write = uart_write;
    dev.write_b = uart_write_b;
    dev.flush = 0;
    dev.seek = 0;
    dev.status = uart_status;
    dev.ioctrl = 0;

    result = cdev_register(&dev);

    if (result != E_OK) {
        return result;
    }

    dev.name = "COM2";
    dev.number = CDEV_COM2;

    result = cdev_register(&dev);

    return result;
}

/**
 * Open a serial connection... allow specification for bps, data size, etc.
 *
 * The specification provided to open is a string of four comma separated values:
 * 1. Bits-per-second: 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
 * 2. Number of data bits: 5, 6, 7, 8
 * 3. Stop bits: 1, or 2
 * 4. Parity: NONE, ODD, EVEN, MARK, or SPACE
 *
 * @param chan the channel descriptor to open
 * @param spec a string describing the connection to open: <bps>,<data bits>,<stop bits>,<parity Y/N>
 * @param mode an unused parameter
 * @return 0 on success, any other number is an error
 */
static int16_t uart_open(p_channel chan, const uint8_t * spec, int16_t mode) {
    uint16_t bps = 0, lcr = 0;
    char * saveptr;
    char spec_copy[80];
    char *token = 0;
    int16_t i = 0;
    int16_t bps_code = 0;
    int16_t lcr_code = 0;

    /* Do some basic initialization of the UART */
    uart_init(cdev_to_uart(chan->dev));

    // Make a local copy of the specification so we can tokenize it
    strncpy(spec_copy, spec, 80);

    // Get the first token
    saveptr = spec_copy;
    token = strtok_r(spec_copy, ",", &saveptr);
    if (token) {
        // Parse the bit rate token
        i = atoi(token);
        bps_code = uart_baud_to_code(atoi(token));
        if (bps_code == 0)
            return ERR_BAD_ARGUMENT;
        uart_setbps(cdev_to_uart(chan->dev), bps_code);

        // Get the next token
        token = strtok_r(spec_copy, ",", &saveptr);
        if (token) {
            // Parse the data bit count
            i = atoi(token);
            switch (i) {
                case 5:
                    lcr_code = LCR_DATABITS_5;
                    break;

                case 6:
                    lcr_code = LCR_DATABITS_6;
                    break;

                case 7:
                    lcr_code = LCR_DATABITS_7;
                    break;

                case 8:
                    lcr_code = LCR_DATABITS_8;
                    break;

                default:
                    logmsg(LOG_ERROR, "uart_open: Bad data word length");
                    return ERR_BAD_ARGUMENT;
            }

            // Get the next token
            token = strtok_r(spec_copy, ",", &saveptr);
            if (token) {
                // Parse the stop bit count (1 or 2)
                i = atoi(token);
                if (i == 1) {
                    lcr_code |= LCR_STOPBIT_1;
                } else if (i == 2) {
                    lcr_code |= LCR_STOPBIT_2;
                } else {
                    logmsg(LOG_ERROR, "uart_open: Bad stop bits");
                    return ERR_BAD_ARGUMENT;
                }

                // Get the next token
                token = strtok_r(spec_copy, ",", &saveptr);
                if (token) {
                    // NONE, ODD, EVEN, MARK, or SPACE
                    if (strcmp(token, "NONE") == 0) {
                        lcr_code |= LCR_PARITY_NONE;
                    } else if (strcmp(token, "ODD") == 0) {
                        lcr_code |= LCR_PARITY_ODD;
                    } else if (strcmp(token, "EVEN") == 0) {
                        lcr_code |= LCR_PARITY_EVEN;
                    } else if (strcmp(token, "MARK") == 0) {
                        lcr_code |= LCR_PARITY_MARK;
                    } else if (strcmp(token, "SPACE") == 0) {
                        lcr_code |= LCR_PARITY_SPACE;
                    } else {
                        logmsg(LOG_ERROR, "uart_open: Bad parity");
                        return ERR_BAD_ARGUMENT;
                    }

                    // Set word length, stop bit size, and parity
                    uart_setlcr(cdev_to_uart(chan->dev),  lcr_code);
                    return 0;

                } else {
                    logmsg(LOG_ERROR, "uart_open: no parity token");
                    return ERR_BAD_ARGUMENT;
                }
            } else {
                logmsg(LOG_ERROR, "uart_open: no stop bit token");
                return ERR_BAD_ARGUMENT;
            }
        } else {
            logmsg(LOG_ERROR, "uart_open: no data length token");
            return ERR_BAD_ARGUMENT;
        }
    } else {
        logmsg(LOG_ERROR, "uart_open: no BPS token");
        return ERR_BAD_ARGUMENT;
    }

    return 0;
}

/*
 * Read a single char from the UART
 *
 * @param channel the number of the channel
 * @return the value read (if negative, error)
 */
static int16_t uart_read_b(p_channel chan) {
    return uart_get(cdev_to_uart(chan->dev));
}

/**
 * Read bytes from the UART
 *
 * @param channel the number of the channel
 * @param buffer the buffer into which to copy the channel data
 * @param size the size of the buffer.
 * @return number of bytes read, any negative number is an error code
 */
static int16_t uart_read(p_channel chan, uint8_t * buffer, int16_t size) {
    int16_t i = 0, count = 0;
    for (i = 0; i < size; i++) {
        buffer[i] = uart_get(cdev_to_uart(chan->dev));
        count++;
    }

    return count;
}

/**
 * Read a line from the UART
 *
 * @param channel the number of the channel
 * @param buffer the buffer into which to copy the channel data
 * @param size the size of the buffer.
 * @returns number of bytes read, any negative number is an error code
 */
static int16_t uart_readline(p_channel chan, uint8_t * buffer, int16_t size) {
    int16_t i = 0, count = 0;
    for (i = 0; i < size; i++) {
        char c = uart_get(cdev_to_uart(chan->dev));
        if ((c == '\n') || (c == '\r')) {
            break;
        }
        buffer[i] = c;
        count++;
    }

    return count;
}

/**
 * Write a single uint8_t to the UART
 *
 * @param channel the number of the channel
 * @param b the uint8_t to write
 * @return 0 on success, a negative value on error
 */
static int16_t uart_write_b(p_channel chan, uint8_t c) {
    uart_put(cdev_to_uart(chan->dev), c);
    return 0;
}

/**
 * Write a bytes to the UART
 *
 * @param channel the number of the channel
 * @param buffer the buffer containing the data to write
 * @param size the size of the buffer.
 * @return number of bytes written, any negative number is an error code
 */
static int16_t uart_write(p_channel chan, const uint8_t * buffer, int16_t size) {
    int i;
    for (i = 0; i < size; i++) {
        uart_put(cdev_to_uart(chan->dev), buffer[i]);
    }
    return 0;
}

/**
 * Return the status of the UART
 *
 * @param channel the descriptor for the channel
 * @return the status of the device
 */
static int16_t uart_status(p_channel chan) {
    int16_t status = 0;

    // Check if there is data to be read
    if (uart_has_bytes(cdev_to_uart(chan->dev))) {
        status |= CDEV_STAT_READABLE;
    }

    // Check if the FIFO is full
    if (uart_can_send(cdev_to_uart(chan->dev))) {
        status |= CDEV_STAT_WRITABLE;
    }

    // TODO: reflect error bits?

    return status;
}

/**
 * Return the UART index for the given channel device
 */
static int16_t cdev_to_uart(int16_t dev) {
    return dev - CDEV_COM1;
}