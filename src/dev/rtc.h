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
 * Make sure the RTC tick counter is enabled
 */
extern void rtc_enable_ticks();

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
 * Get the number of jiffies since the system last booted.
 *
 * NOTE: a jiffie is 1/60 of a second. This timer will not be
 *       100% precise, so it should be used for timeout purposes
 *       where precision is not critical.
 *
 * Returns:
 * the number of jiffies since the last reset
 */
extern long rtc_get_jiffies();

#endif
