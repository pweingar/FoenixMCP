/**
 * Implementation of the console channel device
 *
 * The console maps to the main screen and keyboard.
 *
 */

#ifndef __CONSOLE_H
#define __CONSOLE_H

#define CON_IOCTRL_ANSI_ON  0x01            /* IOCTRL Command: turn on ANSI terminal codes */
#define CON_IOCTRL_ANSI_OFF 0x02            /* IOCTRL Command: turn off ANSI terminal codes */
#define CON_IOCTRL_ECHO_ON  0x03            /* IOCTRL Command: turn on echo of input characters */
#define CON_IOCTRL_ECHO_OFF 0x04            /* IOCTRL Command: turn off echo of input characters */
#define CON_IOCTRL_BREAK    0x05            /* IOCTRL Command: return the status of the keyboard BREAK */
#define CON_IOCTRL_CURS_ON  0x06            /* IOCTRL Command: show the cursor */
#define CON_IOCTRL_CURS_OFF 0x07            /* IOCTRL Command: hide the cursor */

//
// Install the console device driver
//
extern short con_install();

#endif
