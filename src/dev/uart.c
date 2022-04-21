/*
 * Definitions of the UART routines
 */

#include "log.h"
#include "uart_reg.h"
#include "dev/uart.h"

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
 * non-zero if there is data to read, 0 if there is no data.
 */
short uart_has_bytes(short uart) {
    volatile unsigned char * uart_base = uart_get_base(uart);

    if (uart_base) {
        if (uart_base[UART_LSR] & LSR_DATA_AVAIL) {
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
