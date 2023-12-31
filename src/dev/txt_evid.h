/**
 * @file txt_evid.c
 * @brief Low level FoenixMCP driver for the C200 EVID expansion card for the C256 computers
 * @version 0.1
 * @date 2023-08-09
 * 
 */

#ifndef __txt_evid_h__
#define __txt_evid_h__

/* If installed, the EVID is the second screen on the C256. */
#define TXT_SCREEN_EVID 1

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
extern short txt_evid_install();

#endif
