/*
 * Define the various GABE control registers
 */

#ifndef __GABE_REG_H
#define __GABE_REG_H

#define POWER_ON_LED		0x0001
#define SDCARD_LED			0x0002
#define BUZZER_CONTROL		0x0004
#define MANUAL_RESET		0x8000		// Make sure the word "DEAD" is written in GABE_RESET_ID

#define GABE_CTRL_REG		((volatile unsigned short *)0x00C00000)
#define GABE_RESET_ID		((volatile unsigned short *)0x00C00002)
#define GABE_LFSR_REG0		((volatile unsigned short *)0x00C00004)
#define GABE_LFSR_REG1		((volatile unsigned short *)0x00C00006)

#define RGB_LED_L			((volatile unsigned short *)0x00C00008) // Writing Only - A2560K Only - 0x__RR
#define RGB_LED_H			((volatile unsigned short *)0x00C0000A) // Writing Only - A2560K Only - 0xGGBB

#define GABE_LFSR_DATA		((volatile unsigned short *)0x00C00008) // Read Only
#define GABE_LFSR_STAT		((volatile unsigned short *)0x00C0000A) // Read Only
#define GABE_MACHINE_ID	    ((volatile unsigned short *)0x00C0000C) // Machine ID - Read Only

#define GABE_CHIP_SUBREV	((volatile unsigned short *)0x00C0000E)
#define GABE_CHIP_VERSION   ((volatile unsigned short *)0x00C00010)
#define GABE_CHIP_NUMBER	((volatile unsigned short *)0x00C00012)

#endif
