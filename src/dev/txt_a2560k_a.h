/** @file txt_a2560k_a.h
 *
 * Text screen driver for A2560K Channel A
 */

#ifndef __TXT_A2560K_A_H
#define __TXT_A2560K_A_H

/* Channel A is assigned to screen #1, it will be considered a secondary channel */
#define TXT_SCREEN_A2560K_A 1

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
extern short txt_a2560k_a_install();

#endif
