/** @file txt_a2560k_b.h
 *
 * Text screen driver for A2560K Channel B
 */

#ifndef __TXT_A2560K_B_H
#define __TXT_A2560K_B_H

/* Channel B is assigned to screen #0, it will be considered the primary channel */
#define TXT_SCREEN_A2560K_B 0

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
extern short txt_a2560k_b_install();

#endif
