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
