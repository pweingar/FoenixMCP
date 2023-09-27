/*
 * A simple collection of I/O functions the kernel will need often
 */

#include "log_level.h"
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_TRACE
#endif


#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include "syscalls.h"
#include "simpleio.h"
#include "dev/txt_screen.h"

/*
 * Print a character to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * c = the character to print
 */
void print_c(short channel, char c) {
    //sys_chan_write_b(channel, c);
    txt_put(channel, c);
}

/*
 * Print a string to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * message = the ASCII-Z string to print
 */
void print(short channel, const char * message) {
	TRACE1("print(%d,..)", channel);
	
    short ret = sys_chan_write(channel, (const unsigned char *)message, strlen(message));
    if (ret < 0)
        ERROR1("Error while printing: %d", ret);
}

/**
 * Print a message to the channel, translating certain characters to box drawing characters
 *
 * @param channel the number of the channel to print on
 * @param message the message to translate and print
 */
void print_box(short channel, const char * message) {
    const char * x;
    char c;
    for (x = message; *x; x++) {
        switch (*x) {
            case 0x1B:
                // ESC... print this and the next character
                sys_chan_write_b(channel, 0x1B);
                if (*(x+1) == '[') {
                    sys_chan_write_b(channel, '[');
                    x++;
                }
                break;

            case '{':
                // Upper-left angle
                c = 0xDA;
                break;

            case '}':
                // Upper-right angle
                c = 0xBF;
                break;

            case '[':
                // Lower-left angle
                c = 0xC0;
                break;

            case ']':
                // Lower-right angle
                c = 0xD9;
                break;

            case '-':
                // Horizontal
                c = 0xC4;
                break;

            case '|':
                // Vertical
                c = 0xB3;
                break;

            case '!':
                // T down
                c = 0xC2;
                break;

            case '@':
                // T up
                c = 0xC1;
                break;

            case '>':
                // T right
                c = 0xC3;
                break;

            case '<':
                // T left
                c = 0xB4;
                break;

            case '#':
                // Cross
                c = 0xC5;
                break;

            case '*':
                // Block
                c = 0xDB;
                break;

            default:
                c = *x;
                break;
        }

        sys_chan_write_b(channel, c);
    }
}

const unsigned char hex_digits[] = "0123456789ABCDEF";

/*
 * Print an 8-bit number as hex to a channel
 *
 * Inputs:
 * channel = the number of the channel
 * n = the number to print
 */
