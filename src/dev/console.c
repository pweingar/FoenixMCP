/**
 * Implementation of the console channel device
 *
 * The console maps to the main screen and keyboard.
 *
 */

#include <ctype.h>
#include <string.h>
#include "log.h"
#include "types.h"
#include "constants.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/ps2.h"
#include "dev/kbd_mo.h"
#include "dev/text_screen_iii.h"

#define ANSI_BUFFER_SIZE    16
#define MAX_ANSI_ARGS       10

#define CON_CTRL_ANSI       0x80            /* Set to enable ANSI escape processing */

typedef void (*ansi_handler)(p_channel, short, short[]);

/*
 * Structure to map an ANSI escape sequence pattern to a handler
 */
typedef struct s_ansi_seq {
    char * pattern;
    ansi_handler handler;
} t_ansi_seq, *p_ansi_seq;

/*
 * Structure to track console state
 */
typedef struct s_console_data {
    unsigned char control;              /* Control flags for the console: e.g. process ANSI codes */
    unsigned char ansi_buffer_count;    /* Number of characters in the ANSI BUFFER */
    char ansi_buffer[ANSI_BUFFER_SIZE]; /* Used to keep track of characters in the ANSI escape sequences */
} t_console_data, *p_console_data;

/*
 * Forwards
 */

extern void ansi_cuu(p_channel chan, short arg_count, short args[]);
extern void ansi_cuf(p_channel chan, short arg_count, short args[]);
extern void ansi_cub(p_channel chan, short arg_count, short args[]);
extern void ansi_cud(p_channel chan, short arg_count, short args[]);
extern void ansi_cup(p_channel chan, short arg_count, short args[]);
extern void ansi_ed(p_channel chan, short arg_count, short args[]);
extern void ansi_el(p_channel chan, short arg_count, short args[]);
extern void ansi_ich(p_channel chan, short arg_count, short args[]);
extern void ansi_dch(p_channel chan, short arg_count, short args[]);

/*
 * Console variables and constants
 */

/*
 * ANSI escape sequences:
 * # is a placeholder for any number of numerals
 *
 */
const t_ansi_seq ansi_sequence[] = {
    { "\x1B[H", ansi_cup },
    { "\x1B[#A", ansi_cuu },
    { "\x1B[#B", ansi_cuf },
    { "\x1B[#C", ansi_cub },
    { "\x1B[#D", ansi_cud },
    { "\x1B[#J", ansi_ed },
    { "\x1B[#K", ansi_el },
    { "\x1B[#@]", ansi_ich },
    { "\x1B[#P]", ansi_dch },
    { "\x1B[#;#H", ansi_cup },
    { 0, 0 }
};

/*
 * ANSI Handler: cursor up
 */
void ansi_cuu(p_channel chan, short arg_count, short args[]) {
    unsigned short x, y;
    short delta = 1;

    TRACE("ansi_cuu");

    if (arg_count > 0) {
        delta = args[0];
    }

    if (delta == 0) delta = 1;

    text_get_xy(chan->dev, &x, &y);
    y -= delta;
    text_set_xy(chan->dev, x, y);
}

/*
 * ANSI Handler: cursor forward
 */
void ansi_cuf(p_channel chan, short arg_count, short args[]) {
    unsigned short x, y;
    short delta = 1;

    TRACE("ansi_cuf");

    if (arg_count > 0) {
        delta = args[0];
    }

    if (delta == 0) delta = 1;

    text_get_xy(chan->dev, &x, &y);
    x += delta;
    text_set_xy(chan->dev, x, y);
}

/*
 * ANSI Handler: cursor back
 */
void ansi_cub(p_channel chan, short arg_count, short args[]) {
    unsigned short x, y;
    short delta = 1;

    TRACE("ansi_cub");

    if (arg_count > 0) {
        delta = args[0];
    }

    if (delta == 0) delta = 1;

    text_get_xy(chan->dev, &x, &y);
    x -= delta;
    text_set_xy(chan->dev, x, y);
}

