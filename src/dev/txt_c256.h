/** @file txt_c2560.h
 *
 * Text screen driver for the C256
 */

#ifndef __TXT_C256_H
#define __TXT_C256_H

/* We only have one screen */
#define TXT_SCREEN_C256 0

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
extern short txt_c256_install();

#endif
