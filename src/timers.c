#include "interrupt.h"
#include "timers.h"
#include "gabe_reg.h"
#include "timers_reg.h"

long jiffy_count;

/*
 * Interrupt handler for the Channel A SOF interrupt... just counts jiffies
 *
 * NOTE: in time, this should be handled by the RTC or another timer.
 */
void sof_a_handler() {
    jiffy_count++;
}

/*
 * Initialize the timers and their interrupts
 */
void timers_init() {
    jiffy_count = 0;

    // int_register(INT_SOF_A, sof_a_handler);
    // int_enable(INT_SOF_A);

    *TIMER_TCR0 = 0;    // Reset timers 0, 1, and 2
    *TIMER_TCR1 = 0;    // Reset timers 3, and 4 (if 4 is available)

    // Clear timer 3

    *TIMER_VALUE_3 = 0;
    *TIMER_TCR1 = TCR_ENABLE_3 | TCR_CNTUP_3 | TCR_CLEAR_3;

    // Set timer 3 to count up and auto clear

    *TIMER_TCR1 = TCR_ENABLE_3 | TCR_CNTUP_3;
}

/*
 * Return the number of jiffies (1/60 of a second) since last reset time
 */
SYSTEMCALL long timers_jiffies() {
    // return jiffy_count;
    return *TIMER_VALUE_3;
}
