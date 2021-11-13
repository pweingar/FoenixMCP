/*
 * A logging utility
 */

#include <string.h>
#include "log.h"
#include "simpleio.h"
#include "dev/text_screen_iii.h"

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
    "file system invalid parameter"
};

/*
 * Return human readable message for an error number
 */
const char * err_message(short err_number) {
    short index = 0 - err_number;

    if (index < sizeof(err_messages)) {
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

    if (err_number < 0) {
        sprintf(buffer, "%s: %s\n", message, err_message(err_number));
    } else {
        sprintf(buffer, "%s: #%d\n", message, err_number);
    }

    sys_chan_write(channel, buffer, strlen(buffer));
}

/*
 * Display a panic screen
 */
void panic() {
    char buffer[80];
    short column = 18;
    short row = 10;
    short address_expected = 0;

    /* Shut off all interrupts */
    int_disable_all();

    /* Re-initialize the text screen */
    text_init();
    text_set_border(0, 0, 0, 0, 0);
    text_set_color(0, 15, 1);
    text_set_cursor(0, 0, 0, 0, 0);
    text_clear(0, 2);

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");
    print(0, buffer);

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xB3                                          \xB3");
    print(0, buffer);

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xB3 Oh dear, something has gone wrong...     \xB3");
    print(0, buffer);

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xB3                                          \xB3");
    print(0, buffer);

    text_set_xy(0, column, row++);
    switch (panic_number) {
        case 2:
            sprintf(buffer, "\xB3 Bus Error                                \xB3");
            address_expected = 1;
            break;
        case 3:
            sprintf(buffer, "\xB3 Address Error                            \xB3");
            address_expected = 1;
            break;
        case 4:
            sprintf(buffer, "\xB3 Illegal Instruction Error                \xB3");
            break;
        case 5:
            sprintf(buffer, "\xB3 Division by Zero Error                   \xB3");
            break;
        case 6:
            sprintf(buffer, "\xB3 Range Check Exception                    \xB3");
            break;
        case 7:
            sprintf(buffer, "\xB3 Overflow Exception                       \xB3");
            break;
        case 8:
            sprintf(buffer, "\xB3 Privilege Exception                      \xB3");
            break;
        case 24:
            sprintf(buffer, "\xB3 Spurious Interrupt                       \xB3");
            break;

        default:
            sprintf(buffer, "\xB3 Unknown Exception                        \xB3");
            break;
    }
    print(0, buffer);

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xB3                                          \xB3");
    print(0, buffer);

    if (address_expected) {
        text_set_xy(0, column, row++);
        print(0, "\xB3 PC: ");
        print_hex_32(0, panic_pc);
        print(0, "           Address: ");
        print_hex_32(0, panic_address);
        print(0, " \xB3");
    } else {
        text_set_xy(0, column, row++);
        print(0, "\xB3 PC: ");
        print_hex_32(0, panic_pc);
        print(0, "                             \xB3");
    }

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xB3                                          \xB3");
    print(0, buffer);

    text_set_xy(0, column, row++);
    sprintf(buffer, "\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");
    print(0, buffer);

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
        print(log_channel, message);
        print_c(log_channel, '\n');
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
        print(log_channel, message1);
        print(log_channel, message2);
        print_c(log_channel, '\n');
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
void log3(short level, char * message1, char * message2, char * message3) {
    if (level <= log_level) {
        print(log_channel, message1);
        print(log_channel, message2);
        print(log_channel, message3);
        print_c(log_channel, '\n');
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
    if (level <= log_level) {
        print(log_channel, message);
        print_hex_32(log_channel, n);
        print_c(log_channel, '\n');
    }
}

/*
 * Send a single character to the debugging channel
 */
void log_c(short level, char c) {
    if (log_level <= level) {
        print_c(log_channel, c);
    }
}
