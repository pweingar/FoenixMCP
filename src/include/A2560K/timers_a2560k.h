/**
 * @file timers_a2560k.h
 *
 * Define timer registers on the A2560K
 */

#ifndef __A2560K_TIMERS_H
#define __A2560K_TIMERS_H

/** Timer control register 0: timers 0, 1, and 2 */
#define TIMER_TCR0      ((unsigned long *)0xFEC00200)
#define TCR_ENABLE_0    0x00000001  /** Enable counter 0 */
#define TCR_CLEAR_0     0x00000002  /** Master clear of counter 0 */
#define TCR_LOAD_0      0x00000004  /** Master load of counter 0 */
#define TCR_CNTUP_0     0x00000008  /** Counter 0 Count up if 1, count down if 0 */
#define TCR_INE_0       0x00000080  /** Interrupt enable for counter 0 */

#define TCR_ENABLE_1    0x00000100  /** Enable counter 1 */
#define TCR_CLEAR_1     0x00000200  /** Master clear of counter 1 */
#define TCR_LOAD_1      0x00000400  /** Master load of counter 1 */
#define TCR_CNTUP_1     0x00000800  /** Counter 1 Count up if 1, count down if 0 */
#define TCR_INE_1       0x00008000  /** Interrupt enable for counter 1 */

#define TCR_ENABLE_2    0x00010000  /** Enable counter 2 */
#define TCR_CLEAR_2     0x00020000  /** Master clear of counter 2 */
#define TCR_LOAD_2      0x00040000  /** Master load of counter 2 */
#define TCR_CNTUP_2     0x00080000  /** Counter 2 Count up if 1, count down if 0 */
#define TCR_INE_2       0x00800000  /** Interrupt enable for counter 2 */

/** Timer control register 1: timers 3 and 4 */
#define TIMER_TCR1      ((unsigned long *)0xFEC00204)
#define TCR_ENABLE_3    0x00000001  /** Enable counter 3 */
#define TCR_CLEAR_3     0x00000002  /** Master clear of counter 3 */
#define TCR_LOAD_3      0x00000004  /** Master load of counter 3 */
#define TCR_CNTUP_3     0x00000008  /** Counter 3 Count up if 1, count down if 0 */
#define TCR_RECLR_3     0x00000010  /** Enable reclear of timer 3 */
#define TCR_RELOAD_3    0x00000020  /** Enable reload of timer 3 */
#define TCR_INE_3       0x00000080  /** Interrupt enable for counter 3 */

#define TCR_ENABLE_4    0x00000100  /** Enable counter 4 */
#define TCR_CLEAR_4     0x00000200  /** Master clear of counter 4 */
#define TCR_LOAD_4      0x00000400  /** Master load of counter 4 */
#define TCR_CNTUP_4     0x00000800  /** Counter 4 Count up if 1, count down if 0 */
#define TCR_RECLR_4     0x00001000  /** Enable reclear of timer 4 */
#define TCR_RELOAD_4    0x00002000  /** Enable reload of timer 4 */
#define TCR_INE_4       0x00008000  /** Interrupt enable for counter 4 */

#define TCR_STAT_EQ0    0x08000000  /** Read only: timer 0 is equal to its comparison value */
#define TCR_STAT_EQ1    0x10000000  /** Read only: timer 1 is equal to its comparison value */
#define TCR_STAT_EQ2    0x20000000  /** Read only: timer 2 is equal to its comparison value */
#define TCR_STAT_EQ3    0x40000000  /** Read only: timer 3 is equal to its comparison value */

#define TIMER_VALUE_0   ((unsigned long *)0xFEC00208)
#define TIMER_COMPARE_0 ((unsigned long *)0xFEC0020C)
#define TIMER_VALUE_1   ((unsigned long *)0xFEC00210)
#define TIMER_COMPARE_1 ((unsigned long *)0xFEC00214)
#define TIMER_VALUE_2   ((unsigned long *)0xFEC00218)
#define TIMER_COMPARE_2 ((unsigned long *)0xFEC0021C)
#define TIMER_VALUE_3   ((unsigned long *)0xFEC00220)
#define TIMER_COMPARE_3 ((unsigned long *)0xFEC00224)
#define TIMER_VALUE_4   ((unsigned long *)0xFEC00228)
#define TIMER_COMPARE_4 ((unsigned long *)0xFEC0022C)

#endif
