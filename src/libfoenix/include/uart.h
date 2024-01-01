/*
 * Declare low level routines for the UARTs
 */

#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Returns the address of the first register in the given UART
 *
 * @param uart the number of the UART 0 = COM1, 1 = COM2
 */
uint8_t * uart_get_base(int16_t uart);

/*
 * Set the data transfer speed
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * bps_code = the UART_* code number for the bits-per-second
 */
void uart_setbps(int16_t uart, uint16_t bps_code);

/*
 * Set the line control register
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * lcr_bits = the LCR settings
 */
void uart_setlcr(int16_t uart, uint8_t lcr_bits);

/*
 * Initialize a UART
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 */
void uart_init(int16_t uart);

/*
 * Return true (non-zero) if the UART has data ready to read in its FIFO
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 *
 * Returns:
 * non-zero if there is data to read, 0 if there is no data.
 */
bool uart_has_bytes(int16_t uart);

/*
 * Send a byte to the UART. Blocks until the FIFO is ready to recieve a byte.
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 * b = the byte to send
 */
void uart_put(int16_t uart, uint8_t b);

/*
 * Get a byte from the UART. Blocks until there is data to read.
 *
 * Inputs:
 * uart = the number of the UART: 0 for COM1, 1 for COM2
 *
 * Returns:
 * the byte read from the UART
 */
uint8_t uart_get(int16_t uart);

/**
 * Compute the BitsPerSecond (bps) code for the desired baud speed
 * 
 * @return the value that can be used with _set_bps, or 0 if invalid argument
 */
uint32_t uart_baud_to_code(uint16_t i);

/** Indicates whether the uart is ready to send */
int16_t uart_can_send(int16_t uart);

#endif