/*
 * ANSI Handler: cursor down
 */
void ansi_cud(p_channel chan, short arg_count, short args[]) {
    unsigned short x, y;
    short delta = 1;

    TRACE("ansi_cud");

    if (arg_count > 0) {
        delta = args[0];
    }

    if (delta == 0) delta = 1;

    text_get_xy(chan->dev, &x, &y);
    y += delta;
    text_set_xy(chan->dev, x, y);
}

/*
 * ANSI Handler: cursor position
 */
void ansi_cup(p_channel chan, short arg_count, short args[]) {
    unsigned short x = 1;
    unsigned short y = 1;

    TRACE("ansi_cup");

    if (arg_count > 0) {
        x = args[0];
        if (arg_count > 1) {
            y = args[1];
        }
    }

    if (x == 0) x = 1;
    if (y == 0) y = 1;

    text_set_xy(chan->dev, x - 1, y - 1);
}

/*
 * ANSI Handler: erase in display
 */
void ansi_ed(p_channel chan, short arg_count, short args[]) {
    unsigned short n = 2;

    TRACE("ansi_ed");

    if (arg_count > 0) {
        n = args[0];
    }

    text_clear(chan->dev, n);
}

/*
 * ANSI Handler: erase in line
 */
void ansi_el(p_channel chan, short arg_count, short args[]) {
    unsigned short n = 2;

    TRACE("ansi_el");

    if (arg_count > 0) {
        n = args[0];
    }

    text_clear_line(chan->dev, n);
}

/*
 * ANSI Handler: insert a character
 */
void ansi_ich(p_channel chan, short arg_count, short args[]) {
    unsigned short n = 2;

    TRACE("ansi_ich");

    if (arg_count > 0) {
        n = args[0];
    }

    text_insert(chan->dev, n);
}

/*
 * ANSI Handler: delete a character
 */
void ansi_dch(p_channel chan, short arg_count, short args[]) {
    unsigned short n = 2;

    TRACE("ansi_dch");

    if (arg_count > 0) {
        n = args[0];
    }

    text_delete(chan->dev, n);
}

//
// Initialize the console... nothing needs to happen here
//
short con_init() {
    return 0;
}

/*
 * Open the consolde device for the given channel
 *
 * Inputs:
 * chan = the channel record for this console device
 * path = unused
 * mode = unused
 *
 * Returns
 * 0 on success, negative number on failure
 */
short con_open(p_channel chan, uint8_t * path, short mode) {
    int i;
    p_console_data con_data;

    TRACE("con_open");

    /* Initialize the console data for this channel */

    con_data = &(chan->data);
    con_data->control = CON_CTRL_ANSI;
    con_data->ansi_buffer_count = 0;
    for (i = 0; i < ANSI_BUFFER_SIZE; i++) {
        con_data->ansi_buffer[i] = 0;
    }

    return 0;
}

/*
 * Flush the output to the console...
 *
 * Really only does something if the console is set to process ANSI escape codes
 *
 */
short con_flush(p_channel chan) {
    int i;
    p_console_data con_data;

    con_data = &(chan->data);
    if (con_data->control & CON_CTRL_ANSI) {
        for (i = 0; i < con_data->ansi_buffer_count; i++) {
            text_put_raw(chan->dev, con_data->ansi_buffer[i]);
        }
    }
    return 0;
}

/*
 * Close the console channel... just flush the data
 *
 * Inputs:
 * chan = the channel record for this console device
 */
short con_close(p_channel chan) {
    con_flush(chan);
    return 0;
}

/*
 * Return true if a character is an initial character for an ANSI escape code
 *
 * Inputs:
 * c = the character to test
 *
 * Returns:
 * 0 if the character is not an ANSI initial, 1 if it is
 */
short ansi_start_code(char c) {
    switch (c) {
        case CHAR_ESC:
            return 1;

        default:
            return 0;
    }
}

