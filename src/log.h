/*
 * A logging utility...
 */

#ifndef __LOG_H
#define __LOG_H

/*
 * Send a message to the debugging channel
 */
extern void DEBUG(char * message);

#define TRACE(msg) DEBUG(msg);

#endif
