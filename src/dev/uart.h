/*
 * Declare low level routines for the UARTs
 */

#ifndef __UART_H
#define __UART_H

/**
 * Returns the address of the first register in the given UART
 *
 * @param uart the number of the UART 0 = COM1, 1 = COM2
 */
extern volatile unsigned char * uart_get_base(short uart);

/*
 * Set the data transfer speed
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * bps_code = the UART_* code number for the bits-per-second
 */
extern void uart_setbps(short uart, unsigned short bps_code);

/*
 * Set the line control register
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * lcr_bits = the LCR settings
 */
extern void uart_setlcr(short uart, unsigned char lcr_bits);

/*
 * Initialize a UART
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 */
extern void uart_init(short uart);

/*
 * Return true (non-zero) if the UART has data ready to read in its FIFO
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 *
 * Returns:
 * non-zero if there is data to read, 0 if there is no data.
 */
extern short uart_has_bytes(short uart);

/*
 * Send a byte to the UART. Blocks until the FIFO is ready to recieve a byte.
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * b = the byte to send
 */
extern void uart_put(short uart, unsigned char b);

/*
 * Get a byte from the UART. Blocks until there is data to read.
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 *
 * Returns:
 * the byte read from the UART
 */
extern unsigned char uart_get(short uart);
#endif