/*
 * Attempt to match a pattern to what's in the buffer
 * Calls the sequence's handler if the pattern matches
 *
 * Inputs:
 * chan = pointer to the channel record
 * con_data = pointer to the console data record for this channel
 * sequence = pointer to the ANSI sequence mapping to check
 *
 * Returns
 * 0 if not a match, 1 if it is a match
 */
short ansi_match_pattern(p_channel chan, p_console_data con_data, p_ansi_seq sequence) {
    short arg_idx = 0;
    short arg[MAX_ANSI_ARGS];
    short i, j;
    char * pattern = sequence->pattern;
    char * buffer = con_data->ansi_buffer;
    short buffer_count = con_data->ansi_buffer_count;

    for (i = 0, j = 0; i < strlen(pattern); i++) {
        if (buffer[j] == pattern[i]) {
            /* Character matches... advance to the next character in the buffer and pattern */
            j++;

        } else if (pattern[i] == '#') {
            /* Parse a number of decimal digits */
            arg[arg_idx] = 0;
            if (buffer[j] == pattern[i+1]) {
                /* Parameter is missing... set to zero */
                arg_idx++;

            } else {
                while (isdigit(buffer[j]) && (j < buffer_count)) {
                    arg[arg_idx] = arg[arg_idx] * 10 + (buffer[j] - '0');
                    j++;
                }

                if (j == buffer_count) {
                    /* This pattern does not match */
                    return 0;
                } else {
                    /* We've read a number */
                    arg_idx++;
                }
            }
        } else {
            return 0;
        }
    }

    if (i == strlen(pattern)) {
        /* The pattern has been completely matched */

        /* Clear the buffer */
        for (i = 0; i < buffer_count; i++) {
            buffer[i] = 0;
        }
        con_data->ansi_buffer_count = 0;

        sequence->handler(chan, arg_idx, arg);
        return 1;
    }

    return 0;
}

/*
 * Attempt to match what's in the buffer to known ANSI escape sequences
 *
 * Inputs:
 * chan = pointer to the channel record
 * con_data = pointer to the console data record for this channel
 */
void ansi_match_buffer(p_channel chan, p_console_data con_data) {
    short arg[MAX_ANSI_ARGS];
    short arg_count = 0;
    int i;

    for (i = 0; i < MAX_ANSI_ARGS; i++) {
        arg[i] = 0;
    }

    for (i = 0; ansi_sequence[i].pattern; i++) {
        if (ansi_match_pattern(chan, con_data, &ansi_sequence[i])) {
            return;
        }
    }

    /* If not currently matching, but the buffer is full, we need to flush */
    if (con_data->ansi_buffer_count == ANSI_BUFFER_SIZE) {
        con_flush(chan);
    }
}

/*
 * Send a byte to the console screen
 */
short con_write_b(p_channel chan, uint8_t b) {
    p_console_data con_data;

    /* Check to see if we need to process ANSI codes */
    con_data = &(chan->data);
    if (con_data->control & CON_CTRL_ANSI) {
        /* ANSI codes are to be processed */
        if ((con_data->ansi_buffer_count > 0) || ansi_start_code(b)) {
            /* We're working on a sequence: add the character to the buffer */
            con_data->ansi_buffer[con_data->ansi_buffer_count++] = b;

            /* Attempt to match the buffer to an escape sequence */
            ansi_match_buffer(chan, con_data);

        } else {
            /* We're not working on a sequence: just print the character */
            text_put_raw(chan->dev, (char)b);
        }

    } else {
        /* Not processing ANSI codes... just pass it to the text driver */
        text_put_raw(chan->dev, (char)b);
    }
    return 0;
}

/*
 * Attempt to read from the keyboard.
 */
short con_read_b(p_channel chan) {
    char c;
    do {
#ifdef KBD_POLLED
        ps2_mouse_get_packet();
        c = kbdmo_getc_poll();
#else
        c = kbdmo_getc();
#endif
    } while (c == 0);

    // Echo the character to the screen
    con_write_b(chan, c);

    return c;
}


