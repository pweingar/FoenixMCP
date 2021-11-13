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
extern char kbdmo_getc();

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

#endif

#endif
