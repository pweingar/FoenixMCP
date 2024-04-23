/**
 * @file expansion.h
 * @brief Register definitions for the C256 expansion card
 * @version 0.1
 * @date 2023-08-30
 * 
 */

#ifndef __EXPANSION_H_
#define __EXPANSION_H_

#include <stdint.h>
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS

struct s_c256_exp_card {
	char card_name[16];					// ASCII-Z name of card (max 15 characters)
	uint16_t vendor_id;					// ID of vendor
	uint16_t card_id;					// ID of card
	uint16_t class_id;					// TBD
	uint16_t subclass_id;				// TBD
	uint16_t rsrv1;						// Reserved
	uint8_t hardware_revision;			// 00 in hex
	uint8_t fpga_rev;					// 00 in hex
	uint16_t rsrv2;						// Reserved
	uint16_t chksum;					// Not implemented
};

#define EXP_CARD_INFO		((volatile __attribute__((far)) struct s_c256_exp_card *)0xae0000)

/**
 * Card Vendor IDs (currently there is only an ID for Foenix Retro Systems)
 */

#define EXP_VENDOR_FOENIX	0xf0e1

/**
 * Expansion Card IDs (currently only the EVID and ESID cards are supported)
 */

#define EXP_CARD_C100_ESID	0x9172
#define EXP_CARD_C200_EVID	0x00c8

#endif

#endif