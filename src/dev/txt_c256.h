/** @file txt_c256.h
 *
 * Text screen driver for C256's main screen (U, U+, and FMX)
 * The driver for the EVID will be in a separate file.
 */

#ifndef __TXT_C256_H
#define __TXT_C256_H

/* This screen is assigned to screen #0, it will be considered the primary channel */
#define TXT_SCREEN_C256 0

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
extern short txt_c256_install();

#endif
