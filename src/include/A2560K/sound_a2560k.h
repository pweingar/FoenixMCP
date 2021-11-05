/*
 * Sound device register definitions for the A2560K
 */

#ifndef __SOUND_A2560K_H
#define __SOUND_A2560K_H

#define PSG_PORT        ((volatile unsigned char *)0x00C20100)  /* Control register for the SN76489 */
#define OPL3_PORT       ((volatile unsigned char *)0x00C20200)  /* Access port for the OPL3 */
#define OPM_EXT_BASE    ((volatile unsigned char *)0x00C20600)  /* External OPM base address */
#define OPN2_EXT_BASE   ((volatile unsigned char *)0x00C20400)  /* External OPN2 base address */
#define OPM_INT_BASE    ((volatile unsigned char *)0x00C20C00)  /* Internal OPM base address */
#define OPN2_INT_BASE   ((volatile unsigned char *)0x00C20A00)  /* Internal OPN2 base address */
#define CODEC           ((volatile unsigned short *)0x00C20E00) /* Control register for the CODEC */

/*
 * A2560K supports two external, physical SID chips.
 * It also implements the Gideon FPGA based SID in either stereo or mono (neutral) form
 *
 * $00C20800..$00C008FF - Extern Left SID
 * $00C20900..$00C009FF - Extern Right SID
 * $00C21000..$00C211FF - Internal SID Left
 * $00C21200..$00C213FF - Internal SID Right
 * $00C21400..$00C215FF - Internal SID Neutral
 */

/*
 * External SID Left Channel
 */

#define SID_EXT_L_V1_FREQ_LO  	((unsigned char *)0x00C20800)
#define SID_EXT_L_V1_FREQ_HI  	((unsigned char *)0x00C20801)
#define SID_EXT_L_V1_PW_LO		((unsigned char *)0x00C20802)
#define SID_EXT_L_V1_PW_HI		((unsigned char *)0x00C20803)
#define SID_EXT_L_V1_CTRL		((unsigned char *)0x00C20804)
#define SID_EXT_L_V1_ATCK_DECY  ((unsigned char *)0x00C20805)
#define SID_EXT_L_V1_SSTN_RLSE  ((unsigned char *)0x00C20806)
#define SID_EXT_L_V2_FREQ_LO    ((unsigned char *)0x00C20807)
#define SID_EXT_L_V2_FREQ_HI    ((unsigned char *)0x00C20808)
#define SID_EXT_L_V2_PW_LO      ((unsigned char *)0x00C20809)
#define SID_EXT_L_V2_PW_HI		((unsigned char *)0x00C2080A)
#define SID_EXT_L_V2_CTRL       ((unsigned char *)0x00C2080B)
#define SID_EXT_L_V2_ATCK_DECY  ((unsigned char *)0x00C2080C)
#define SID_EXT_L_V2_SSTN_RLSE  ((unsigned char *)0x00C2080D)
#define SID_EXT_L_V3_FREQ_LO    ((unsigned char *)0x00C2080E)
#define SID_EXT_L_V3_FREQ_HI    ((unsigned char *)0x00C2080F)
#define SID_EXT_L_V3_PW_LO      ((unsigned char *)0x00C20810)
#define SID_EXT_L_V3_PW_HI		((unsigned char *)0x00C20811)
#define SID_EXT_L_V3_CTRL       ((unsigned char *)0x00C20812)
#define SID_EXT_L_V3_ATCK_DECY  ((unsigned char *)0x00C20813)
#define SID_EXT_L_V3_SSTN_RLSE  ((unsigned char *)0x00C20814)
#define SID_EXT_L_FC_LO			((unsigned char *)0x00C20815)
#define SID_EXT_L_FC_HI         ((unsigned char *)0x00C20816)
#define SID_EXT_L_RES_FILT      ((unsigned char *)0x00C20817)
#define SID_EXT_L_MODE_VOL      ((unsigned char *)0x00C20818)
#define SID_EXT_L_POT_X         ((unsigned char *)0x00C20819)
#define SID_EXT_L_POT_Y         ((unsigned char *)0x00C2081A)
#define SID_EXT_L_OSC3_RND      ((unsigned char *)0x00C2081B)
#define SID_EXT_L_ENV3          ((unsigned char *)0x00C2081C)
#define SID_EXT_L_NOT_USED0     ((unsigned char *)0x00C2081D)
#define SID_EXT_L_NOT_USED1     ((unsigned char *)0x00C2081E)
#define SID_EXT_L_NOT_USED2     ((unsigned char *)0x00C2081F)

