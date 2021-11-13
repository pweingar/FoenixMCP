/*
 * Code to support the timers
 */

#ifndef __TIMERS_H
#define __TIMERS_H

/*
 * Initialize the timers and their interrupts
 */
extern void timers_init();

/*
 * Return the number of jiffies (1/60 of a second) since last reset time
 */
extern long timers_jiffies();

#endif
