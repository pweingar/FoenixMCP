#include "syscalls.h"
#include "interrupt.h"
#include "sys_general.h"
#include "timers.h"
#include "dev/channel.h"
#include "dev/block.h"
#include "dev/ps2.h"
#include "dev/rtc.h"

/**
 * @brief Translation file to map system calls into the irregularly named implementations.
 * 
 */

/*
 * Get the number of jiffies since the system last booted.
 *
 * Returns:
 * the number of jiffies since the last reset
 */
long time_jiffies() {
	return timers_jiffies();
}

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
void get_info(p_sys_info info) {
	sys_get_information(info);
}

/*
 * Return the next scan code from the keyboard... 0 if nothing pending
 */
unsigned short kbd_scancode() {
    return kbd_get_scancode();
}