/*
 * External SID Right Channel
 */

#define SID_EXT_R_V1_FREQ_LO  	((unsigned char *)0x00C20900)
#define SID_EXT_R_V1_FREQ_HI  	((unsigned char *)0x00C20901)
#define SID_EXT_R_V1_PW_LO		((unsigned char *)0x00C20902)
#define SID_EXT_R_V1_PW_HI		((unsigned char *)0x00C20903)
#define SID_EXT_R_V1_CTRL		((unsigned char *)0x00C20904)
#define SID_EXT_R_V1_ATCK_DECY  ((unsigned char *)0x00C20905)
#define SID_EXT_R_V1_SSTN_RLSE  ((unsigned char *)0x00C20906)
#define SID_EXT_R_V2_FREQ_LO    ((unsigned char *)0x00C20907)
#define SID_EXT_R_V2_FREQ_HI    ((unsigned char *)0x00C20908)
#define SID_EXT_R_V2_PW_LO      ((unsigned char *)0x00C20909)
#define SID_EXT_R_V2_PW_HI		((unsigned char *)0x00C2090A)
#define SID_EXT_R_V2_CTRL       ((unsigned char *)0x00C2090B)
#define SID_EXT_R_V2_ATCK_DECY  ((unsigned char *)0x00C2090C)
#define SID_EXT_R_V2_SSTN_RLSE  ((unsigned char *)0x00C2090D)
#define SID_EXT_R_V3_FREQ_LO    ((unsigned char *)0x00C2090E)
#define SID_EXT_R_V3_FREQ_HI    ((unsigned char *)0x00C2090F)
#define SID_EXT_R_V3_PW_LO      ((unsigned char *)0x00C20910)
#define SID_EXT_R_V3_PW_HI		((unsigned char *)0x00C20911)
#define SID_EXT_R_V3_CTRL       ((unsigned char *)0x00C20912)
#define SID_EXT_R_V3_ATCK_DECY  ((unsigned char *)0x00C20913)
#define SID_EXT_R_V3_SSTN_RLSE  ((unsigned char *)0x00C20914)
#define SID_EXT_R_FC_LO			((unsigned char *)0x00C20915)
#define SID_EXT_R_FC_HI         ((unsigned char *)0x00C20916)
#define SID_EXT_R_RES_FILT      ((unsigned char *)0x00C20917)
#define SID_EXT_R_MODE_VOL      ((unsigned char *)0x00C20918)
#define SID_EXT_R_POT_X         ((unsigned char *)0x00C20919)
#define SID_EXT_R_POT_Y         ((unsigned char *)0x00C2091A)
#define SID_EXT_R_OSC3_RND      ((unsigned char *)0x00C2091B)
#define SID_EXT_R_ENV3          ((unsigned char *)0x00C2091C)
#define SID_EXT_R_NOT_USED0     ((unsigned char *)0x00C2091D)
#define SID_EXT_R_NOT_USED1     ((unsigned char *)0x00C2091E)
#define SID_EXT_R_NOT_USED2     ((unsigned char *)0x00C2091F)

/*
 * Internal SID Left Channel
 */

