/*
 * A logging utility...
 */

#ifndef __LOG_H
#define __LOG_H

#include <stdio.h> /* Not used here but convenience: there is every chance log messages will use sprintf */
#include "log_level.h"

#define LOG_CHANNEL_UART0 -1
#define LOG_CHANNEL_CHANNEL_A 0
#define LOG_CHANNEL_CHANNEL B 1

/*
 * Settings
 */
#ifndef DEFAULT_LOG_LEVEL
  #define DEFAULT_LOG_LEVEL LOG_TRACE
#endif

#ifndef LOG_CHANNEL
#define LOG_CHANNEL LOG_CHANNEL_UART0
#endif

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
extern void panic(void);


/*
 * Setup the logging facility (for debugging)
 */
extern void log_init(void);

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
extern void log(short level, const char * message, ...);
extern void trace(const char * message, ...);
/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 */
extern void log2(short level, const char * message1, const char * message2);

/*
 * Log a message to the console
 *
 * Inputs:
 * level = the severity of the message... the logging level will filter messages displayed
 * message1 = the first part of the message to log
 * message2 = the second part of the message to log
 * message3 = the third part of the message to log
 */
extern void log3(short level, const char * message1, const char * message2, const char * message3);

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

#if DEFAULT_LOG_LEVEL >= LOG_ERROR
# define ERROR(m)    log(LOG_ERROR, m)
# define ERROR1(a,b)     log(LOG_ERROR, a, b)
# define ERROR2(a,b,c)     log(LOG_ERROR, a, b, c)
# define ERROR3(a,b,c,d)     log(LOG_ERROR, a, b, c, d)
# define ERROR4(a,b,c,d,e)     log(LOG_ERROR, a, b, c, d, e)
# define ERROR5(a,b,c,d,e,f)     log(LOG_ERROR, a, b, c, d, e, f)
#else
# define ERROR(m)
# define ERROR1(a,b)
# define ERROR2(a,b,c)
# define ERROR3(a,b,c,d)
# define ERROR4(a,b,c,d,e)
# define ERROR5(a,b,c,d,e,f)
#endif


#if DEFAULT_LOG_LEVEL >= LOG_INFO
# define INFO(m)    log(LOG_INFO, m)
# define INFO1(a,b)     log(LOG_INFO, a, b)
# define INFO2(a,b,c)     log(LOG_INFO, a, b, c)
# define INFO3(a,b,c,d)     log(LOG_INFO, a, b, c, d)
# define INFO4(a,b,c,d,e)     log(LOG_INFO, a, b, c, d, e)
# define INFO5(a,b,c,d,e,f)     log(LOG_INFO, a, b, c, d, e, f)
#else
# define INFO(m)
# define INFO1(a,b)
# define INFO2(a,b,c)
# define INFO3(a,b,c,d)
# define INFO4(a,b,c,d,e)
# define INFO5(a,b,c,d,e,f)
#endif

#if DEFAULT_LOG_LEVEL >= LOG_DEBUG
# define DEBUG(m)    log(LOG_DEBUG, m)
# define DEBUG1(a,b)     log(LOG_DEBUG, a, b)
# define DEBUG2(a,b,c)     log(LOG_DEBUG, a, b, c)
# define DEBUG3(a,b,c,d)     log(LOG_DEBUG, a, b, c, d)
# define DEBUG4(a,b,c,d,e)     log(LOG_DEBUG, a, b, c, d, e)
# define DEBUG5(a,b,c,d,e,f)     log(LOG_DEBUG, a, b, c, d, e, f)
#else
# define DEBUG(m)
# define DEBUG1(a,b)
# define DEBUG2(a,b,c)
# define DEBUG3(a,b,c,d)
# define DEBUG4(a,b,c,d,e)
# define DEBUG5(a,b,c,d,e,f)
#endif

#if DEFAULT_LOG_LEVEL >= LOG_TRACE
# define TRACE(m)    trace(m)
# define TRACE1(a,b)     trace(a, b)
# define TRACE2(a,b,c)     trace(a, b, c)
# define TRACE3(a,b,c,d)     trace(a, b, c, d)
# define TRACE4(a,b,c,d,e)     trace(a, b, c, d, e)
# define TRACE5(a,b,c,d,e,f)     trace(a, b, c, d, e, f)
# define TRACE6(a,b,c,d,e,f,g)     trace(a, b, c, d, e, f, g)
# define TRACE7(a,b,c,d,e,f,g,h)     trace(a, b, c, d, e, f, g, h)
#else
# define TRACE(m)
# define TRACE1(a,b)
# define TRACE2(a,b,c)
# define TRACE3(a,b,c,d)
# define TRACE4(a,b,c,d,e)
# define TRACE5(a,b,c,d,e,f)
# define TRACE6(a,b,c,d,e,f,g)
# define TRACE7(a,b,c,d,e,f,g,h)
#endif
#define TRACEC(c)   log_c(LOG_TRACE, c);

#endif
