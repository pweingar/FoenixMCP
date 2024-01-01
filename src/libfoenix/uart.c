/*
 * Definitions of the UART routines
 */

#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "log.h"
#include "uart_reg.h"
#include "dev/channel.h"
#include "libfoenix/include/uart.h"
#include "simpleio.h"
#include "utilities.h"


/*
 * Initialize a UART
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 */
void uart_init(int16_t uart) {
    volatile uint8_t * uart_base = uart_get_base(uart);

    if (uart_base) {
        /* Default to 9600 bps */
        uart_setbps(uart, UART_9600);

        /* Set: no parity, 1 stop bit, 8 data bits */
        uart_setlcr(uart,  LCR_PARITY_NONE | LCR_STOPBIT_1 | LCR_DATABITS_8);

        /* Enable FIFO, set for 56 byte trigger level */
        uart_base[UART_FCR] = 0xC1;
    }
}

uint8_t * uart_get_base(int16_t uart) {
    if (uart == 0) {
        return (uint8_t *)UART1_BASE;
    } else {
        return (uint8_t *)UART2_BASE;
    }
}

/**
 * Compute the BitsPerSecond (bps) code for the desired baud speed
 * 
 * @return the value that can be used with _set_bps, or 0 if invalid argument
 */
uint32_t uart_baud_to_code(uint16_t i) {
    switch (i) {
    case 300:  return UART_300;
    case 1200: return UART_1200;
    case 2400: return UART_2400;
    case 4800: return UART_4800;
    case 9600: return UART_9600;
    case 19200: return UART_19200;
    case 38400: return UART_38400;
    case 57600: return UART_57600;
    case 115200: return UART_115200;
    default:
        return 0;
    }
}

/*
 * Set the data transfer speed
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * bps_code = the UART_* code number for the bits-per-second
 */
void uart_setbps(int16_t uart, uint16_t bps_code) {
    volatile uint8_t * uart_base = uart_get_base(uart);

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
void uart_setlcr(int16_t uart, uint8_t lcr_bits) {
    volatile uint8_t * uart_base = uart_get_base(uart);

    if (uart_base) {
        uart_base[UART_LCR] = lcr_bits;  /* Set the LCR bits (without the DLL bit) */
        if (uart_base[UART_LCR] != lcr_bits) {
            DEBUG("LCR mismatched!");
        }
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
bool uart_has_bytes(int16_t uart) {
    volatile uint8_t * uart_base = uart_get_base(uart);

    return uart_base && (uart_base[UART_LSR] & LSR_DATA_AVAIL) ? true : false;
}

/**
 * Return true (non-zero) if the UART transmit FIFO is not full
 *
 * @param uart the number of the UART: 0 for COM1, 1 for COM2
 * @return non-zero if the FIFO can accept a byte, 0 if it is full
 */
int16_t uart_can_send(int16_t uart) {
    volatile uint8_t * uart_base = uart_get_base(uart);

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
void uart_put(int16_t uart, uint8_t b) {
    volatile uint8_t * uart_base = uart_get_base(uart);

    if (uart_base) {
        uint8_t status = 0;

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
uint8_t uart_get(int16_t uart) {
    volatile uint8_t * uart_base = uart_get_base(uart);

    if (uart_base) {
        uint8_t status = 0;

        /* Wait for there to be data available */
        do {
            status = uart_base[UART_LSR];
        } while ((status & LSR_DATA_AVAIL) == 0);

        /* Return the byte */
        return uart_base[UART_TRHB];
    }
}



