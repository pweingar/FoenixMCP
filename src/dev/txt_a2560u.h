/** @file txt_a2560u.h
 *
 * Text screen driver for the A2560U
 */

#ifndef __TXT_A2560U_H
#define __TXT_A2560U_H

/* We only have one screen */
#define TXT_SCREEN_A2560U 0

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
extern short txt_a2560u_install();

#endif
