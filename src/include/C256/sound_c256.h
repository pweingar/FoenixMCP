/*
 * Sound device register definitions for the A2560K
 */

#ifndef __SOUND_C256_H
#define __SOUND_C256_H

#include <stdint.h>

#define PSG_PORT        ((volatile __attribute__((far)) uint8_t *)0xaff100) 	/* Control register for the SN76489 */
// #define PSG_INT_L_PORT  ((volatile unsigned char *)0xFEC20110)  /* Control register for the SN76489 */
// #define PSG_INT_R_PORT  ((volatile unsigned char *)0xFEC20120)  /* Control register for the SN76489 */
// #define PSG_INT_S_PORT  ((volatile unsigned char *)0xFEC20130)  /* Control register for the SN76489 */
// #define OPL3_PORT       ((volatile unsigned char *)0xFEC20200)  /* Access port for the OPL3 */
// #define OPM_EXT_BASE    ((volatile unsigned char *)0xFEC20600)  /* External OPM base address */
// #define OPN2_EXT_BASE   ((volatile unsigned char *)0xFEC20400)  /* External OPN2 base address */
// #define OPM_INT_BASE    ((volatile unsigned char *)0xFEC20C00)  /* Internal OPM base address */
// #define OPN2_INT_BASE   ((volatile unsigned char *)0xFEC20A00)  /* Internal OPN2 base address */
#define CODEC           ((volatile __attribute__((far)) uint16_t *)0xafe900)	/* Data register for the CODEC */
#define CODEC_WR_CTRL	((volatile __attribute__((far)) uint8_t *)0xafe902)		/* Data register for the CODEC */

/*
 * Internal SID Left Channel
 */

// #define SID_INT_L_V1_FREQ_LO  	((unsigned char *)0xFEC21000)
// #define SID_INT_L_V1_FREQ_HI  	((unsigned char *)0xFEC21001)
// #define SID_INT_L_V1_PW_LO		((unsigned char *)0xFEC21002)
// #define SID_INT_L_V1_PW_HI		((unsigned char *)0xFEC21003)
// #define SID_INT_L_V1_CTRL		((unsigned char *)0xFEC21004)
// #define SID_INT_L_V1_ATCK_DECY  ((unsigned char *)0xFEC21005)
// #define SID_INT_L_V1_SSTN_RLSE  ((unsigned char *)0xFEC21006)
// #define SID_INT_L_V2_FREQ_LO    ((unsigned char *)0xFEC21007)
// #define SID_INT_L_V2_FREQ_HI    ((unsigned char *)0xFEC21008)
// #define SID_INT_L_V2_PW_LO      ((unsigned char *)0xFEC21009)
// #define SID_INT_L_V2_PW_HI		((unsigned char *)0xFEC2100A)
// #define SID_INT_L_V2_CTRL       ((unsigned char *)0xFEC2100B)
// #define SID_INT_L_V2_ATCK_DECY  ((unsigned char *)0xFEC2100C)
// #define SID_INT_L_V2_SSTN_RLSE  ((unsigned char *)0xFEC2100D)
// #define SID_INT_L_V3_FREQ_LO    ((unsigned char *)0xFEC2100E)
// #define SID_INT_L_V3_FREQ_HI    ((unsigned char *)0xFEC2100F)
// #define SID_INT_L_V3_PW_LO      ((unsigned char *)0xFEC21010)
// #define SID_INT_L_V3_PW_HI		((unsigned char *)0xFEC21011)
// #define SID_INT_L_V3_CTRL       ((unsigned char *)0xFEC21012)
// #define SID_INT_L_V3_ATCK_DECY  ((unsigned char *)0xFEC21013)
// #define SID_INT_L_V3_SSTN_RLSE  ((unsigned char *)0xFEC21014)
// #define SID_INT_L_FC_LO			((unsigned char *)0xFEC21015)
// #define SID_INT_L_FC_HI         ((unsigned char *)0xFEC21016)
// #define SID_INT_L_RES_FILT      ((unsigned char *)0xFEC21017)
// #define SID_INT_L_MODE_VOL      ((unsigned char *)0xFEC21018)
// #define SID_INT_L_POT_X         ((unsigned char *)0xFEC21019)
// #define SID_INT_L_POT_Y         ((unsigned char *)0xFEC2101A)
// #define SID_INT_L_OSC3_RND      ((unsigned char *)0xFEC2101B)
// #define SID_INT_L_ENV3          ((unsigned char *)0xFEC2101C)
// #define SID_INT_L_NOT_USED0     ((unsigned char *)0xFEC2101D)
// #define SID_INT_L_NOT_USED1     ((unsigned char *)0xFEC2101E)
// #define SID_INT_L_NOT_USED2     ((unsigned char *)0xFEC2101F)

/*
 * Internal SID Right Channel
 */

