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

/*
 * Set the keyboard translation tables
 *
 * The translation tables provided to the keyboard consist of eight
 * consecutive tables of 128 characters each. Each table maps from
 * the MAKE scan code of a key to its appropriate 8-bit character code.
 *
 * The tables included must include, in order:
 * - UNMODIFIED: Used when no modifier keys are pressed or active
 * - SHIFT: Used when the SHIFT modifier is pressed
 * - CTRL: Used when the CTRL modifier is pressed
 * - CTRL-SHIFT: Used when both CTRL and SHIFT are pressed
 * - CAPSLOCK: Used when CAPSLOCK is down but SHIFT is not pressed
 * - CAPSLOCK-SHIFT: Used when CAPSLOCK is down and SHIFT is pressed
 * - ALT: Used when only ALT is presse
 * - ALT-SHIFT: Used when ALT is pressed and either CAPSLOCK is down
 *   or SHIFT is pressed (but not both)
 *
 * Inputs:
 * tables = pointer to the keyboard translation tables
 */
extern short kbd_layout(const char * tables);

/*
 * Query the mouse for an update packet (use if we aren't using interrupts)
 *
 * Returns:
 * 0 on success, any other number is an error
 */
extern short ps2_mouse_get_packet();

/*
 * Set the visibility of the VICKY mouse pointer
 *
 * Input:
 * is_visible = 0 for hide, any other value to show
 */
extern void mouse_set_visible(short is_visible);

#endif
