/*
 * A logging utility...
 */

#ifndef __LOG_H
#define __LOG_H

#define LOG_FATAL   0   /* Log a critical or fatal event */
#define LOG_ERROR   1   /* Log an error */
#define LOG_INFO    2   /* Log miscellaneous information */
#define LOG_DEBUG   3   /* Log a debugging message */
#define LOG_TRACE   4   /* Log tracing information (like entry into a subroutine) */
#define LOG_VERBOSE 5   /* Log a truly verbose message... the sort you almost never want to bother with */

/*
 * Return human readable message for an error number
 */
extern const char * err_message(short err_number);

/*
 * Print an error message
 *
 * Inputs:
 * channel = the number of the channel to print to
 * message = a message string to print before the error
 * err_number = the number of the error
 */
extern void err_print(short channel, const char * message, short err_number);

/*
 * Display a panic screen
 *
 * Inputs:
 * vector = the number of the vector that was called
 */
extern void panic(unsigned short exception_number);

/*
 * Set the maximum level of verbosity in logging.
 * To be printed, a message must have a level lower or equal to this level.
 *
 * Input:
 * level = the maximum level of verbosity to log
 */
extern void log_setlevel(short level);

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message = the message to log
 */
extern void log(short level, char * message);

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 */
extern void log2(short level, char * message1, char * message2);

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 * message3 = the third part of the message to log
 */
extern void log3(short level, char * message1, char * message2, char * message3);

/*
 * Log a message with a number
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * n = the number to log
 */
extern void log_num(short level, char * message, int n);

/*
 * Log a single character
 */
extern void log_c(short log_level, char c);

/*
 * Send a message to the debugging channel
 */
#define DEBUG(m)    log(LOG_DEBUG, m);
#define TRACE(m)    log(LOG_TRACE, m);
#define TRACEC(c)   log_c(LOG_TRACE, c);

#endif