//
// Attempt to read a buffer's worth of bytes from the keyboard
//
short con_read(p_channel chan, uint8_t * buffer, short size) {
    int i;

    for (i = 0; i < size; i++) {
        short c = con_read_b(chan);
        if (c < 0) {
            return c;
        } else if (c > 0) {
            buffer[i] = (uint8_t)(c & 0xff);
        }
    }

    return i;
}

//
// Attempt to read a line of text from the keyboard (stops at buffer size or newline)
//
// This routine also allows for some basic line editing
//
short con_readline(p_channel chan, uint8_t * buffer, short size) {
    int i = 0;

    while (i < size - 1) {
        short c = con_read_b(chan);
        if (c < 0) {
            // Return the error, if we got one
            return c;

        } else if (c > 0) {
            c = c & 0xff;

            buffer[i] = 0;          // By default, we'll have this as the end of string sentinel

            switch (c) {
                case CHAR_NL:
                    // Newline character, end the string and return the size of the string
                    buffer[i] = 0;
                    return i;

                case CHAR_CR:
                    // Newline character, end the string and return the size of the string
                    buffer[i] = 0;
                    return i;

                case CHAR_BS:
                    // Backspace character, delete the character to the left
                    if (i > 0) {
                        buffer[--i] = 0;
                    }
                    break;

                default:
                    // Ordinary character, add it to the buffer
                    buffer[i++] = (char)c;
                    buffer[i] = 0;
                    break;
            }
        }
    }

    return i;
}

//
// Write a string of bytes to the console.
//
// Terminates writing bytes at a null.
//
// Inputs:
//  buffer = the string of bytes
//  size = the number of bytes to write
//
short con_write(p_channel chan, const uint8_t * buffer, short size) {
    int i;

    TRACE("con_write");

    for (i = 0; i < size; i++) {
        char c = (char)buffer[i];
        if (c == 0) {
            break;
        } else {
            con_write_b(chan, c);
        }
    }

    return i;
}

//
// Return the status of the console
//
short con_status(p_channel chan) {
    // TODO: make CDEV_STAT_READABLE conditional

    return CDEV_STAT_READABLE | CDEV_STAT_WRITABLE;
}

//
// We can't seek on the console... just return 0
//
short con_seek(p_channel chan, long position, short base) {
    return 0;
}

short con_ioctrl(p_channel chan, short command, uint8_t * buffer, short size) {
    return 0;
}

//
// Install the console device driver
//
short con_install() {
    short result;
    t_dev_chan dev;

    dev.name = "CONSOLE";
    dev.number = CDEV_CONSOLE;
    dev.init = con_init;
    dev.open = con_open;
    dev.close = con_close;
    dev.read = con_read;
    dev.readline = con_readline;
    dev.read_b = con_read_b;
    dev.write = con_write;
    dev.write_b = con_write_b;
    dev.flush = con_flush;
    dev.seek = con_seek;
    dev.status = con_status;
    dev.ioctrl = con_ioctrl;

    result = cdev_register(&dev);
    if (result) {
        return result;
    }

    dev.name = "EVID";
    dev.number = CDEV_EVID;
    dev.init = con_init;
    dev.open = con_open;
    dev.close = con_close;
    dev.read = con_read;
    dev.readline = con_readline;
    dev.read_b = con_read_b;
    dev.write = con_write;
    dev.write_b = con_write_b;
    dev.flush = con_flush;
    dev.seek = con_seek;
    dev.status = con_status;
    dev.ioctrl = con_ioctrl;

    result = cdev_register(&dev);
    if (result) {
        return result;
    }

    /* Pre-open the console and EVID channels */

    chan_open(CDEV_CONSOLE, 0, 0);
    // chan_open(CDEV_EVID, 0, 0);

    return result;
}
