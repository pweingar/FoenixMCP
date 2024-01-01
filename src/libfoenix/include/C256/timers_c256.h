/**
 * @file timers_c256.h
 *
 * Define timer registers on the C256
 */

#ifndef __C256_TIMERS_H
#define __C256_TIMERS_H

//
// TIMER_CTRL_* flags
//

#define TIMER_CTRL_EN		0x01	// Set to enable timer
#define TIMER_CTRL_SCLR		0x02	// Set to clear the timer for counting up
#define TIMER_CTRL_SLOAD	0x04	// Set to load the timer for counting down
#define TIMER_CTRL_CNT_UP	0x08	// Set to count up, clear to count down

//
// TIMER_CMP_* flags
//

#define TIMER_CMP_RECLR		0x01	// Set to reclear the register on reaching the comparison value
#define TIMER_CMP_RELOAD	0x02	// Set to reload the charge value on reaching 0

//
// Timer 0 and 1 -- Based on system clock (14318180Hz)
//

#define TIMER_CTRL_0    ((volatile __attribute__((far)) uint8_t *)0x000160)
#define TIMER_CHG_L_0	((volatile __attribute__((far)) uint8_t *)0x000161)
#define TIMER_CHG_M_0	((volatile __attribute__((far)) uint8_t *)0x000162)
#define TIMER_CHG_H_0	((volatile __attribute__((far)) uint8_t *)0x000163)
#define TIMER_CMPC_0	((volatile __attribute__((far)) uint8_t *)0x000164)
#define TIMER_CMP_L_0	((volatile __attribute__((far)) uint8_t *)0x000165)
#define TIMER_CMP_M_0	((volatile __attribute__((far)) uint8_t *)0x000166)
#define TIMER_CMP_H_0	((volatile __attribute__((far)) uint8_t *)0x000167)

#define TIMER_CTRL_1    ((volatile __attribute__((far)) uint8_t *)0x000168)
#define TIMER_CHG_L_1	((volatile __attribute__((far)) uint8_t *)0x000169)
#define TIMER_CHG_M_1	((volatile __attribute__((far)) uint8_t *)0x00016a)
#define TIMER_CHG_H_1	((volatile __attribute__((far)) uint8_t *)0x00016b)
#define TIMER_CMPC_1	((volatile __attribute__((far)) uint8_t *)0x00016c)
#define TIMER_CMP_L_1	((volatile __attribute__((far)) uint8_t *)0x00016d)
#define TIMER_CMP_M_1	((volatile __attribute__((far)) uint8_t *)0x00016e)
#define TIMER_CMP_H_1	((volatile __attribute__((far)) uint8_t *)0x00016f)

//
// Timer 2 -- Based on start of frame clock (60/70 Hz)
//

#define TIMER_CTRL_2    ((volatile __attribute__((far)) uint8_t *)0x000170)
#define TIMER_CHG_L_2	((volatile __attribute__((far)) uint8_t *)0x000171)
#define TIMER_CHG_M_2	((volatile __attribute__((far)) uint8_t *)0x000172)
#define TIMER_CHG_H_2	((volatile __attribute__((far)) uint8_t *)0x000173)
#define TIMER_CMPC_2	((volatile __attribute__((far)) uint8_t *)0x000174)
#define TIMER_CMP_L_2	((volatile __attribute__((far)) uint8_t *)0x000175)
#define TIMER_CMP_M_2	((volatile __attribute__((far)) uint8_t *)0x000176)
#define TIMER_CMP_H_2	((volatile __attribute__((far)) uint8_t *)0x000177)

#endif
