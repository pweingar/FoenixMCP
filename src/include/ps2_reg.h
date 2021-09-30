/*
 * Include file for the PS/2 registers
 */


#ifndef __PS2_REG_H
#define __PS2_REG_H

#include "sys_general.h"

/*
 * Status flags
 */

#define PS2_STAT_OBF    0x01
#define PS2_STAT_IBF    0x02
#define PS2_STAT_SYS	0x04
#define PS2_STAT_CMD	0x08
#define PS2_STAT_INH    0x10
#define PS2_STAT_TTO	0x20
#define PS2_STAT_RTO	0x40
#define PS2_STAT_PE     0x80

/*
 * PS2 and Keyboard Commands
 */

 #define PS2_CTRL_WRITECMD   0x60
 #define PS2_CTRL_SELFTEST   0xAA
 #define PS2_CTRL_KBDTEST    0xAB
 #define PS2_CTRL_ENABLE_1   0xAE
 #define PS2_CTRL_DISABLE_1  0xAD
 #define PS2_CTRL_ENABLE_2   0xA9
 #define PS2_CTRL_DISABLE_2  0xA7

 #define KBD_CMD_RESET       0xFF    /* Keyboard command: reset the keyboard */
 #define KBD_CMD_ENABLE      0xF4    /* Keyboard command: enable to keyboard */
 #define KBD_CMD_SET_LED     0xED    /* Keyboard command: set the LEDs */

// #if SYSTEM == SYS_FOENIX_U || SYSTEM == SYS_FOENIX_U_PLUS
// #define PS2_STATUS      ((unsigned char *)0xAF1807)
// #define PS2_CMD_BUF     ((unsigned char *)0xAF1807)
// #define PS2_OUT_BUF     ((unsigned char *)0xAF1803)
// #define PS2_INPT_BUF    ((unsigned char *)0xAF1803)
// #define PS2_DATA_BUF    ((unsigned char *)0xAF1803)
//
// #elif SYSTEM == SYS_FOENIX_FMX
// #define PS2_STATUS      ((unsigned char *)0xAF1064)
// #define PS2_CMD_BUF     ((unsigned char *)0xAF1064)
// #define PS2_OUT_BUF     ((unsigned char *)0xAF1060)
// #define PS2_INPT_BUF    ((unsigned char *)0xAF1060)
// #define PS2_DATA_BUF    ((unsigned char *)0xAF1060)
//
// #elif SYSTEM == SYS_FOENIX_A2560K
#define PS2_STATUS      ((unsigned char *)0x00C02064)
#define PS2_CMD_BUF     ((unsigned char *)0x00C02064)
#define PS2_OUT_BUF     ((unsigned char *)0x00C02060)
#define PS2_INPT_BUF    ((unsigned char *)0x00C02060)
#define PS2_DATA_BUF    ((unsigned char *)0x00C02060)
// #endif

#endif
