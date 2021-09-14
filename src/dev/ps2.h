/**
 * Definitions for PS/2 interface mouse and keyboard.
 */

#ifndef __PS2_H
#define __PS2_H

#include "constants.h"
#include "types.h"
#include "ps2_reg.h"

/*
 * ps2_init status code flags
 */

#define PS2_FAIL_SELFTEST   -1
#define PS2_FAIL_KBDTEST    -2

#define PS2_KBD_MISSING     0x0001
#define PS2_MOUSE_MISSING   0x0002

/*
 * Initialize the PS2 controller and any attached devices
 * Enable keyboard and mouse interrupts as appropriate.
 *
 * Returns:
 *  Status code indicating if either the mouse or the keyboard is missing.
 */
extern short ps2_init();

/*
 * Try to retrieve the next scancode from the keyboard.
 *
 * Returns:
 *      The next scancode to be processed, 0 if nothing.
 */
extern unsigned short kbd_get_scancode();

/*
 * Try to get a character from the keyboard...
 *
 * Returns:
 *      the next character to be read from the keyboard (0 if none available)
 */
extern char kbd_getc();

extern char kbd_getc_poll();

#endif
