#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

/* This is separate from log.h so this can be included first in .c files,
 * so we can use these defines for setting the log level with a friendly name. */

#define LOG_FATAL   0   /* Log a critical or fatal event */
#define LOG_ERROR   1   /* Log an error */
#define LOG_INFO    2   /* Log miscellaneous information */
#define LOG_DEBUG   3   /* Log a debugging message */
#define LOG_TRACE   4   /* Log tracing information (like entry into a subroutine) */
#define LOG_VERBOSE 5   /* Log a truly verbose message... the sort you almost never want to bother with */

#endif