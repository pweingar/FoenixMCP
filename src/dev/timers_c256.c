#include <stdint.h>

#include "interrupt.h"
#include "timers.h"
#include "gabe_reg.h"
#include "timers_reg.h"

long jiffy_count;

/**
 * @brief Interrupt handler for the timer 0 interrupt... counts jiffies since boot
 * 
 */
void timer_0_handler() {
	jiffy_count++;
}

/*
 * Initialize the timers and their interrupts
 */
void timers_init() {
	// Number of system clock ticks per second
	const uint32_t sys_clock = 14318180;

	// Number of system clocks per jiffy
	const uint32_t jiffy_target = sys_clock / (uint32_t)60;

    jiffy_count = 0;

	// Disable all timers
	*TIMER_CTRL_0 = 0;
	*TIMER_CTRL_1 = 0;
	*TIMER_CTRL_2 = 0;

	// Register our timer 0 interrupt handler
	int_register(INT_SOF_A, timer_0_handler);
	int_enable(INT_SOF_A);

	// Set timer 0 to tick every jiffy

	// *TIMER_CHG_L_0 = jiffy_target & 0xff;
	// *TIMER_CHG_M_0 = (jiffy_target >> 8) & 0xff;
	// *TIMER_CHG_H_0 = (jiffy_target >> 16) & 0xff;
	
	// *TIMER_CMP_L_0 = 0;
	// *TIMER_CMP_M_0 = 0;
	// *TIMER_CMP_H_0 = 0;

	// *TIMER_CMPC_0 = TIMER_CMP_RELOAD;
	// *TIMER_CTRL_0 = TIMER_CTRL_EN | TIMER_CTRL_SLOAD;
}

/*
 * Return the number of jiffies (1/60 of a second) since last reset time
 */
long timers_jiffies() {
    return jiffy_count;
}
