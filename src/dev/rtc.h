/*
 * Declarations for access the bq4802LY real time clock
 */

#ifndef __RTC_H
#define __RTC_H

typedef struct s_time {
    short year;
    short month;
    short day;
    short hour;
    short minute;
    short second;
    short is_pm;
    short is_24hours;
} t_time, *p_time;

/*
 * Initialize the RTC
 */
extern void rtc_init();

/*
 * Set the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record containing the correct time
 */
extern void rtc_set_time(p_time time);

/*
 * Get the time on the RTC
 *
 * Inputs:
 * time = pointer to a t_time record in which to put the current time
 */
extern void rtc_get_time(p_time time);

/*
 * Get the number of ticks since the system last booted.
 *
 * NOTE: a tick is almost, but not quite, 1ms. The RTC periodic interrupt
 *       period does not line up with a 1ms timer, but it comes close.
 *       Therefore, a tick will be 976.5625 microseconds... a little faster
 *       than 1ms.
 *
 * Returns:
 * the number of ticks since the last reset
 */
extern long rtc_get_ticks();

#endif
