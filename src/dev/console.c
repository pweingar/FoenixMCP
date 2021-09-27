/**
 * Implementation of the console channel device
 *
 * The console maps to the main screen and keyboard.
 *
 */

#include "types.h"
#include "constants.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/ps2.h"
#include "dev/kbd_mo.h"
#include "dev/text_screen_iii.h"


//
// Initialize the console... nothing needs to happen here
//
short con_init() {
    return 0;
}

//
// Send a uint8_t to the console screen
//
short con_write_b(p_channel chan, uint8_t b) {
    text_put_raw(chan->dev, (char)b);
    return 0;
}

//
// Attempt to read from the keyboard.
//
short con_read_b(p_channel chan) {
    char c;
    do {
        c = kbdmo_getc();
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

    for (i = 0; i < size; i++) {
        char c = (char)buffer[i];
        if (c == 0) {
            break;
        } else {
            text_put_raw(chan->dev, c);
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
// Flush the output to the console... this does nothing...
//
short con_flush(p_channel chan) {
    return 0;
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
    t_dev_chan dev;

    dev.name = "CONSOLE";
    dev.number = CDEV_CONSOLE;
    dev.init = con_init;
    dev.read = con_read;
    dev.readline = con_readline;
    dev.read_b = con_read_b;
    dev.write = con_write;
    dev.write_b = con_write_b;
    dev.flush = con_flush;
    dev.seek = con_seek;
    dev.status = con_status;
    dev.ioctrl = con_ioctrl;

    cdev_register(&dev);

    dev.name = "EVID";
    dev.number = CDEV_EVID;
    dev.init = con_init;
    dev.read = con_read;
    dev.readline = con_readline;
    dev.read_b = con_read_b;
    dev.write = con_write;
    dev.write_b = con_write_b;
    dev.flush = con_flush;
    dev.seek = con_seek;
    dev.status = con_status;
    dev.ioctrl = con_ioctrl;

    return cdev_register(&dev);
}
