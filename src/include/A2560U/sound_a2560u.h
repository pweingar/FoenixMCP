/*
 * Sound device register definitions for the A2560U and A2560U+
 */

#ifndef __SOUND_A2560U_H
#define __SOUND_A2560U_H

#include <stdint.h>

#define PSG_PORT        ((volatile uint8_t *)0x00B20130)  /* Control register for the SN76489 */
#define OPL3_PORT       ((volatile uint8_t *)0x00B20200)  /* Access port for the OPL3 */
#define CODEC           ((volatile uint16_t *)0x00B20E00) /* Control register for the CODEC */
#define SID_INT_L       ((volatile uint8_t *)0x00B21000)  /* Left SID base */
#define SID_INT_R       ((volatile uint8_t *)0x00B21200)  /* Right SID base */
#define SID_INT_N       ((volatile uint8_t *)0x00B21400)  /* Both SIDs */

/*
 * A2560U supports two SIDs emulated in the FPGA.
 */

/*
 * Internal SID Left Channel
 */

#define SID_INT_L_V1_FREQ_LO  	((uint8_t *)SID_INT_L)
#define SID_INT_L_V1_FREQ_HI  	((uint8_t *)SID_INT_L+1)
#define SID_INT_L_V1_PW_LO		((uint8_t *)SID_INT_L+2)
#define SID_INT_L_V1_PW_HI		((uint8_t *)SID_INT_L+3)
#define SID_INT_L_V1_CTRL		((uint8_t *)SID_INT_L+4)
#define SID_INT_L_V1_ATCK_DECY  ((uint8_t *)SID_INT_L+5)
#define SID_INT_L_V1_SSTN_RLSE  ((uint8_t *)SID_INT_L+6)
#define SID_INT_L_V2_FREQ_LO    ((uint8_t *)SID_INT_L+7)
#define SID_INT_L_V2_FREQ_HI    ((uint8_t *)SID_INT_L+8)
#define SID_INT_L_V2_PW_LO      ((uint8_t *)SID_INT_L+9)
#define SID_INT_L_V2_PW_HI		((uint8_t *)SID_INT_L+10)
#define SID_INT_L_V2_CTRL       ((uint8_t *)SID_INT_L+11)
#define SID_INT_L_V2_ATCK_DECY  ((uint8_t *)SID_INT_L+12)
#define SID_INT_L_V2_SSTN_RLSE  ((uint8_t *)SID_INT_L+13)
#define SID_INT_L_V3_FREQ_LO    ((uint8_t *)SID_INT_L+14)
#define SID_INT_L_V3_FREQ_HI    ((uint8_t *)SID_INT_L+15)
#define SID_INT_L_V3_PW_LO      ((uint8_t *)SID_INT_L+16)
#define SID_INT_L_V3_PW_HI		((uint8_t *)SID_INT_L+17)
#define SID_INT_L_V3_CTRL       ((uint8_t *)SID_INT_L+18)
#define SID_INT_L_V3_ATCK_DECY  ((uint8_t *)SID_INT_L+19)
#define SID_INT_L_V3_SSTN_RLSE  ((uint8_t *)SID_INT_L+20)
#define SID_INT_L_FC_LO			((uint8_t *)SID_INT_L+21)
#define SID_INT_L_FC_HI         ((uint8_t *)SID_INT_L+22)
#define SID_INT_L_RES_FILT      ((uint8_t *)SID_INT_L+23)
#define SID_INT_L_MODE_VOL      ((uint8_t *)SID_INT_L+24)
#define SID_INT_L_POT_X         ((uint8_t *)SID_INT_L+25)
#define SID_INT_L_POT_Y         ((uint8_t *)SID_INT_L+26)
#define SID_INT_L_OSC3_RND      ((uint8_t *)SID_INT_L+27)
#define SID_INT_L_ENV3          ((uint8_t *)SID_INT_L+28)
#define SID_INT_L_NOT_USED0     ((uint8_t *)SID_INT_L+29)
#define SID_INT_L_NOT_USED1     ((uint8_t *)SID_INT_L+30)
#define SID_INT_L_NOT_USED2     ((uint8_t *)SID_INT_L+31)

/*
 * Internal SID Right Channel
 */

