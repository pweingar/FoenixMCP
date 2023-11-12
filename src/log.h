/*
 * A logging utility...
 */

#ifndef __LOG_H
#define __LOG_H

#include <stdio.h> /* Not used here but convenience: there is every chance log messages will use sprintf */

#include "log_level.h"
#include "sys_general.h"

#define LOG_CHANNEL_CHANNEL_A 0
#define LOG_CHANNEL_CHANNEL_B 1
#define LOG_CHANNEL_CHANNEL_A_LOW_LEVEL 10 // low-level routines (doesn't use MCP's console stuff)
#define LOG_CHANNEL_UART1 20

/*
 * Default settings
 */
#ifndef DEFAULT_LOG_LEVEL
  #define DEFAULT_LOG_LEVEL LOG_INFO
#endif

#ifndef LOG_CHANNEL
  #if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_A2560X || MODEL == MODEL_FOENIX_GENX
    #define LOG_CHANNEL LOG_CHANNEL_CHANNEL_A_LOW_LEVEL
  #else
    #define LOG_CHANNEL LOG_CHANNEL_UART1
  #endif
#else
#endif

#define LOGBUF_SIZE 200;
extern short log_level;
extern void (*do_log)(const char* message);
extern char logbuf[];

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


void buzzer_on(void);
void buzzer_off(void);

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
extern void logmsg(short level, const char * message, ...);

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
 * Send a message to the debugging channel.
 * We are inling calls to snprintf and do_log because there are problems when using a proxy function to vsnprintf
 */

#if DEFAULT_LOG_LEVEL >= LOG_ERROR
#define ERROR(a) if (log_level >= LOG_ERROR) { do_log(a); }
#define ERROR1(a,b) if (log_level >= LOG_ERROR) { snprintf(logbuf,200,a,b);do_log(logbuf); }
#define ERROR2(a,b,c) if (log_level >= LOG_ERROR) { snprintf(logbuf,200,a,b,c);do_log(logbuf); }
#define ERROR3(a,b,c,d) if (log_level >= LOG_ERROR) { snprintf(logbuf,200,a,b,c,d);do_log(logbuf); }
#define ERROR4(a,b,c,d,e) if (log_level >= LOG_ERROR) { snprintf(logbuf,200,a,b,c,d,e);do_log(logbuf); }
#define ERROR5(a,b,c,d,e,f) if (log_level >= LOG_ERROR) { snprintf(logbuf,200,a,b,c,d,e,f);do_log(logbuf); }
#else
# define ERROR(m)
# define ERROR1(a,b)
# define ERROR2(a,b,c)
# define ERROR3(a,b,c,d)
# define ERROR4(a,b,c,d,e)
# define ERROR5(a,b,c,d,e,f)
#endif


#if DEFAULT_LOG_LEVEL >= LOG_INFO
#define INFO(a) if (log_level >= LOG_INFO) { do_log(a); }
#define INFO1(a,b) if (log_level >= LOG_INFO) { snprintf(logbuf,200,a,b);do_log(logbuf); }
#define INFO2(a,b,c) if (log_level >= LOG_INFO) { snprintf(logbuf,200,a,b,c);do_log(logbuf); }
#define INFO3(a,b,c,d) if (log_level >= LOG_INFO) { snprintf(logbuf,200,a,b,c,d);do_log(logbuf); }
#define INFO4(a,b,c,d,e) if (log_level >= LOG_INFO) { snprintf(logbuf,200,a,b,c,d,e);do_log(logbuf); }
#define INFO5(a,b,c,d,e,f) if (log_level >= LOG_INFO) { snprintf(logbuf,200,a,b,c,d,e,f);do_log(logbuf); }
#else
# define INFO(m)
# define INFO1(a,b)
# define INFO2(a,b,c)
# define INFO3(a,b,c,d)
# define INFO4(a,b,c,d,e)
# define INFO5(a,b,c,d,e,f)
#endif

#if DEFAULT_LOG_LEVEL >= LOG_DEBUG
#define DEBUG(a) if (log_level >= LOG_DEBUG) { do_log(a); }
#define DEBUG1(a,b) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b);do_log(logbuf); }
#define DEBUG2(a,b,c) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b,c);do_log(logbuf); }
#define DEBUG3(a,b,c,d) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b,c,d);do_log(logbuf); }
#define DEBUG4(a,b,c,d,e) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b,c,d,e);do_log(logbuf); }
#define DEBUG5(a,b,c,d,e,f) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b,c,d,e,f);do_log(logbuf); }
#define DEBUG6(a,b,c,d,e,f,g) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b,c,d,e,f,g);do_log(logbuf); }
#define DEBUG7(a,b,c,d,e,f,g,h) if (log_level >= LOG_DEBUG) { snprintf(logbuf,200,a,b,c,d,e,f,g,h);do_log(logbuf); }
#else
# define DEBUG(m)
# define DEBUG1(a,b)
# define DEBUG2(a,b,c)
# define DEBUG3(a,b,c,d)
# define DEBUG4(a,b,c,d,e)
# define DEBUG5(a,b,c,d,e,f)
# define DEBUG6(a,b,c,d,e,f,g)
# define DEBUG7(a,b,c,d,e,f,g,h)
#endif

#if 0 && DEFAULT_LOG_LEVEL >= LOG_TRACE
#define TRACE(a) if (log_level >= LOG_TRACE) { do_log(a); }
#define TRACE1(a,b) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b);do_log(logbuf); }
#define TRACE2(a,b,c) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b,c);do_log(logbuf); }
#define TRACE3(a,b,c,d) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b,c,d);do_log(logbuf); }
#define TRACE4(a,b,c,d,e) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b,c,d,e);do_log(logbuf); }
#define TRACE5(a,b,c,d,e,f) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b,c,d,e,f);do_log(logbuf); }
#define TRACE6(a,b,c,d,e,f,g) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b,c,d,e,f,g);do_log(logbuf); }
#define TRACE7(a,b,c,d,e,f,g,h) if (log_level >= LOG_TRACE) { snprintf(logbuf,200,a,b,c,d,e,f,g,h);do_log(logbuf); }
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
