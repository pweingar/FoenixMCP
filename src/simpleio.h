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

#endif
