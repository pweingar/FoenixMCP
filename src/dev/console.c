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
#include "simpleio.h"

#define ANSI_BUFFER_SIZE    16
#define MAX_ANSI_ARGS       10

#define CON_CTRL_ANSI       0x80            /* Set to enable ANSI escape processing */
#define CON_IOCTRL_ANSI_ON  0x01            /* IOCTRL Command: turn on ANSI terminal codes */
#define CON_IOCTRL_ANSI_OFF 0x02            /* IOCTRL Command: turn off ANSI terminal codes */

typedef void (*ansi_handler)(p_channel, short, short[]);

/*
 * Structure to map an ANSI escape sequence pattern to a handler
 */
typedef struct s_ansi_seq {
    char code;
    ansi_handler handler;
} t_ansi_seq, *p_ansi_seq;

/*
 * Structure to track console state
 */
typedef struct s_console_data {
    unsigned char control;              /* Control flags for the console: e.g. process ANSI codes */
    unsigned char ansi_buffer_count;    /* Number of characters in the ANSI BUFFER */
    char ansi_buffer[ANSI_BUFFER_SIZE]; /* Used to keep track of characters in the ANSI escape sequences */
    char key_buffer;                    /* Used to peek at keyboard input */
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
extern void ansi_sgr(p_channel chan, short arg_count, short args[]);

/*
 * Console variables and constants
 */

/*
 * ANSI escape sequences
 */
const t_ansi_seq ansi_sequence[] = {
    { '@', ansi_ich },
    { 'A', ansi_cuu },
    { 'B', ansi_cud },
    { 'C', ansi_cuf },
    { 'D', ansi_cub },
    { 'J', ansi_ed },
    { 'K', ansi_el },
    { 'P', ansi_dch },
    { 'H', ansi_cup },
    { 'm', ansi_sgr },
    { 0, 0 }
};

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
        case '\x1b':
            return 1;

        default:
            return 0;
    }
}

/*
 * Find the escape sequence pattern that matches what we've collected.
 *
 * If one is found, execute it.
 * If one is not found, just dump the ANSI buffer
 */
void ansi_match_pattern(p_channel chan, p_console_data con_data) {
    char c;
    short argc = 0;
    short arg[MAX_ANSI_ARGS];
    short i, j;

    TRACE("ansi_match_pattern");

    /* Clear out the argument list */
    for (i = 0; i < MAX_ANSI_ARGS; i++) {
        arg[i] = 0;
    }

    /* Make sure we have an escape sequence */
    if ((con_data->ansi_buffer[0] != '\x1b') || (con_data->ansi_buffer[1] != '[')) {
        /* If not, dump the buffer and return */
        con_flush(chan);
        return;
    }

    /* Try to assemble the arguments */
    for (i = 2, argc = 0; i < con_data->ansi_buffer_count - 1; i++) {
        c = con_data->ansi_buffer[i];
        if (isdigit(c)) {
            /* If the character is a digit, add it to the current argument */
            arg[argc] = arg[argc] * 10 + (c - '0');

        } else if (c == ';') {
            /* If it's a semi-colon, start the next argument */
            argc++;
        }
    }

    argc++;

    /* Get the last character... should be the code */
    c = con_data->ansi_buffer[con_data->ansi_buffer_count - 1];

    /* Try to find the matching escape code */
    for (i = 0; ansi_sequence[i].code != 0; i++) {
        if (ansi_sequence[i].code == c) {
            ansi_handler handler = ansi_sequence[i].handler;
            if (handler != 0) {
                /* If we found the handler... call it */
                handler(chan, argc, arg);
                return;
            }
        }
    }

    /* We got to the end without a match... dump the buffer */
    con_flush(chan);
}

/*
 * Add a character to the ANSI buffer... and process when an escape sequence is found
 */
void ansi_process_c(p_channel chan, p_console_data con_data, char c) {
    TRACE("ansi_process_c");

    if (c == '\x1B') {
        /* Start the escape sequence */
        con_data->ansi_buffer[0] = c;
        con_data->ansi_buffer_count = 1;

    } else if (con_data->ansi_buffer_count > 0) {
        /* We're processing an escape sequence... add the character to the buffer */
        con_data->ansi_buffer[con_data->ansi_buffer_count++] = c;

        /* Check to see if we just wrote the trigger */
        if (((c >= '@') && (c <= 'Z')) ||
            ((c >= 'a') && (c <= 'z'))) {
            ansi_match_pattern(chan, con_data);
            con_data->ansi_buffer_count = 0;
        }

    } else {
        /* Not working on a sequence... so just print it */
        text_put_raw(chan->dev, c);
    }
}

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

