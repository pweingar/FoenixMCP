/*
 * A logging utility
 */

#include <stdio.h>
#include <string.h>

#include "interrupt.h"
#include "log.h"
#include "simpleio.h"
#include "syscalls.h"
#include "dev/txt_screen.h"

static short log_channel = 0;
static short log_level = 999;

void log_init() {
    log_channel = 0;
    log_level = 999;
}

unsigned short panic_number;        /* The number of the kernel panic */
unsigned long panic_pc;             /* The PC where the issue occurred */
unsigned long panic_address;        /* The address that was accessed (for some exceptions) */

const char * err_messages[] = {
    "OK",
    "error",
    "bad device number",
    "operation timed out",
    "device could not be initialized",
    "could not read from device",
    "could not write to device",
    "out of bounds",
    "no media",
    "device is write protected",
    "bad channel number",
    "out of handles",
    "bad handle",
    "unknown file type",
    "out of memory",
    "bad binary file",
    "file is not executable",
    "not found",
    "low level disk error",
    "file system assertion failed",
    "device not ready",
    "file not found",
    "directory not found",
    "invalid path name",
    "access denied",
    "prohibited access",
    "invalid object",
    "drive is write protected",
    "invalid drive",
    "volume has no work area",
    "no file system found",
    "creation of file system aborted",
    "file system timeout",
    "file locked",
    "not enough core",
    "too many open files",
    "file system invalid parameter",
    "not supported",
    "bad argument"
};

/*
 * Return human readable message for an error number
 */
const char * err_message(short err_number) {
    short index = 0 - err_number;

    if (index < 39) {
        return err_messages[index];
    } else {
        return "unknown error";
    }
}

/*
 * Print an error message
 *
 * Inputs:
 * channel = the number of the channel to print to
 * message = a message string to print before the error
 * err_number = the number of the error
 */
void err_print(short channel, const char * message, short err_number) {
    char buffer[80];

    if ((err_number < 0) && (err_number > -38)) {
        sprintf(buffer, "%s: %s\n", message, err_message(err_number));
    } else {
        sprintf(buffer, "%s: #%d\n", message, err_number);
    }

    sys_chan_write(channel, buffer, strlen(buffer));
}

/*
 * Display a panic screen
 */
void panic(void) {
    char buffer[80];
    short column = 2;
    short row = 2;
    short address_expected = 0;
    t_rect region;

    /* Shut off all interrupts */
    int_disable_all();

    /* Re-initialize the text screen */
    txt_init_screen(0);
    txt_set_border(0, 0, 0);
    region.origin.x = 0;
    region.origin.y = 0;
    region.size.width = 0;
    region.size.height = 0;
    txt_set_region(0, &region);
    txt_set_color(0, 15, 1);
    txt_set_cursor(0, 0, 0, 0);
    txt_clear(0, 2);

    txt_set_xy(0, column, row++);
    txt_print(0, "\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");

    txt_set_xy(0, column, row++);
    txt_print(0, "\xB3                                          \xB3");

    txt_set_xy(0, column, row++);
    txt_print(0, "\xB3 Oh dear, something has gone wrong...     \xB3");

    txt_set_xy(0, column, row++);
    txt_print(0, "\xB3                                          \xB3");

    txt_set_xy(0, column, row++);
    switch (panic_number) {
        case 2:
            txt_print(0, "\xB3 Bus Error                                \xB3");
            address_expected = 1;
            break;
        case 3:
            txt_print(0, "\xB3 Address Error                            \xB3");
            address_expected = 1;
            break;
        case 4:
            txt_print(0, "\xB3 Illegal Instruction Error                \xB3");
            break;
        case 5:
            txt_print(0, "\xB3 Division by Zero Error                   \xB3");
            break;
        case 6:
            txt_print(0, "\xB3 Range Check Exception                    \xB3");
            break;
        case 7:
            txt_print(0, "\xB3 Overflow Exception                       \xB3");
            break;
        case 8:
            txt_print(0, "\xB3 Privilege Exception                      \xB3");
            break;
        case 24:
            txt_print(0, "\xB3 Spurious Interrupt                       \xB3");
            break;

        default:
            txt_print(0, "\xB3 Unknown Exception                        \xB3");
            break;
    }

    txt_set_xy(0, column, row++);
    txt_print(0, "\xB3                                          \xB3");

    if (address_expected) {
        txt_set_xy(0, column, row++);
        sprintf(buffer, "\xB3 PC: %08X           Address: %08X \xB3", (unsigned int)panic_pc, (unsigned int)panic_address);
        txt_print(0, buffer);
    } else {
        txt_set_xy(0, column, row++);
        sprintf(buffer, "\xB3 PC: %08X                             \xB3", (unsigned int)panic_pc);
        txt_print(0, buffer);
    }

    txt_set_xy(0, column, row++);
    txt_print(0, "\xB3                                          \xB3");

    txt_set_xy(0, column, row++);
    txt_print(0, "\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");

    /* Wait forever */
    while (1) ;
}

/*
 * Set the maximum level of verbosity in logging.
 * To be printed, a message must have a level lower or equal to this level.
 *
 * Input:
 * level = the maximum level of verbosity to log
 */
void log_setlevel(short level) {
    log_level = level;
}

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message = the message to log
 */
void log(short level, char * message) {
    if (level <= log_level) {
        txt_print(log_channel, message);
        txt_print(log_channel, "\n");
    }
}

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 */
void log2(short level, char * message1, char * message2) {
    if (level <= log_level) {
        char line[80];
        sprintf(line, "%s%s\n", message1, message2);
        txt_print(log_channel, line);
    }
}

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 * message3 = the third part of the message to log
 */
void log3(short level, const char * message1, const char * message2, const char * message3) {
    if (level <= log_level) {
        char line[80];
        sprintf(line, "%s%s%s\n", message1, message2, message3);
        txt_print(log_channel, line);
    }
}

/*
 * Log a message with a number
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * n = the number to log
 */
void log_num(short level, char * message, int n) {
    char line[80];

    if (level <= log_level) {
        sprintf(line, "%s%08X\n", message, n);
        print(log_channel, line);
    }
}

/*
 * Send a single character to the debugging channel
 */
void log_c(short level, char c) {
    if (log_level <= level) {
        txt_put(log_channel, c);
    }
}
