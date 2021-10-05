/*
 * A simple collection of I/O functions the kernel will need often
 */

#include <string.h>
#include "syscalls.h"
#include "simpleio.h"

/*
 * Print a character to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * c = the character to print
 */
void print_c(short channel, char c) {
    //sys_chan_write_b(channel, c);
    text_put_raw(channel, c);
}

/*
 * Print a string to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * message = the ASCII-Z string to print
 */
void print(short channel, char * message) {
    int i;
    for (i = 0; i < strlen(message); i++) {
        print_c(channel, message[i]);
    }
    // sys_chan_write(channel, message, strlen(message));
}

unsigned char number[5];
unsigned char hex_digits[] = "0123456789ABCDEF";

/*
 * Print an 8-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
void print_hex_8(short channel, unsigned short x) {
    short digit;

    digit = (x & 0xf0) >> 4;
    number[0] = hex_digits[digit];

    digit = (x & 0x0f);
    number[1] = hex_digits[digit];

    number[2] = 0;

    print(channel, number);
}

/*
 * Print an 16-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
void print_hex_16(short channel, unsigned short x) {
    short digit;

    digit = (x >> 12) & 0x000f;
    number[0] = hex_digits[digit];

    digit = (x >> 8) & 0x000f;
    number[1] = hex_digits[digit];

    digit = (x >> 4) & 0x000f;
    number[2] = hex_digits[digit];

    digit = x & 0x000f;
    number[3] = hex_digits[digit];

    number[4] = 0;

    print(channel, number);
}

/*
 * Print an 32-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
void print_hex_32(short channel, long n) {
    char number[9];
    short digit;
    short i;

    for (i = 0; i < 8; i++) {
        number[7 - i] = hex_digits[n & 0x0f];
        n = n >> 4;
    }
    number[8] = 0;

    print(channel, number);
}

/*
 * Convert a BCD byte to an integer
 *
 * Inputs:
 * bcd = a byte containing a BCD number
 *
 * Returns:
 * the binary form of the number
 */
unsigned short bcd_to_i(unsigned char bcd) {
    short tens = (bcd >> 4) & 0xf0;
    short ones = bcd & 0x0f;

    if ((ones > 9) || (tens > 9)) {
        /* Byte was not in BCD... just return a 0 */
        return 0;
    } else {
        return tens * 10 + ones;
    }
}

/*
 * Convert a number from 0 to 99 to BCD
 *
 * Inputs:
 * n = a binary number from 0 to 99
 *
 * Returns:
 * a byte containing n as a BCD number
 */
unsigned char i_to_bcd(unsigned short n) {
    if (n > 99) {
        /* Input was out of range... just return 0 */
        return 0;

    } else {
        unsigned short tens = n / 10;
        unsigned short ones = n - (tens * 10);

        return tens << 4 | ones;
    }
}