/*
 * Set Graphics Rendition
 */
void ansi_sgr(p_channel chan, short argc, short args[]) {
    short foreground = 0, background = 0;
    short i;

    TRACE("ansi_sgr");

    /* Get the current colors */
    text_get_color(chan->dev, &foreground, &background);

    /* Walk through each argument code... */
    for (i = 0; i < argc; i++) {
        short code = args[i];

        if ((code >= 30) && (code <= 37)) {
            /* Set foreground color */
            foreground = code - 30;

        } else if ((code >= 90) && (code <= 97)) {
            /* Set bright foreground color */
            foreground = code - 82;

        } else if ((code >= 40) && (code <= 47)) {
            /* Set background color */
            background = code - 40;

        } else if ((code >= 100) && (code <= 107)) {
            /* Set bright background color */
            background = code - 92;

        } else if ((code == 0) || (code == 2) || (code == 22)) {
            /* Reset, dim, and normal intensity */
            foreground = foreground & 0x07;
            background = background & 0x07;

        } else if (code == 1) {
            /* Bold intensity */
            foreground = foreground | 0x08;
        }
    }

    /* Set the colors */
    text_set_color(chan->dev, foreground, background);
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
    con_data->key_buffer = 0;

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
            con_data->ansi_buffer[i] = 0;
        }
    }

    con_data->ansi_buffer_count = 0;
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
 * Send a byte to the console screen
 */
short con_write_b(p_channel chan, uint8_t b) {
    p_console_data con_data;

    /* Check to see if we need to process ANSI codes */
    con_data = &(chan->data);
    if (con_data->control & CON_CTRL_ANSI) {
        /* ANSI codes are to be processed */
        ansi_process_c(chan, con_data, (char)b);

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
    p_console_data con_data;

    /* Check to see if we need to process ANSI codes */
    con_data = &(chan->data);

    char c;
    do {
        if (con_data->key_buffer != 0) {
            c = con_data->key_buffer;
            con_data->key_buffer = 0;

        } else {

#if MODEL == MODEL_FOENIX_A2560K
#ifdef KBD_POLLED
            ps2_mouse_get_packet();
            c = kbdmo_getc_poll();
#else
            c = kbdmo_getc();
#endif
#else
#ifdef KBD_POLLED
            c = kbd_getc_poll();
#else
            c = kbd_getc();
#endif
#endif
        }

    } while (c == 0);

    // Echo the character to the screen
    con_write_b(chan, c);

    return (short)(c & 0x00ff);
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

short con_has_input(p_channel chan) {
    p_console_data con_data;
    char c;

    /* Check to see if we need to process ANSI codes */
    con_data = &(chan->data);

    if (con_data->key_buffer != 0) {
        /* If we already peeked and have a character... return true */
        return 1;

    } else {
        /* Otherwise, peek at the keyboard to see if there is a valid key */

    #if MODEL == MODEL_FOENIX_A2560K
    #ifdef KBD_POLLED
                ps2_mouse_get_packet();
                c = kbdmo_getc_poll();
    #else
                c = kbdmo_getc();
    #endif
    #else
    #ifdef KBD_POLLED
                c = kbd_getc_poll();
    #else
                c = kbd_getc();
    #endif
    #endif

        if (c == 0) {
            /* No: return false */
            return 0;

        } else {
            /* Yes: save the key we got and return true */
            con_data->key_buffer = c;
            return 1;
        }
    }
}

//
// Return the status of the console
//
short con_status(p_channel chan) {
    short status = CDEV_STAT_WRITABLE;  /* The console is always writable */
    if (con_has_input(chan)) {
        /* If there is data available, flag that it's readable */
        status |= CDEV_STAT_READABLE;
    }

    return status;
}

//
// We can't seek on the console... just return 0
//
short con_seek(p_channel chan, long position, short base) {
    return 0;
}

short con_ioctrl(p_channel chan, short command, uint8_t * buffer, short size) {
    p_console_data con_data;

    /* Check to see if we need to process ANSI codes */
    con_data = &(chan->data);

    switch (command) {
        case CON_IOCTRL_ANSI_ON:
            /* Turn on ANSI interpreting */
            con_data->control |= CON_CTRL_ANSI;
            return 0;

        case CON_IOCTRL_ANSI_OFF:
            /* Turn on ANSI interpreting */
            con_data->control &= ~CON_CTRL_ANSI;
            return 0;

        default:
            break;
    }
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
