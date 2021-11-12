/*
 * Define the various GABE control registers
 */

#ifndef __GABE_REG_H
#define __GABE_REG_H

#include "sys_general.h"

#define POWER_ON_LED		0x0001
#define SDCARD_LED			0x0002
#define BUZZER_CONTROL		0x0004
#define MANUAL_RESET		0x8000		// Make sure the word "DEAD" is written in GABE_RESET_ID

#if MODEL == MODEL_FOENIX_A2560K
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

#define GABE_DIP_REG        ((volatile unsigned short *)0x00C00518)
#define GABE_DIP_BOOT_MASK  0x0003      /* Mask for the boot mode: */
#define GABE_DIP_USER_MASK  0x0300      /* Mask for the user switches: */

/*
 * 16-bit register controlling the key of the A2560K built-in keyboard
 */

#define GABE_MO_LEDS        ((volatile unsigned short *)0x00C0000C)
#define GABE_MO_LED_0_B     0x0001      /* LED 0 (Close to DEL key) -- Blue ON */
#define GABE_MO_LED_0_G     0x0002      /* LED 0 (Close to DEL key) -- Green ON */
#define GABE_MO_LED_0_R     0x0004      /* LED 0 (Close to DEL key) -- Red ON */
#define GABE_MO_LED_1_B     0x0008      /* LED 1 (Below #0) -- Blue ON */
#define GABE_MO_LED_1_G     0x0010      /* LED 1 (Below #0) -- Green ON */
#define GABE_MO_LED_1_R     0x0020      /* LED 1 (Below #0) -- Red ON */
#define GABE_MO_LED_2_B     0x0040      /* LED 2 (above arrow) -- Blue ON */
#define GABE_MO_LED_2_G     0x0080      /* LED 2 (above arrow) -- Green ON */
#define GABE_MO_LED_2_R     0x0100      /* LED 2 (above arrow) -- Red ON */
#define GABE_MO_LED_3_B     0x0200      /* LED 2 (caps lock, Rev C) -- Blue ON */
#define GABE_MO_LED_3_G     0x0400      /* LED 2 (caps lock, Rev C) -- Green ON */
#define GABE_MO_LED_3_R     0x0800      /* LED 2 (caps lock, Rev C) -- Red ON */

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS

#define GABE_CTRL_REG		((volatile unsigned short *)0x00B00000)
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
#endif