#define SID_INT_L_V1_FREQ_LO  	((unsigned char *)0x00C21000)
#define SID_INT_L_V1_FREQ_HI  	((unsigned char *)0x00C21001)
#define SID_INT_L_V1_PW_LO		((unsigned char *)0x00C21002)
#define SID_INT_L_V1_PW_HI		((unsigned char *)0x00C21003)
#define SID_INT_L_V1_CTRL		((unsigned char *)0x00C21004)
#define SID_INT_L_V1_ATCK_DECY  ((unsigned char *)0x00C21005)
#define SID_INT_L_V1_SSTN_RLSE  ((unsigned char *)0x00C21006)
#define SID_INT_L_V2_FREQ_LO    ((unsigned char *)0x00C21007)
#define SID_INT_L_V2_FREQ_HI    ((unsigned char *)0x00C21008)
#define SID_INT_L_V2_PW_LO      ((unsigned char *)0x00C21009)
#define SID_INT_L_V2_PW_HI		((unsigned char *)0x00C2100A)
#define SID_INT_L_V2_CTRL       ((unsigned char *)0x00C2100B)
#define SID_INT_L_V2_ATCK_DECY  ((unsigned char *)0x00C2100C)
#define SID_INT_L_V2_SSTN_RLSE  ((unsigned char *)0x00C2100D)
#define SID_INT_L_V3_FREQ_LO    ((unsigned char *)0x00C2100E)
#define SID_INT_L_V3_FREQ_HI    ((unsigned char *)0x00C2100F)
#define SID_INT_L_V3_PW_LO      ((unsigned char *)0x00C21010)
#define SID_INT_L_V3_PW_HI		((unsigned char *)0x00C21011)
#define SID_INT_L_V3_CTRL       ((unsigned char *)0x00C21012)
#define SID_INT_L_V3_ATCK_DECY  ((unsigned char *)0x00C21013)
#define SID_INT_L_V3_SSTN_RLSE  ((unsigned char *)0x00C21014)
#define SID_INT_L_FC_LO			((unsigned char *)0x00C21015)
#define SID_INT_L_FC_HI         ((unsigned char *)0x00C21016)
#define SID_INT_L_RES_FILT      ((unsigned char *)0x00C21017)
#define SID_INT_L_MODE_VOL      ((unsigned char *)0x00C21018)
#define SID_INT_L_POT_X         ((unsigned char *)0x00C21019)
#define SID_INT_L_POT_Y         ((unsigned char *)0x00C2101A)
#define SID_INT_L_OSC3_RND      ((unsigned char *)0x00C2101B)
#define SID_INT_L_ENV3          ((unsigned char *)0x00C2101C)
#define SID_INT_L_NOT_USED0     ((unsigned char *)0x00C2101D)
#define SID_INT_L_NOT_USED1     ((unsigned char *)0x00C2101E)
#define SID_INT_L_NOT_USED2     ((unsigned char *)0x00C2101F)

/*
 * Internal SID Right Channel
 */

#define SID_INT_R_V1_FREQ_LO  	((unsigned char *)0x00C21200)
#define SID_INT_R_V1_FREQ_HI  	((unsigned char *)0x00C21201)
#define SID_INT_R_V1_PW_LO		((unsigned char *)0x00C21202)
#define SID_INT_R_V1_PW_HI		((unsigned char *)0x00C21203)
#define SID_INT_R_V1_CTRL		((unsigned char *)0x00C21204)
#define SID_INT_R_V1_ATCK_DECY  ((unsigned char *)0x00C21205)
#define SID_INT_R_V1_SSTN_RLSE  ((unsigned char *)0x00C21206)
#define SID_INT_R_V2_FREQ_LO    ((unsigned char *)0x00C21207)
#define SID_INT_R_V2_FREQ_HI    ((unsigned char *)0x00C21208)
#define SID_INT_R_V2_PW_LO      ((unsigned char *)0x00C21209)
#define SID_INT_R_V2_PW_HI		((unsigned char *)0x00C2120A)
#define SID_INT_R_V2_CTRL       ((unsigned char *)0x00C2120B)
#define SID_INT_R_V2_ATCK_DECY  ((unsigned char *)0x00C2120C)
#define SID_INT_R_V2_SSTN_RLSE  ((unsigned char *)0x00C2120D)
#define SID_INT_R_V3_FREQ_LO    ((unsigned char *)0x00C2120E)
#define SID_INT_R_V3_FREQ_HI    ((unsigned char *)0x00C2120F)
#define SID_INT_R_V3_PW_LO      ((unsigned char *)0x00C21210)
#define SID_INT_R_V3_PW_HI		((unsigned char *)0x00C21211)
#define SID_INT_R_V3_CTRL       ((unsigned char *)0x00C21212)
#define SID_INT_R_V3_ATCK_DECY  ((unsigned char *)0x00C21213)
#define SID_INT_R_V3_SSTN_RLSE  ((unsigned char *)0x00C21214)
#define SID_INT_R_FC_LO			((unsigned char *)0x00C21215)
#define SID_INT_R_FC_HI         ((unsigned char *)0x00C21216)
#define SID_INT_R_RES_FILT      ((unsigned char *)0x00C21217)
#define SID_INT_R_MODE_VOL      ((unsigned char *)0x00C21218)
#define SID_INT_R_POT_X         ((unsigned char *)0x00C21219)
#define SID_INT_R_POT_Y         ((unsigned char *)0x00C2121A)
#define SID_INT_R_OSC3_RND      ((unsigned char *)0x00C2121B)
#define SID_INT_R_ENV3          ((unsigned char *)0x00C2121C)
#define SID_INT_R_NOT_USED0     ((unsigned char *)0x00C2121D)
#define SID_INT_R_NOT_USED1     ((unsigned char *)0x00C2121E)
#define SID_INT_R_NOT_USED2     ((unsigned char *)0x00C2121F)

