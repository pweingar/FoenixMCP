/*
 * Declarations for Mo, the built-in keyboard of the A2560K
 */

#ifndef __KBD_MO_H
#define __KBD_MO_H

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

#endif