#define SID_INT_R_V1_FREQ_LO  	((uint8_t *)SID_INT_R)
#define SID_INT_R_V1_FREQ_HI  	((uint8_t *)SID_INT_R+1)
#define SID_INT_R_V1_PW_LO		((uint8_t *)SID_INT_R+2)
#define SID_INT_R_V1_PW_HI		((uint8_t *)SID_INT_R+3)
#define SID_INT_R_V1_CTRL		((uint8_t *)SID_INT_R+4)
#define SID_INT_R_V1_ATCK_DECY  ((uint8_t *)SID_INT_R+5)
#define SID_INT_R_V1_SSTN_RLSE  ((uint8_t *)SID_INT_R+6)
#define SID_INT_R_V2_FREQ_LO    ((uint8_t *)SID_INT_R+7)
#define SID_INT_R_V2_FREQ_HI    ((uint8_t *)SID_INT_R+8)
#define SID_INT_R_V2_PW_LO      ((uint8_t *)SID_INT_R+9)
#define SID_INT_R_V2_PW_HI		((uint8_t *)SID_INT_R+10)
#define SID_INT_R_V2_CTRL       ((uint8_t *)SID_INT_R+11)
#define SID_INT_R_V2_ATCK_DECY  ((uint8_t *)SID_INT_R+12)
#define SID_INT_R_V2_SSTN_RLSE  ((uint8_t *)SID_INT_R+13)
#define SID_INT_R_V3_FREQ_LO    ((uint8_t *)SID_INT_R+14)
#define SID_INT_R_V3_FREQ_HI    ((uint8_t *)SID_INT_R+15)
#define SID_INT_R_V3_PW_LO      ((uint8_t *)SID_INT_R+16)
#define SID_INT_R_V3_PW_HI		((uint8_t *)SID_INT_R+17)
#define SID_INT_R_V3_CTRL       ((uint8_t *)SID_INT_R+18)
#define SID_INT_R_V3_ATCK_DECY  ((uint8_t *)SID_INT_R+19)
#define SID_INT_R_V3_SSTN_RLSE  ((uint8_t *)SID_INT_R+20)
#define SID_INT_R_FC_LO			((uint8_t *)SID_INT_R+21)
#define SID_INT_R_FC_HI         ((uint8_t *)SID_INT_R+22)
#define SID_INT_R_RES_FILT      ((uint8_t *)SID_INT_R+23)
#define SID_INT_R_MODE_VOL      ((uint8_t *)SID_INT_R+24)
#define SID_INT_R_POT_X         ((uint8_t *)SID_INT_R+25)
#define SID_INT_R_POT_Y         ((uint8_t *)SID_INT_R+26)
#define SID_INT_R_OSC3_RND      ((uint8_t *)SID_INT_R+27)
#define SID_INT_R_ENV3          ((uint8_t *)SID_INT_R+28)
#define SID_INT_R_NOT_USED0     ((uint8_t *)SID_INT_R+29)
#define SID_INT_R_NOT_USED1     ((uint8_t *)SID_INT_R+30)
#define SID_INT_R_NOT_USED2     ((uint8_t *)SID_INT_R+31)

/*
 * Internal SID Neutral Channel - When writting here, the value is written in R and L Channel at the same time
 */

#define SID_INT_N_V1_FREQ_LO  	((uint8_t *)SID_INT_N)
#define SID_INT_N_V1_FREQ_HI  	((uint8_t *)SID_INT_N+1)
#define SID_INT_N_V1_PW_LO		((uint8_t *)SID_INT_N+2)
#define SID_INT_N_V1_PW_HI		((uint8_t *)SID_INT_N+3)
#define SID_INT_N_V1_CTRL		((uint8_t *)SID_INT_N+4)
#define SID_INT_N_V1_ATCK_DECY  ((uint8_t *)SID_INT_N+5)
#define SID_INT_N_V1_SSTN_RLSE  ((uint8_t *)SID_INT_N+6)
#define SID_INT_N_V2_FREQ_LO    ((uint8_t *)SID_INT_N+7)
#define SID_INT_N_V2_FREQ_HI    ((uint8_t *)SID_INT_N+8)
#define SID_INT_N_V2_PW_LO      ((uint8_t *)SID_INT_N+9)
#define SID_INT_N_V2_PW_HI		((uint8_t *)SID_INT_N+10)
#define SID_INT_N_V2_CTRL       ((uint8_t *)SID_INT_N+11)
#define SID_INT_N_V2_ATCK_DECY  ((uint8_t *)SID_INT_N+12)
#define SID_INT_N_V2_SSTN_RLSE  ((uint8_t *)SID_INT_N+13)
#define SID_INT_N_V3_FREQ_LO    ((uint8_t *)SID_INT_N+14)
#define SID_INT_N_V3_FREQ_HI    ((uint8_t *)SID_INT_N+15)
#define SID_INT_N_V3_PW_LO      ((uint8_t *)SID_INT_N+16)
#define SID_INT_N_V3_PW_HI		((uint8_t *)SID_INT_N+17)
#define SID_INT_N_V3_CTRL       ((uint8_t *)SID_INT_N+18)
#define SID_INT_N_V3_ATCK_DECY  ((uint8_t *)SID_INT_N+19)
#define SID_INT_N_V3_SSTN_RLSE  ((uint8_t *)SID_INT_N+20)
#define SID_INT_N_FC_LO			((uint8_t *)SID_INT_N+21)
#define SID_INT_N_FC_HI         ((uint8_t *)SID_INT_N+22)
#define SID_INT_N_RES_FILT      ((uint8_t *)SID_INT_N+23)
#define SID_INT_N_MODE_VOL      ((uint8_t *)SID_INT_N+24)
#define SID_INT_N_POT_X         ((uint8_t *)SID_INT_N+25)
#define SID_INT_N_POT_Y         ((uint8_t *)SID_INT_N+26)
#define SID_INT_N_OSC3_RND      ((uint8_t *)SID_INT_N+27)
#define SID_INT_N_ENV3          ((uint8_t *)SID_INT_N+28)
#define SID_INT_N_NOT_USED0     ((uint8_t *)SID_INT_N+29)
#define SID_INT_N_NOT_USED1     ((uint8_t *)SID_INT_N+30)
#define SID_INT_N_NOT_USED2     ((uint8_t *)SID_INT_N+31)

#endif
