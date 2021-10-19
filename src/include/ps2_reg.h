/*
 * Include file for the PS/2 registers
 */


#ifndef __PS2_REG_H
#define __PS2_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "A2560K/ps2_a2560k.h"
#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/ps2_a2560u.h"
#endif

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
 * Controller responses
 */

#define PS2_RESP_OK         0x55    /* Keyboard response: Command was OK */
#define PS2_RESP_ACK        0xFA    /* Keyboard response: command acknowledged */

/*
 * PS2 and Keyboard Commands
 */

#define PS2_CTRL_READCMD    0x20    /* Read the command byte */
#define PS2_CTRL_WRITECMD   0x60    /* Write the command byte */
#define PS2_CTRL_SELFTEST   0xAA    /* Test the PS/2 controller */
#define PS2_CTRL_KBDTEST    0xAB    /* Test the main (keyboard) port */
#define PS2_CTRL_MOUSETEST  0xA9    /* Test the secondary (mouse) port */
#define PS2_CTRL_ENABLE_1   0xAE    /* Enable the first (keyboard) port */
#define PS2_CTRL_DISABLE_1  0xAD    /* Disable the first (keyboard) port */
#define PS2_CTRL_ENABLE_2   0xA9    /* Enable the second (mouse) port */
#define PS2_CTRL_DISABLE_2  0xA7    /* Disable the second (mouse) port */

#define KBD_CMD_RESET       0xFF    /* Keyboard command: reset the keyboard */
#define KBD_CMD_ENABLE      0xF4    /* Keyboard command: enable to keyboard */
#define KBD_CMD_SET_LED     0xED    /* Keyboard command: set the LEDs */

#define MOUSE_CMD_PREFIX    0xD4    /* Controller code to prefix all mouse commands */
#define MOUSE_CMD_SETRES    0xE8    /* Mouse command: set resolution */
#define MOUSE_CMD_REQPACK   0xEB    /* Mouse command: request a packet */
#define MOUSE_CMD_ENABLE    0xF4    /* Mouse command: enable streaming */
#define MOUSE_CMD_DISABLE   0xF5    /* Mouse command: disable streaming */
#define MOUSE_CMD_DEFAULTS  0xF6    /* Mouse command: set defaults */
#define MOUSE_CMD_RESET     0xFF    /* Mouse command: reset the mouse */

#endif
