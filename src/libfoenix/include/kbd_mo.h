/*
 * Declarations for Mo, the built-in keyboard of the A2560K
 */

#ifndef __KBD_MO_H
#define __KBD_MO_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K

/*
 * Initialize the PS2 controller and any attached devices
 * Enable keyboard and mouse interrupts as appropriate.
 *
 * Returns:
 *  Status code indicating if either the mouse or the keyboard is missing.
 */
extern short kbdmo_init();

/**
 * Set the color of the A2560K keyboard LED matrix
 *
 * @param row the number of the row to set (0 - 5)
 * @param color the color for the LEDs: ARGB
 */
void kbdmo_set_led_matrix_row(unsigned char row, unsigned short color);

/**
 * Set all the LEDs to the same color
 *
 * @param color the color for the LEDs: ARGB
 */
void kbdmo_set_led_matrix_fill(unsigned short color);

/*
 * Check to see if a BREAK code has been pressed recently
 * If so, return true and reset the internal flag.
 *
 * BREAK will be F-ESC on the A2560K
 *
 * Returns:
 * true if a BREAK has been pressed since the last check
 */
extern bool kbdmo_break();

/*
 * Try to retrieve the next scancode from the keyboard.
 *
 * Returns:
 *      The next scancode to be processed, 0 if nothing.
 */
extern unsigned short kbdmo_get_scancode();

/*
 * Try to get a character from the keyboard...
 *
 * Returns:
 *      the next character to be read from the keyboard (0 if none available)
 */
extern unsigned char kbdmo_getc();

/*
 * Use polling to fetch a key
 */
extern char kbdmo_getc_poll();

/*
 * Use polling to fetch a scan code
 */
extern unsigned short kbdmo_get_scancode_poll();

extern void kbdmo_handle_irq();

/*
 * Set the color of the LED for the capslock
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void kbdmo_set_caps_led(short colors);

/*
 * Set the color of the LED for the floppy drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void kbdmo_set_fdc_led(short colors);

/*
 * Set the color of the LED for the SD card slot
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void kbdmo_set_sdc_led(short colors);

/*
 * Set the color of the LED for the IDE hard drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void kbdmo_set_hdc_led(short colors);

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
extern short kbdmo_layout(const char * tables);

#endif

#endif
