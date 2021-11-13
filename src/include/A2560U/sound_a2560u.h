/*
 * Sound device register definitions for the A2560U and A2560U+
 */

#ifndef __SOUND_A2560U_H
#define __SOUND_A2560U_H

#define PSG_PORT        ((volatile unsigned char *)0x00B20130)  /* Control register for the SN76489 */
#define OPL3_PORT       ((volatile unsigned char *)0x00B20200)  /* Access port for the OPL3 */
#define OPM_INT_BASE    ((volatile unsigned char *)0x00B20C00)  /* Internal OPM base address */
#define OPN2_INT_BASE   ((volatile unsigned char *)0x00B20A00)  /* Internal OPN2 base address */
#define CODEC           ((volatile unsigned short *)0x00B20E00) /* Control register for the CODEC */

/*
 * A2560U supports two external, physical SID chips.
 * It also implements the Gideon FPGA based SID in either stereo or mono (neutral) form
 *
 * $00C21000..$00C211FF - Internal SID Left
 * $00C21200..$00C213FF - Internal SID Right
 * $00C21400..$00C215FF - Internal SID Neutral
 */

/*
 * Internal SID Left Channel
 */

#define SID_INT_L_V1_FREQ_LO  	((unsigned char *)0x00B21000)
#define SID_INT_L_V1_FREQ_HI  	((unsigned char *)0x00B21001)
#define SID_INT_L_V1_PW_LO		((unsigned char *)0x00B21002)
#define SID_INT_L_V1_PW_HI		((unsigned char *)0x00B21003)
#define SID_INT_L_V1_CTRL		((unsigned char *)0x00B21004)
#define SID_INT_L_V1_ATCK_DECY  ((unsigned char *)0x00B21005)
#define SID_INT_L_V1_SSTN_RLSE  ((unsigned char *)0x00B21006)
#define SID_INT_L_V2_FREQ_LO    ((unsigned char *)0x00B21007)
#define SID_INT_L_V2_FREQ_HI    ((unsigned char *)0x00B21008)
#define SID_INT_L_V2_PW_LO      ((unsigned char *)0x00B21009)
#define SID_INT_L_V2_PW_HI		((unsigned char *)0x00B2100A)
#define SID_INT_L_V2_CTRL       ((unsigned char *)0x00B2100B)
#define SID_INT_L_V2_ATCK_DECY  ((unsigned char *)0x00B2100C)
#define SID_INT_L_V2_SSTN_RLSE  ((unsigned char *)0x00B2100D)
#define SID_INT_L_V3_FREQ_LO    ((unsigned char *)0x00B2100E)
#define SID_INT_L_V3_FREQ_HI    ((unsigned char *)0x00B2100F)
#define SID_INT_L_V3_PW_LO      ((unsigned char *)0x00B21010)
#define SID_INT_L_V3_PW_HI		((unsigned char *)0x00B21011)
#define SID_INT_L_V3_CTRL       ((unsigned char *)0x00B21012)
#define SID_INT_L_V3_ATCK_DECY  ((unsigned char *)0x00B21013)
#define SID_INT_L_V3_SSTN_RLSE  ((unsigned char *)0x00B21014)
#define SID_INT_L_FC_LO			((unsigned char *)0x00B21015)
#define SID_INT_L_FC_HI         ((unsigned char *)0x00B21016)
#define SID_INT_L_RES_FILT      ((unsigned char *)0x00B21017)
#define SID_INT_L_MODE_VOL      ((unsigned char *)0x00B21018)
#define SID_INT_L_POT_X         ((unsigned char *)0x00B21019)
#define SID_INT_L_POT_Y         ((unsigned char *)0x00B2101A)
#define SID_INT_L_OSC3_RND      ((unsigned char *)0x00B2101B)
#define SID_INT_L_ENV3          ((unsigned char *)0x00B2101C)
#define SID_INT_L_NOT_USED0     ((unsigned char *)0x00B2101D)
#define SID_INT_L_NOT_USED1     ((unsigned char *)0x00B2101E)
#define SID_INT_L_NOT_USED2     ((unsigned char *)0x00B2101F)

/*
 * Internal SID Right Channel
 */

