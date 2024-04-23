/**
 * @file gabe_a2560x.h
 *
 * Define miscellaneous GABE registers
 */

#ifndef __GABE_A2560X_H
#define __GABE_A2560X_H

// These are 32bits Wide Registers since they are internal to the VICKY III
#define GABE_CTRL_REG		((volatile unsigned int *)0xFEC00000)
#define POWER_ON_LED		0x00000001
#define SDCARD_LED			0x00000002
#define BUZZER_CONTROL		0x00000010
#define MANUAL_RESET		0x00008000		// Make sure the word "DEAD" is written in GABE_RESET_ID

#define GABE_LFSR_REG0		((volatile unsigned int *)0xFEC00004)

#define RGB_LED			    ((volatile unsigned int *)0xFEC00008)    // Writing Only - A2560K Only - 0x__RRGGBB
#define GABE_LFSR_STATDATA	((volatile unsigned int *)0xFEC00008)    // Read Only

#define GABE_SUBVER_ID	    ((volatile unsigned int *)0xFEC0000C)    // Subversion[31:16], Machine ID[3:2] - Read Only
#define GABE_CPU_ID_MASK    0x0000FF00
#define GABE_CPU_SPD_MASK   0x000000E0
#define GABE_ID_MASK        0x0000000C
#define GABE_CHIP_SV_MASK   0xFFFF0000

#define GABE_CHIP_VERSION   ((volatile unsigned int *)0xFEC00010)    // Number[31:16], Version[15:0]
#define GABE_CHIP_V_MASK    0x0000FFFF
#define GABE_CHIP_N_MASK    0xFFFF0000


#define GABE_FIRMWARE_DATE  ((volatile unsigned int *)0xFEC00014)    // xxDDMMYY
#define GABE_FIRMWARE_DATE_YEAR_MASK  0x000000FF
#define GABE_FIRMWARE_DATE_MONTH_MASK 0x0000FF00
#define GABE_FIRMWARE_DATE_DAY_MASK   0x00FF0000

// This applies to GenX and A2560X ONLY
#define GABE_SUB_MODEL_FF_ID   ((volatile unsigned short *)0xFEC00514)    
#define GABE_SUB_FF_MASK  0x0003  /* Mask for the sub-model: PB, LB, CU*/

#define GABE_SUB_MODEL_ID   ((volatile unsigned short *)0xFEC00516)
#define GABE_SUB_MOD_MODEL 0x000F  /* This indicate What is the Model of the Unit*/

// Those registers in the GenX/A2560X are 16bits Wide.
#define GABE_DIP_REG        ((volatile unsigned short *)0xFEC00518)
#define GABE_DIP_BOOT_MASK  0x0003  /* Mask for the boot mode: */
#define GABE_DIP_USER_MASK  0x0300  /* Mask for the user switches: */


#define GABE_GENX_STAT_LEDS        ((volatile unsigned long *)0xFEC0000C)
#define GABE_GENX_STAT_LED_0_B     0x00000001  /* LED 0 (Close to DEL key) -- Blue ON */
#define GABE_GENX_STAT_LED_0_G     0x00000002  /* LED 0 (Close to DEL key) -- Green ON */
#define GABE_GENX_STAT_LED_0_R     0x00000004  /* LED 0 (Close to DEL key) -- Red ON */
#define GABE_GENX_STAT_LED_1_B     0x00000008  /* LED 1 (Below #0) -- Blue ON */
#define GABE_GENX_STAT_LED_1_G     0x00000010  /* LED 1 (Below #0) -- Green ON */
#define GABE_GENX_STAT_LED_1_R     0x00000020  /* LED 1 (Below #0) -- Red ON */
#define GABE_GENX_STAT_LED_2_B     0x00000040  /* LED 2 (above arrow) -- Blue ON */
#define GABE_GENX_STAT_LED_2_G     0x00000080  /* LED 2 (above arrow) -- Green ON */
#define GABE_GENX_STAT_LED_2_R     0x00000100  /* LED 2 (above arrow) -- Red ON */


#endif
