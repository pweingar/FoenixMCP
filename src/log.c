/*
 * A logging utility
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "interrupt.h"
#include "log.h"
#include "simpleio.h"
#include "syscalls.h"

#include "dev/uart.h"
#include "dev/txt_screen.h"
#include "vicky3_txt_a_logger.h"

#if MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/gabe_a2560u.h"
#elif (MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X)
#include "A2560K/gabe_a2560k.h"
#elif MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS || MODEL == MODEL_FOENIX_FMX
#include "C256/gabe_c256.h"
#endif

/* Channel to which the logging output should go.
 * Positive: screen number
 * -1: UART. 
 */
static short log_channel;
static short log_level;

// do_log either points to log_to_uart or log_to_screen.
static void (*do_log)(const char* message);
static void log_to_uart(const char* message);
static void log_to_screen(const char* message);
static void log_to_channel_A_low_level(const char *message);

#define UART_COM1 0

/* Can use the buzzer as sound clues */
void buzzer_on(void) {
    *(GABE_CTRL_REG) = BUZZER_CONTROL;
}


void buzzer_off(void) {
    *(GABE_CTRL_REG) &= ~BUZZER_CONTROL;
}


void log_init(void) {
    log_setlevel(DEFAULT_LOG_LEVEL);
    log_channel = LOG_CHANNEL;

    switch (log_channel) {
    case LOG_CHANNEL_UART1:
        do_log = log_to_uart;
        uart_init(UART_COM1);

        break;
#if (MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X)
    case LOG_CHANNEL_CHANNEL_A_LOW_LEVEL:
        channel_A_logger_init();
        do_log = log_to_channel_A_low_level;
        break;
#endif
    default:
        do_log = log_to_screen;
    }
    INFO("FOENIX DEBUG OUTPUT------------");
}

unsigned short panic_number;        /* The number of the kernel panic */
unsigned long panic_pc;             /* The PC where the issue occurred */
unsigned long panic_address;        /* The address that was accessed (for some exceptions) */

const char * err_messages[] = {
    "OK",
    "general error",
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
    "bad argument",
    "media changed"
};

/*
 * Return human readable message for an error number
 */
const char * err_message(short err_number) {
    short index = 0 - err_number;

    if (index < 40) {
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

    if ((err_number < 0) && (err_number > -40)) {
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
TRACE("PANIC------------------------------------------");
    /* Shut off all interrupts */
    // TODO: int_disable_all();

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


static void log_to_uart(const char *message) {
    char *c = (char*)message;
    while (*c) {
        if (*c == '\n')
            uart_put(UART_COM1,'\r');
        uart_put(UART_COM1, *c++);
    }
     uart_put(UART_COM1,'\r');
     uart_put(UART_COM1,'\n');
}

static void log_to_screen(const char *message) {
    txt_print(log_channel, message);
    txt_print(log_channel, "\n");
}

#if (MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X)
static void log_to_channel_A_low_level(const char *message) {
    channel_A_write(message);
    channel_A_write("\n");
}
#endif

/*
 * Log a message to the console.
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message/args = like printf.
 *
 * Caveat:
 * The total length should not exceed 512 chars.
 */
void logmsg(short level, const char * message, ...) {
    if (level > log_level)
        return;

    char buf[80]; // Should hopefully be long enough !

    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);

    do_log(buf);
}

void trace(const char * message, ...) {
    if (LOG_TRACE > log_level)
        return;

    char buf[80]; // Should hopefully be long enough !

    va_list args;
    va_start(args, message);
    vsprintf(buf, message, args);
    va_end(args);

    (*do_log)(buf);
}

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 */
void log2(short level, const char * message1, const char * message2) {
    if (level <= log_level) {
        char line[80];
        sprintf(line, "%s%s\n", message1, message2);
        logmsg(level, line);
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
        logmsg(level, line);
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
        sprintf(line, "%s%08X", message, n);
        logmsg(level, line);
    }
}

/*
 * Send a single character to the debugging channel
 */
void log_c(short level, char c) {
    char line[2];
    line[0] = c;
    line[1] = '\0';
    logmsg(level, line);
}