void print_hex_8(short channel, unsigned short x) {
    short digit;
    char number[3];

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
    char number[5];

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
void print_hex_32(short channel, unsigned long n) {
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
    short tens = (bcd >> 4) & 0x0f;
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

/*
 * Print a nice dump of a byte buffer to the channel
 *
 * Inputs:
 * channel = the number of the channel
 * buffer = the byte buffer to print
 * size = the number of bytes to print
 * labels = 0: none, 1: offset, 2: address
 */
void dump_buffer(short channel, const unsigned char * buffer, short size, short labels) {
    short i, j, ascii_idx;
    char ascii_buffer[17];
    unsigned char c;

    for (j = 0; j < 17; j++) {
        ascii_buffer[j] = 0;
    }

    for (i = 0; i < size; i++) {
        c = buffer[i];

        if (i % 16 == 0) {
            print(channel, " ");
            print(channel, ascii_buffer);
            print(channel, "\n");

            for (j = 0; j < 17; j++) {
                ascii_buffer[j] = 0;
            }

            ascii_idx = 0;

            if (labels == 1) {
                print_hex_16(channel, i);
                print(channel, ":");
            } else if (labels == 2) {
                print_hex_32(channel, (unsigned long)buffer + i);
                print(channel, ":");
            }
        }

        if (isgraph(c)) {
            ascii_buffer[ascii_idx++] = c;
        } else {
            ascii_buffer[ascii_idx++] = '.';
        }

        if ((i % 8) == 0) {
            print(channel, " ");
        }

        print_hex_8(channel, c);
    }

    print(channel, " ");
    print(channel, ascii_buffer);
    print(channel, "\n");
}

/**
 * Convert a string to upper case
 *
 * @param src the string to convert
 * @param dst the buffer in which to copy the converted string
 */
void str_upcase(const char * src, char * dst) {
    int i;
    for (i = 0; i < strlen(src); i++) {
        dst[i] = toupper(src[i]);
    }
    dst[strlen(src)] = 0;
}

const char * model_banner[] = {
    // 0 = C256 FMX
    "   _________   ___________    ________  ____  __",
    "  / ____/__ \\ / ____/ ___/   / ____/  |/  / |/ /",
    " / /    __/ //___ \\/ __ \\   / /_  / /|_/ /|   / ",
    "/ /___ / __/____/ / /_/ /  / __/ / /  / //   |  ",
    "\\____//____/_____/\\____/  /_/   /_/  /_//_/|_|  ",

    // 1 = C256 U
    "   _________   ___________    __  __",
    "  / ____/__ \\ / ____/ ___/   / / / /",
    " / /    __/ //___ \\/ __ \\   / / / / ",
    "/ /___ / __/____/ / /_/ /  / /_/ /  ",
    "\\____//____/_____/\\____/   \\____/   ",

    // 2 = Blank
    "",
    "",
    "",
    "",
    "",

    // 3 = Blank
    "",
    "",
    "",
    "",
    "",

    // 4 = A2560 GENX
    "     GGGGGG  EEEEEEE NN    NN XX    XXX  3333333 2222222",
    "   GG       EE      NNN   NN   XX XXX        33      22 ",
    "  GG  GGGG EEEEE   NN NN NN    XXX       33333  22222   ",
    " GG    GG EE      NN   NNN  XXX  XX        33 22        ",
    " GGGGGG  EEEEEEE NN    NN XXX     XX 3333333 2222222    ",

    // 5 = C256 U+
    "   _________   ___________    __  __    ",
    "  / ____/__ \\ / ____/ ___/   / / / / __ ",
    " / /    __/ //___ \\/ __ \\   / / / /_/ /_",
    "/ /___ / __/____/ / /_/ /  / /_/ /_  __/",
    "\\____//____/_____/\\____/   \\____/ /_/   ",

    // 6 = A2560 U+
    "    ___   ___   ___________ ____     __  __    ",
    "   /   | |__ \\ / ____/ ___// __ \\   / / / / __ ",
    "  / /| | __/ //___ \\/ __ \\/ / / /  / / / /_/ /_",
    " / ___ |/ __/____/ / /_/ / /_/ /  / /_/ /_  __/",
    "/_/  |_/____/_____/\\____/\\____/   \\____/ /_/   ",

    // 7 = Blank
    "",
    "",
    "",
    "",
    "",

    // 8 = A2560 X
    //"    ___   ___   ___________ ____     _  __",
    //"   /   | |__ \\ / ____/ ___// __ \\   | |/ /",
    //"  / /| | __/ //___ \\/ __ \\/ / / /   |   / ",
    //" / ___ |/ __/____/ / /_/ / /_/ /   /   |  ",
    //"/_/  |_/____/_____/\\____/\\____/   /_/|_|  ",
    " AAA  22222 55555 6     00000 X   X",
    "A   A     2 5     6     0   0  X X ",
    "AAAAA 22222 55555 66666 0   0   X  ",
    "A   A 2         5 6   6 0   0  X X ",
    "A   A 22222 55555 66666 00000 X   X",

    // 9 = A2560 U
    "    ___   ___   ___________ ____     __  __",
    "   /   | |__ \\ / ____/ ___// __ \\   / / / /",
    "  / /| | __/ //___ \\/ __ \\/ / / /  / / / / ",
    " / ___ |/ __/____/ / /_/ / /_/ /  / /_/ /  ",
    "/_/  |_/____/_____/\\____/\\____/   \\____/   ",

    // 10 = Blank
    "",
    "",
    "",
    "",
    "",

    // 11 = A2560 K
    "    ___   ___   ___________ ____     __ __",
    "   /   | |__ \\ / ____/ ___// __ \\   / //_/",
    "  / /| | __/ //___ \\/ __ \\/ / / /  / ,<   ",
    " / ___ |/ __/____/ / /_/ / /_/ /  / /| |  ",
    "/_/  |_/____/_____/\\____/\\____/  /_/ |_|  "
};

/**
 * Print out the bannerized name of the model, given its ID
 *
 * @param channel the number of the channel to print on
 * @param model_id the number of the particular Foenix model
 */
void print_banner(short channel, short model_id) {
    const char * color_bars = "\x1b[31m\x0b\x0c\x1b[35m\x0b\x0c\x1b[33m\x0b\x0c\x1b[32m\x0b\x0c\x1b[36m\x0b\x0c\x1b[37m";
    const short lines_per_model = 5;
    const short max_model_id = 11;
    char line[80];

    if (model_id <= max_model_id) {
        short index = model_id * lines_per_model;
        int i = 0, j = 0;

        for (i = 0; i < lines_per_model; i++) {
            for (j = lines_per_model - i - 1; j > 0; j--) {
                sys_chan_write_b(channel, ' ');
            }
            print(channel, color_bars);
            for (j = 0; j < i; j++) {
                sys_chan_write_b(channel, ' ');
            }
            print(channel, model_banner[index+i]);
            sys_chan_write_b(channel, '\n');
        }
    }
}
