/*
 * A simple collection of I/O functions the kernel will need often
 */

#ifndef __SIMPLE_IO_H
#define __SIMPLE_IO_H

/*
 * Print a string to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * message = the ASCII-Z string to print
 */
extern void print(short channel, char * message);

/*
 * Print a character to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * c = the character to print
 */
extern void print_c(short channel, char c);

/*
 * Print an 8-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
extern void print_hex_8(short channel, short n);

/*
 * Print an 16-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
extern void print_hex_16(short channel, short n);

/*
 * Print an 32-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
extern void print_hex_32(short channel, long n);

/*
 * Convert a BCD byte to an integer
 *
 * Inputs:
 * bcd = a byte containing a BCD number
 *
 * Returns:
 * the binary form of the number
 */
extern unsigned short bcd_to_i(unsigned char bcd);

/*
 * Convert a number from 0 to 99 to BCD
 *
 * Inputs:
 * n = a binary number from 0 to 99
 *
 * Returns:
 * a byte containing n as a BCD number
 */
extern unsigned char i_to_bcd(unsigned short n);

/*
 * Print a nice dump of a byte buffer to the channel
 *
 * Inputs:
 * channel = the number of the channel
 * buffer = the byte buffer to print
 * size = the number of bytes to print
 * labels = 0: none, 1: offset, 2: address
 */
extern void dump_buffer(short channel, unsigned char * buffer, short size, short labels);

#endif