#define SID_INT_R_V1_FREQ_LO  	((unsigned char *)0x00B21200)
#define SID_INT_R_V1_FREQ_HI  	((unsigned char *)0x00B21201)
#define SID_INT_R_V1_PW_LO		((unsigned char *)0x00B21202)
#define SID_INT_R_V1_PW_HI		((unsigned char *)0x00B21203)
#define SID_INT_R_V1_CTRL		((unsigned char *)0x00B21204)
#define SID_INT_R_V1_ATCK_DECY  ((unsigned char *)0x00B21205)
#define SID_INT_R_V1_SSTN_RLSE  ((unsigned char *)0x00B21206)
#define SID_INT_R_V2_FREQ_LO    ((unsigned char *)0x00B21207)
#define SID_INT_R_V2_FREQ_HI    ((unsigned char *)0x00B21208)
#define SID_INT_R_V2_PW_LO      ((unsigned char *)0x00B21209)
#define SID_INT_R_V2_PW_HI		((unsigned char *)0x00B2120A)
#define SID_INT_R_V2_CTRL       ((unsigned char *)0x00B2120B)
#define SID_INT_R_V2_ATCK_DECY  ((unsigned char *)0x00B2120C)
#define SID_INT_R_V2_SSTN_RLSE  ((unsigned char *)0x00B2120D)
#define SID_INT_R_V3_FREQ_LO    ((unsigned char *)0x00B2120E)
#define SID_INT_R_V3_FREQ_HI    ((unsigned char *)0x00B2120F)
#define SID_INT_R_V3_PW_LO      ((unsigned char *)0x00B21210)
#define SID_INT_R_V3_PW_HI		((unsigned char *)0x00B21211)
#define SID_INT_R_V3_CTRL       ((unsigned char *)0x00B21212)
#define SID_INT_R_V3_ATCK_DECY  ((unsigned char *)0x00B21213)
#define SID_INT_R_V3_SSTN_RLSE  ((unsigned char *)0x00B21214)
#define SID_INT_R_FC_LO			((unsigned char *)0x00B21215)
#define SID_INT_R_FC_HI         ((unsigned char *)0x00B21216)
#define SID_INT_R_RES_FILT      ((unsigned char *)0x00B21217)
#define SID_INT_R_MODE_VOL      ((unsigned char *)0x00B21218)
#define SID_INT_R_POT_X         ((unsigned char *)0x00B21219)
#define SID_INT_R_POT_Y         ((unsigned char *)0x00B2121A)
#define SID_INT_R_OSC3_RND      ((unsigned char *)0x00B2121B)
#define SID_INT_R_ENV3          ((unsigned char *)0x00B2121C)
#define SID_INT_R_NOT_USED0     ((unsigned char *)0x00B2121D)
#define SID_INT_R_NOT_USED1     ((unsigned char *)0x00B2121E)
#define SID_INT_R_NOT_USED2     ((unsigned char *)0x00B2121F)

/*
 * Internal SID Neutral Channel - When writting here, the value is written in R and L Channel at the same time
 */

#define SID_INT_N_V1_FREQ_LO  	((unsigned char *)0x00B21400)
#define SID_INT_N_V1_FREQ_HI  	((unsigned char *)0x00B21401)
#define SID_INT_N_V1_PW_LO		((unsigned char *)0x00B21402)
#define SID_INT_N_V1_PW_HI		((unsigned char *)0x00B21403)
#define SID_INT_N_V1_CTRL		((unsigned char *)0x00B21404)
#define SID_INT_N_V1_ATCK_DECY  ((unsigned char *)0x00B21405)
#define SID_INT_N_V1_SSTN_RLSE  ((unsigned char *)0x00B21406)
#define SID_INT_N_V2_FREQ_LO    ((unsigned char *)0x00B21407)
#define SID_INT_N_V2_FREQ_HI    ((unsigned char *)0x00B21408)
#define SID_INT_N_V2_PW_LO      ((unsigned char *)0x00B21409)
#define SID_INT_N_V2_PW_HI		((unsigned char *)0x00B2140A)
#define SID_INT_N_V2_CTRL       ((unsigned char *)0x00B2140B)
#define SID_INT_N_V2_ATCK_DECY  ((unsigned char *)0x00B2140C)
#define SID_INT_N_V2_SSTN_RLSE  ((unsigned char *)0x00B2140D)
#define SID_INT_N_V3_FREQ_LO    ((unsigned char *)0x00B2140E)
#define SID_INT_N_V3_FREQ_HI    ((unsigned char *)0x00B2140F)
#define SID_INT_N_V3_PW_LO      ((unsigned char *)0x00B21410)
#define SID_INT_N_V3_PW_HI		((unsigned char *)0x00B21411)
#define SID_INT_N_V3_CTRL       ((unsigned char *)0x00B21412)
#define SID_INT_N_V3_ATCK_DECY  ((unsigned char *)0x00B21413)
#define SID_INT_N_V3_SSTN_RLSE  ((unsigned char *)0x00B21414)
#define SID_INT_N_FC_LO			((unsigned char *)0x00B21415)
#define SID_INT_N_FC_HI         ((unsigned char *)0x00B21416)
#define SID_INT_N_RES_FILT      ((unsigned char *)0x00B21417)
#define SID_INT_N_MODE_VOL      ((unsigned char *)0x00B21418)
#define SID_INT_N_POT_X         ((unsigned char *)0x00B21419)
#define SID_INT_N_POT_Y         ((unsigned char *)0x00B2141A)
#define SID_INT_N_OSC3_RND      ((unsigned char *)0x00B2141B)
#define SID_INT_N_ENV3          ((unsigned char *)0x00B2141C)
#define SID_INT_N_NOT_USED0     ((unsigned char *)0x00B2141D)
#define SID_INT_N_NOT_USED1     ((unsigned char *)0x00B2141E)
#define SID_INT_N_NOT_USED2     ((unsigned char *)0x00B2141F)

#endif
