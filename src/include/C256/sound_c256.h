/*
 * Sound device register definitions for the C256
 */

#ifndef __SOUND_C256_H
#define __SOUND_C256_H

#include <stdint.h>

struct s_sid_voice {
	uint16_t frequency;
	uint16_t pulse_width;
	uint8_t control;
	uint8_t attack_decay;
	uint8_t sustain_release;
};

struct s_sid {
	struct s_sid_voice v1;
	struct s_sid_voice v2;
	struct s_sid_voice v3;

	uint16_t filter_frequency;
	uint8_t resonance_filter;
	uint8_t mode_volume;
	
	uint8_t pot_x;
	uint8_t pot_y;
	uint8_t osc3;
	uint8_t env3;
};

#define PSG_PORT        ((volatile __attribute__((far)) uint8_t *)0xaff100) 	/* Control register for the SN76489 */
#define OPL3_PORT       ((volatile unsigned char *)0xFEC20200)  /* Access port for the OPL3 */
// #define OPM_EXT_BASE    ((volatile unsigned char *)0xFEC20600)  /* External OPM base address */
// #define OPN2_EXT_BASE   ((volatile unsigned char *)0xFEC20400)  /* External OPN2 base address */
// #define OPM_INT_BASE    ((volatile unsigned char *)0xFEC20C00)  /* Internal OPM base address */
// #define OPN2_INT_BASE   ((volatile unsigned char *)0xFEC20A00)  /* Internal OPN2 base address */
#define CODEC           ((volatile __attribute__((far)) uint16_t *)0xafe900)	/* Data register for the CODEC */
#define CODEC_WR_CTRL	((volatile __attribute__((far)) uint8_t *)0xafe902)		/* Data register for the CODEC */

/*
 * Internal SID
 */

#define SID_INT_N_V1_FREQ_LO  	((volatile __attribute__((far)) uint8_t *)0xafe400)
#define SID_INT_N  				((volatile __attribute__((far)) struct s_sid *)0xafe400)

#endif
