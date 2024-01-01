/*
 * Definitions of the UART routines
 */

#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "log.h"
#include "uart_reg.h"
#include "dev/channel.h"
#include "dev/uart.h"
#include "simpleio.h"
#include "utilities.h"

/**
 * Return the UART index for the given channel device
 */
short cdev_to_uart(short dev) {
    return dev - CDEV_COM1;
}

volatile unsigned char * uart_get_base(short uart) {
    if (uart == 0) {
        return (volatile unsigned char *)UART1_BASE;
    } else {
        return (volatile unsigned char *)UART2_BASE;
    }
}

/*
 * Set the data transfer speed
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * bps_code = the UART_* code number for the bits-per-second
 */
void uart_setbps(short uart, unsigned short bps_code) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        /* Enable divisor latch */
        uart_base[UART_LCR] = uart_base[UART_LCR] | 0x80;

        /* Set the divisor */
        uart_base[UART_TRHB] = bps_code & 0xff;
        uart_base[UART_TRHB+1] = (bps_code >> 8) & 0xff;

        /* Disable divisor latch */
        uart_base[UART_LCR] = uart_base[UART_LCR] & 0x7F;
    }
}

/*
 * Set the line control register
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * lcr_bits = the LCR settings
 */
void uart_setlcr(short uart, unsigned char lcr_bits) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        uart_base[UART_LCR] = lcr_bits;  /* Set the LCR bits (without the DLL bit) */
        if (uart_base[UART_LCR] != lcr_bits) {
            DEBUG("LCR mismatched!");
        }
    }
}

/*
 * Initialize a UART
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 */
void uart_init(short uart) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        /* Default to 9600 bps */
        uart_setbps(uart, UART_9600);

        /* Set: no parity, 1 stop bit, 8 data bits */
        uart_setlcr(uart,  LCR_PARITY_NONE | LCR_STOPBIT_1 | LCR_DATABITS_8);

        /* Enable FIFO, set for 56 byte trigger level */
        uart_base[UART_FCR] = 0xC1;
    }
}

/*
 * Return true (non-zero) if the UART has data ready to read in its FIFO
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 *
 * Returns:
 * true if there is data to read, false if there is no data.
 */
bool uart_has_bytes(short uart) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    return uart_base && (uart_base[UART_LSR] & LSR_DATA_AVAIL) ? true : false;
}

/**
 * Return true (non-zero) if the UART transmit FIFO is not full
 *
 * @param uart the number of the UART: 0 for COM1, 1 for COM2
 * @return non-zero if the FIFO can accept a byte, 0 if it is full
 */
short uart_can_send(short uart) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        if (uart_base[UART_LSR] & LSR_XMIT_EMPTY) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

/*
 * Send a byte to the UART. Blocks until the FIFO is ready to recieve a byte.
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * b = the byte to send
 */
void uart_put(short uart, unsigned char b) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        unsigned char status = 0;

        /* Wait for the transmit buffer to be empty */
        do {
            status = uart_base[UART_LSR];
        } while ((status & LSR_XMIT_EMPTY) == 0);

        /* Send the byte */
        uart_base[UART_TRHB] = b;
    }
}

/*
 * Get a byte from the UART. Blocks until there is data to read.
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 *
 * Returns:
 * the byte read from the UART
 */
unsigned char uart_get(short uart) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        unsigned char status = 0;

        /* Wait for there to be data available */
        do {
            status = uart_base[UART_LSR];
        } while ((status & LSR_DATA_AVAIL) == 0);

        /* Return the byte */
        return uart_base[UART_TRHB];
    }
}

/**
 * Return the status of the UART
 *
 * @param channel the descriptor for the channel
 * @return the status of the device
 */
short uart_status(p_channel chan) {
    short status = 0;

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
short uart_open(p_channel chan, const uint8_t * spec, short mode) {
    unsigned short bps = 0, lcr = 0;
    char * saveptr;
    char spec_copy[80];
    char *token = 0;
    short i = 0;
    short bps_code = 0;
    short lcr_code = 0;

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
        switch (i) {
            case 300:
                bps_code = UART_300;
                break;

            case 1200:
                bps_code = UART_1200;
                break;

            case 2400:
                bps_code = UART_2400;
                break;

            case 4800:
                bps_code = UART_4800;
                break;

            case 9600:
                bps_code = UART_9600;
                break;

            case 19200:
                bps_code = UART_19200;
                break;

            case 38400:
                bps_code = UART_38400;
                break;

            case 57600:
                bps_code = UART_57600;
                break;

            case 115200:
                bps_code = UART_115200;
                break;

            default:
                return ERR_BAD_ARGUMENT;
        }
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
short uart_read_b(p_channel chan) {
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
short uart_read(p_channel chan, uint8_t * buffer, short size) {
    short i = 0, count = 0;
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
short uart_readline(p_channel chan, uint8_t * buffer, short size) {
    short i = 0, count = 0;
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
 * Write a single unsigned char to the UART
 *
 * @param channel the number of the channel
 * @param b the unsigned char to write
 * @return 0 on success, a negative value on error
 */
short uart_write_b(p_channel chan, unsigned char c) {
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
short uart_write(p_channel chan, const uint8_t * buffer, short size) {
    int i;
    for (i = 0; i < size; i++) {
        uart_put(cdev_to_uart(chan->dev), buffer[i]);
    }
    return 0;
}

/**
 * Install the UART driver
 */
short uart_install() {
    t_dev_chan dev;
    short result = 0;

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