// #define SID_INT_R_V1_FREQ_LO  	((unsigned char *)0xFEC21200)
// #define SID_INT_R_V1_FREQ_HI  	((unsigned char *)0xFEC21201)
// #define SID_INT_R_V1_PW_LO		((unsigned char *)0xFEC21202)
// #define SID_INT_R_V1_PW_HI		((unsigned char *)0xFEC21203)
// #define SID_INT_R_V1_CTRL		((unsigned char *)0xFEC21204)
// #define SID_INT_R_V1_ATCK_DECY  ((unsigned char *)0xFEC21205)
// #define SID_INT_R_V1_SSTN_RLSE  ((unsigned char *)0xFEC21206)
// #define SID_INT_R_V2_FREQ_LO    ((unsigned char *)0xFEC21207)
// #define SID_INT_R_V2_FREQ_HI    ((unsigned char *)0xFEC21208)
// #define SID_INT_R_V2_PW_LO      ((unsigned char *)0xFEC21209)
// #define SID_INT_R_V2_PW_HI		((unsigned char *)0xFEC2120A)
// #define SID_INT_R_V2_CTRL       ((unsigned char *)0xFEC2120B)
// #define SID_INT_R_V2_ATCK_DECY  ((unsigned char *)0xFEC2120C)
// #define SID_INT_R_V2_SSTN_RLSE  ((unsigned char *)0xFEC2120D)
// #define SID_INT_R_V3_FREQ_LO    ((unsigned char *)0xFEC2120E)
// #define SID_INT_R_V3_FREQ_HI    ((unsigned char *)0xFEC2120F)
// #define SID_INT_R_V3_PW_LO      ((unsigned char *)0xFEC21210)
// #define SID_INT_R_V3_PW_HI		((unsigned char *)0xFEC21211)
// #define SID_INT_R_V3_CTRL       ((unsigned char *)0xFEC21212)
// #define SID_INT_R_V3_ATCK_DECY  ((unsigned char *)0xFEC21213)
// #define SID_INT_R_V3_SSTN_RLSE  ((unsigned char *)0xFEC21214)
// #define SID_INT_R_FC_LO			((unsigned char *)0xFEC21215)
// #define SID_INT_R_FC_HI         ((unsigned char *)0xFEC21216)
// #define SID_INT_R_RES_FILT      ((unsigned char *)0xFEC21217)
// #define SID_INT_R_MODE_VOL      ((unsigned char *)0xFEC21218)
// #define SID_INT_R_POT_X         ((unsigned char *)0xFEC21219)
// #define SID_INT_R_POT_Y         ((unsigned char *)0xFEC2121A)
// #define SID_INT_R_OSC3_RND      ((unsigned char *)0xFEC2121B)
// #define SID_INT_R_ENV3          ((unsigned char *)0xFEC2121C)
// #define SID_INT_R_NOT_USED0     ((unsigned char *)0xFEC2121D)
// #define SID_INT_R_NOT_USED1     ((unsigned char *)0xFEC2121E)
// #define SID_INT_R_NOT_USED2     ((unsigned char *)0xFEC2121F)

/*
 * Internal SID Neutral Channel - When writting here, the value is written in R and L Channel at the same time
 */

// #define SID_INT_N_V1_FREQ_LO  	((unsigned char *)0xFEC41200)
// #define SID_INT_N_V1_FREQ_HI  	((unsigned char *)0xFEC41201)
// #define SID_INT_N_V1_PW_LO		((unsigned char *)0xFEC41202)
// #define SID_INT_N_V1_PW_HI		((unsigned char *)0xFEC41203)
// #define SID_INT_N_V1_CTRL		((unsigned char *)0xFEC41204)
// #define SID_INT_N_V1_ATCK_DECY  ((unsigned char *)0xFEC41205)
// #define SID_INT_N_V1_SSTN_RLSE  ((unsigned char *)0xFEC41206)
// #define SID_INT_N_V2_FREQ_LO    ((unsigned char *)0xFEC41207)
// #define SID_INT_N_V2_FREQ_HI    ((unsigned char *)0xFEC41208)
// #define SID_INT_N_V2_PW_LO      ((unsigned char *)0xFEC41209)
// #define SID_INT_N_V2_PW_HI		((unsigned char *)0xFEC4120A)
// #define SID_INT_N_V2_CTRL       ((unsigned char *)0xFEC4120B)
// #define SID_INT_N_V2_ATCK_DECY  ((unsigned char *)0xFEC4120C)
// #define SID_INT_N_V2_SSTN_RLSE  ((unsigned char *)0xFEC4120D)
// #define SID_INT_N_V3_FREQ_LO    ((unsigned char *)0xFEC4120E)
// #define SID_INT_N_V3_FREQ_HI    ((unsigned char *)0xFEC4120F)
// #define SID_INT_N_V3_PW_LO      ((unsigned char *)0xFEC41210)
// #define SID_INT_N_V3_PW_HI		((unsigned char *)0xFEC41211)
// #define SID_INT_N_V3_CTRL       ((unsigned char *)0xFEC41212)
// #define SID_INT_N_V3_ATCK_DECY  ((unsigned char *)0xFEC41213)
// #define SID_INT_N_V3_SSTN_RLSE  ((unsigned char *)0xFEC41214)
// #define SID_INT_N_FC_LO			((unsigned char *)0xFEC41215)
// #define SID_INT_N_FC_HI         ((unsigned char *)0xFEC41216)
// #define SID_INT_N_RES_FILT      ((unsigned char *)0xFEC41217)
// #define SID_INT_N_MODE_VOL      ((unsigned char *)0xFEC41218)
// #define SID_INT_N_POT_X         ((unsigned char *)0xFEC41219)
// #define SID_INT_N_POT_Y         ((unsigned char *)0xFEC4121A)
// #define SID_INT_N_OSC3_RND      ((unsigned char *)0xFEC4121B)
// #define SID_INT_N_ENV3          ((unsigned char *)0xFEC4121C)
// #define SID_INT_N_NOT_USED0     ((unsigned char *)0xFEC4121D)
// #define SID_INT_N_NOT_USED1     ((unsigned char *)0xFEC4121E)
// #define SID_INT_N_NOT_USED2     ((unsigned char *)0xFEC4121F)

#endif