/*
 * Internal SID Neutral Channel - When writting here, the value is written in R and L Channel at the same time
 */

#define SID_INT_N_V1_FREQ_LO  	((unsigned char *)0x00C41200)
#define SID_INT_N_V1_FREQ_HI  	((unsigned char *)0x00C41201)
#define SID_INT_N_V1_PW_LO		((unsigned char *)0x00C41202)
#define SID_INT_N_V1_PW_HI		((unsigned char *)0x00C41203)
#define SID_INT_N_V1_CTRL		((unsigned char *)0x00C41204)
#define SID_INT_N_V1_ATCK_DECY  ((unsigned char *)0x00C41205)
#define SID_INT_N_V1_SSTN_RLSE  ((unsigned char *)0x00C41206)
#define SID_INT_N_V2_FREQ_LO    ((unsigned char *)0x00C41207)
#define SID_INT_N_V2_FREQ_HI    ((unsigned char *)0x00C41208)
#define SID_INT_N_V2_PW_LO      ((unsigned char *)0x00C41209)
#define SID_INT_N_V2_PW_HI		((unsigned char *)0x00C4120A)
#define SID_INT_N_V2_CTRL       ((unsigned char *)0x00C4120B)
#define SID_INT_N_V2_ATCK_DECY  ((unsigned char *)0x00C4120C)
#define SID_INT_N_V2_SSTN_RLSE  ((unsigned char *)0x00C4120D)
#define SID_INT_N_V3_FREQ_LO    ((unsigned char *)0x00C4120E)
#define SID_INT_N_V3_FREQ_HI    ((unsigned char *)0x00C4120F)
#define SID_INT_N_V3_PW_LO      ((unsigned char *)0x00C41210)
#define SID_INT_N_V3_PW_HI		((unsigned char *)0x00C41211)
#define SID_INT_N_V3_CTRL       ((unsigned char *)0x00C41212)
#define SID_INT_N_V3_ATCK_DECY  ((unsigned char *)0x00C41213)
#define SID_INT_N_V3_SSTN_RLSE  ((unsigned char *)0x00C41214)
#define SID_INT_N_FC_LO			((unsigned char *)0x00C41215)
#define SID_INT_N_FC_HI         ((unsigned char *)0x00C41216)
#define SID_INT_N_RES_FILT      ((unsigned char *)0x00C41217)
#define SID_INT_N_MODE_VOL      ((unsigned char *)0x00C41218)
#define SID_INT_N_POT_X         ((unsigned char *)0x00C41219)
#define SID_INT_N_POT_Y         ((unsigned char *)0x00C4121A)
#define SID_INT_N_OSC3_RND      ((unsigned char *)0x00C4121B)
#define SID_INT_N_ENV3          ((unsigned char *)0x00C4121C)
#define SID_INT_N_NOT_USED0     ((unsigned char *)0x00C4121D)
#define SID_INT_N_NOT_USED1     ((unsigned char *)0x00C4121E)
#define SID_INT_N_NOT_USED2     ((unsigned char *)0x00C4121F)

#endif
