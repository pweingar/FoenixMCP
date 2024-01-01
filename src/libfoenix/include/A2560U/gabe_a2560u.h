/**
 * @file gabe_a2560u.h
 *
 * Define miscellaneous GABE registers
 */

#ifndef __GABE_A2560U_H
#define __GABE_A2560U_H

#define GABE_CTRL_REG		((volatile unsigned short *)0x00B00000)
#define POWER_ON_LED		0x0001
#define SDCARD_LED			0x0002
#define BUZZER_CONTROL		0x0010
#define MANUAL_RESET		0x8000		// Make sure the word "DEAD" is written in GABE_RESET_ID

#define GABE_RESET_ID		((volatile unsigned short *)0x00B00002)
#define GABE_LFSR_REG0		((volatile unsigned short *)0x00B00004)
#define GABE_LFSR_REG1		((volatile unsigned short *)0x00B00006)

#define RGB_LED_L			((volatile unsigned short *)0x00B00008) // Writing Only - A2560K Only - 0x__RR
#define RGB_LED_H			((volatile unsigned short *)0x00B0000A) // Writing Only - A2560K Only - 0xGGBB

#define GABE_LFSR_DATA		((volatile unsigned short *)0x00B00008) // Read Only
#define GABE_LFSR_STAT		((volatile unsigned short *)0x00B0000A) // Read Only
#define GABE_MACHINE_ID	    ((volatile unsigned short *)0x00B0000C) // Machine ID - Read Only
#define GABE_MACH_ID_MASK   0x0003  // 1001 = A2560U and U+
#define GABE_MEMORY_BANKS   0x00C0  // 10 = 2 MB, 11 = 4 MB
#define GABE_CPU_SPEED      0x0F00
#define GABE_CPU_ID         0xF000  // 0000 = MC68SEC000 @ 20MHz

#define GAVIN_ATARI_JOYSTICK ((volatile unsigned short *)0x00B00500)
#define JOY1_UP             0x0001
#define JOY1_DOWN           0x0002
#define JOY1_LEFT           0x0004
#define JOY1_RIGHT          0x0008
#define JOY1_BTN0           0x0010
#define JOY1_BTN1           0x0040
#define JOY1_BTN2           0x0080
#define JOY2_UP             0x0100
#define JOY2_DOWN           0x0200
#define JOY2_LEFT           0x0400
#define JOY2_RIGHT          0x0800
#define JOY2_BTN0           0x1000
#define JOY2_BTN1           0x4000
#define JOY2_BTN2           0x8000

#define FPGA_DATE_YEAR      ((volatile unsigned short *)0x00B40030) // YYYY
#define FPGA_DATE_MONTHDAY  ((volatile unsigned short *)0x00B40032) // MMDD

#define PCB_REV_1           ((volatile unsigned short *)0x00B40034) // Null terminated string describing the PCB
#define PCB_REV_2           ((volatile unsigned short *)0x00B40036)

#define FPGA_SUBVER         ((volatile unsigned short *)0x00B40038)
#define FPGA_VER            ((volatile unsigned short *)0x00B4003A)
#define FPGA_MODEL_L        ((volatile unsigned short *)0x00B4003C)
#define FPGA_MODEL_H        ((volatile unsigned short *)0x00B4003E)

#define GABE_CHIP_SUBREV	((volatile unsigned short *)0x00B0000E)
#define GABE_CHIP_VERSION   ((volatile unsigned short *)0x00B00010)
#define GABE_CHIP_NUMBER	((volatile unsigned short *)0x00B00012)

#define GABE_DIP_REG        ((volatile unsigned short *)0x00B00518)
#define GABE_DIP_BOOT_MASK  0x0003      /* Mask for the boot mode: */
#define GABE_DIP_USER_MASK  0x0300      /* Mask for the user switches: */

#endif