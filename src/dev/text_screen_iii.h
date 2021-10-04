#ifndef __TEXT_SCREEN_III_H
#define __TEXT_SCREEN_III_H

/*
 * Driver for VICKY III text screens, both channel A and channel B
 */

/*
 * Initialize the text screen driver
 */
extern int text_init();

/*
 * Set the cursor properties
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * color = the color index for the cursor
 * character = the character to display for the cursor
 * rate = the rate of blinking (0 = 1 per sec, 1 = 2 per sec, 2 = 4 per sec, 3 = 5 per sec)
 * enable = 1 to display the cursor, 0 to disable
 */
extern void text_set_cursor(short screen, short color, char character, short rate, short enable);

/*
 * Set the position of the cursor on the screen. Adjusts internal pointers used for printing the characters
 *
 * If the x and y coordinates are out of bounds of the display, the routine will attempt to handle wrapping and
 * scrolling accordingly.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * x = the column of the cursor (0 is left most)
 * y = the row of the cursor (0 is right most)
 */
extern void text_set_xy(short screen, unsigned short x, unsigned short y);

/*
 * Compute the size information for the text screen based on the current settings in VICKY
 * These settings are needed to correctly position text on the screen.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
extern void text_setsizes(short screen);

/*
 * Send a character to the screen without any escape code interpretation
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * c = the character to print
 */
extern void text_put_raw(short screen, char c);

/*
 * Send a character to the screen... but handle ANSI escape codes and process accordingly.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * c = the character to print
 */
void text_put_ansi(short screen, char c);

/*
 * Set the foreground and background color for printing
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * foreground = the foreground color number to use (0 - 15)
 * background = the background color number to use (0 - 15)
 */
extern void text_set_color(short screen, short foreground, short background);

/*
 * Clear the screen of data
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
extern void text_clear(short screen);

/*
 * Scroll the text screen up one row
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
extern void text_scroll(short screen);

#endif