/*
 * Commands to work with the sound devices
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "features.h"
#include "timers.h"
#include "sound_cmds.h"
#include "sound_reg.h"
#include "snd/opn2.h"
#include "snd/psg.h"
#include "snd/sid.h"
#include "libfoenix/include/midi.h"
#include "simpleio.h"
#include "syscalls.h"

#include "include/A2560K/YM2151.h"
#include "include/A2560K/YM2612_Ext.h"
#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_A2560X || MODEL == MODEL_FOENIX_GENX
#include "include/A2560K/sound_a2560k.h"
#endif

/*
 * Play a sound on the PSG
 * TEST PGS [EXT|INTL|INTR|INTS]
 */
short psg_test(short channel, int argc, const char * argv[]) {
    int i;
    long target_time;
    short chip = 0;         /* Default to external PSG */

    if (argc > 1) {
        if ((strcmp(argv[1], "intl") == 0) || (strcmp(argv[1], "INTL") == 0)) {
            chip = 1;
        } else if ((strcmp(argv[1], "intr") == 0) || (strcmp(argv[1], "INTR") == 0)) {
            chip = 2;
        } else if ((strcmp(argv[1], "ints") == 0) || (strcmp(argv[1], "INTS") == 0)) {
            chip = 3;
        }
    }

    psg_tone(chip, 0, 34923);
    psg_attenuation(chip, 0, 0);

    target_time = sys_time_jiffies() + 6;
    while (target_time > sys_time_jiffies()) ;

    psg_tone(chip, 1, 44000);
    psg_attenuation(chip, 1, 0);

    target_time = sys_time_jiffies() + 6;
    while (target_time > sys_time_jiffies()) ;

    psg_tone(chip, 2, 52325);
    psg_attenuation(chip, 2, 0);

    target_time = sys_time_jiffies() + 60;
    while (target_time > sys_time_jiffies()) ;

    psg_attenuation(chip, 0, 15);
    psg_tone(chip, 0, 0);
    psg_attenuation(chip, 1, 15);
    psg_tone(chip, 1, 0);
    psg_attenuation(chip, 2, 15);
    psg_tone(chip, 2, 0);

    return 0;
}

/*
 * Play a sound on the SID
 */
short sid_test(short channel, int argc, const char * argv[]) {
#if HAS_EXTERNAL_SIDS
    if (argc >= 2) {
        /* Allow the user to select the external OPM */
        if ((strcmp(argv[1], "ext") == 0) || (strcmp(argv[1], "EXT") == 0)) {
            sid_test_external();
        } else {
            sid_test_internal();
        }
    }
#else
    sid_test_internal();
#endif

    return 0;
}


#if HAS_OPM
 void Test_EXT_FM2151( void ) {

     unsigned char i;
     unsigned int j;

     *EXT_OPM_0F_NE_NFREQ = 0x90;
     *EXT_OPM_18_LFRQ = 0x55;

     i = 0xD7;
     * EXT_OPM_20_A_RL_FR_CONNECT = i;
     * EXT_OPM_21_B_RL_FR_CONNECT = i;
     * EXT_OPM_22_C_RL_FR_CONNECT = i;
     * EXT_OPM_23_D_RL_FR_CONNECT = i;
     * EXT_OPM_24_E_RL_FR_CONNECT = i;
     * EXT_OPM_25_F_RL_FR_CONNECT = i;
     * EXT_OPM_26_G_RL_FR_CONNECT = i;
     * EXT_OPM_27_H_RL_FR_CONNECT = i;
     i = 0xD7;
     * EXT_OPM_24_E_RL_FR_CONNECT = i;
     * EXT_OPM_25_F_RL_FR_CONNECT = i;
     * EXT_OPM_26_G_RL_FR_CONNECT = i;
     * EXT_OPM_27_H_RL_FR_CONNECT = i;
 //; ADSR
 //;        Note on                                                     Note off
 //;    -------------------------------------------------------
 //;   ¦                                                          ¦
 //;   ¦                                                          ¦
 //;---                                                          -----------
 //;
 //;                   * -V MAX
 //;             *         *
 //;         *                 *
 //;       *                       * - D1L
 //;     *                                  *
 //;    *                                           *
 //;   *                                                     *
 //;  *                                                         *
 //;---         AT      ¦   DT1    ¦           DT2            ¦ TR  ----
     i = 0x00;
     * EXT_OPM_60_A_M1_TL = i;
     * EXT_OPM_61_B_M1_TL = i;
     * EXT_OPM_68_A_M2_TL = i;
     * EXT_OPM_69_B_M2_TL = i;
     * EXT_OPM_70_A_C1_TL = i;
     * EXT_OPM_71_B_C1_TL = i;
     * EXT_OPM_78_A_C2_TL = i;
     * EXT_OPM_79_B_C2_TL = i;

     * EXT_OPM_40_A_M1_DT1_MUL = 0x55;
     * EXT_OPM_41_B_M1_DT1_MUL = 0x18;
     * EXT_OPM_80_A_M1_KS_AR = 0xC4;
     * EXT_OPM_81_B_M1_KS_AR = 0xC4;
     * EXT_OPM_A0_A_M1_AMS_EN_D1R = 0x84;
     * EXT_OPM_A1_B_M1_AMS_EN_D1R = 0x85;
     i = 0x42;
     * EXT_OPM_C0_A_M1_DT2_D2R = i;
     * EXT_OPM_C8_A_M2_DT2_D2R = i;
     * EXT_OPM_D0_A_C1_DT2_D2R = i;
     * EXT_OPM_D8_A_C2_DT2_D2R = i;
     i = 0x43;
     * EXT_OPM_C1_B_M1_DT2_D2R = i;
     * EXT_OPM_C9_B_M2_DT2_D2R = i;
     * EXT_OPM_D1_B_C1_DT2_D2R = i;
     * EXT_OPM_D9_B_C2_DT2_D2R = i;
     i = 0x24;
     * EXT_OPM_E0_A_M1_D1L_RR = i;
     * EXT_OPM_E1_B_M1_D1L_RR = i;
     * EXT_OPM_E8_A_M2_D1L_RR = 0x14;
     * EXT_OPM_E9_B_M2_D1L_RR = 0x14;
     * EXT_OPM_F0_A_C1_D1L_RR = i;
     * EXT_OPM_F1_B_C1_D1L_RR = i;
     * EXT_OPM_F8_A_C2_D1L_RR = 0x44;
     * EXT_OPM_F9_B_C2_D1L_RR = 0x44;
     * EXT_OPM_28_A_KC    = 0x96;
     * EXT_OPM_29_B_KC    = 0x1A;
     * EXT_OPM_2A_C_KC    = 0x26;
     * EXT_OPM_2B_D_KC    = 0x2A;
     * EXT_OPM_2C_E_KC    = 0x36;
     * EXT_OPM_2D_F_KC    = 0x2A;
     * EXT_OPM_2E_G_KC    = 0x46;
     * EXT_OPM_2F_H_KC    = 0x4A;

     * EXT_OPM_30_A_KF    = 0x45;
     * EXT_OPM_31_B_KF    = 0x80;
     * EXT_OPM_32_C_KF    = 0x45;
     * EXT_OPM_33_D_KF     = 0x80;

     * EXT_OPM_08_KEY_ON_OFF = 0x78;
     * EXT_OPM_08_KEY_ON_OFF = 0x79;
     * EXT_OPM_08_KEY_ON_OFF = 0x7A;
     * EXT_OPM_08_KEY_ON_OFF = 0x7C;
     * EXT_OPM_08_KEY_ON_OFF = 0x7D;
     * EXT_OPM_08_KEY_ON_OFF = 0x7E;
     * EXT_OPM_08_KEY_ON_OFF = 0x7F;

     for (j=0 ; j<65536; j++);

     * EXT_OPM_08_KEY_ON_OFF = 0x00;
     * EXT_OPM_08_KEY_ON_OFF = 0x01;
     * EXT_OPM_08_KEY_ON_OFF = 0x02;
     * EXT_OPM_08_KEY_ON_OFF = 0x03;
     * EXT_OPM_08_KEY_ON_OFF = 0x04;
     * EXT_OPM_08_KEY_ON_OFF = 0x05;
     * EXT_OPM_08_KEY_ON_OFF = 0x06;
     * EXT_OPM_08_KEY_ON_OFF = 0x07;

     for (j=0 ; j<262144; j++);

     * EXT_OPM_08_KEY_ON_OFF = 0x00;
 }
#endif

#if HAS_OPN
 void Test_EXT_FM2612( void ) {
 			unsigned char i;
 			unsigned int j;

 			* EXT_OPN2_22_LFO = 0x00;
 			* EXT_OPN2_27_CHANEL_3_MODE = 0x00;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x00;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x01;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x02;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x04;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x05;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x06;
 			* EXT_OPN2_2B_ADC_EN 		= 0x00;
 			i = 0x71;
 			* EXT_OPN2_30_ADSR__DT1_MUL__CH1_OP1 = i;
 			* EXT_OPN2_31_ADSR__DT1_MUL__CH2_OP1 = i;
 			* EXT_OPN2_32_ADSR__DT1_MUL__CH3_OP1 = i;
 			* EXT_OPN2_30_ADSR__DT1_MUL__CH1_OP5 = i;
 			* EXT_OPN2_31_ADSR__DT1_MUL__CH2_OP5 = i;
 			* EXT_OPN2_32_ADSR__DT1_MUL__CH3_OP5 = i;
 			i = 0x0D;
 			* EXT_OPN2_35_ADSR__DT1_MUL__CH2_OP2 = i;
 			* EXT_OPN2_36_ADSR__DT1_MUL__CH3_OP2 = i;
 			* EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP3 = i;
 			* EXT_OPN2_34_ADSR__DT1_MUL__CH1_OP6 = i;
 			* EXT_OPN2_35_ADSR__DT1_MUL__CH2_OP6 = i;
 			* EXT_OPN2_36_ADSR__DT1_MUL__CH3_OP6 = i;
 			i = 0x33;
 			* EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP3 = i;
 			* EXT_OPN2_39_ADSR__DT1_MUL__CH2_OP3 = i;
 			* EXT_OPN2_3A_ADSR__DT1_MUL__CH3_OP3 = i;
 			* EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP7 = i;
 			* EXT_OPN2_39_ADSR__DT1_MUL__CH2_OP7 = i;
 			* EXT_OPN2_3A_ADSR__DT1_MUL__CH3_OP7 = i;
 			i = 0x01;
 			* EXT_OPN2_3C_ADSR__DT1_MUL__CH1_OP4 = i;
 			* EXT_OPN2_3D_ADSR__DT1_MUL__CH2_OP4 = i;
 			* EXT_OPN2_3E_ADSR__DT1_MUL__CH3_OP4 = i;
 			* EXT_OPN2_3C_ADSR__DT1_MUL__CH1_OP8 = i;
 			* EXT_OPN2_3D_ADSR__DT1_MUL__CH2_OP8 = i;
 			* EXT_OPN2_3E_ADSR__DT1_MUL__CH3_OP8 = i;
 			i = 0x23;
 			* EXT_OPN2_40_ADSR__LT__CH1_OP1 = i;
 			* EXT_OPN2_41_ADSR__LT__CH2_OP1 = i;
 			* EXT_OPN2_42_ADSR__LT__CH3_OP1 = i;
 			* EXT_OPN2_40_ADSR__LT__CH1_OP5 = i;
 			* EXT_OPN2_41_ADSR__LT__CH2_OP5 = i;
 			* EXT_OPN2_42_ADSR__LT__CH3_OP5 = i;
 			i = 0x2D;
 			* EXT_OPN2_44_ADSR__LT__CH1_OP2 = i;
 			* EXT_OPN2_45_ADSR__LT__CH2_OP2 = i;
 			* EXT_OPN2_46_ADSR__LT__CH3_OP2 = i;
 			* EXT_OPN2_44_ADSR__LT__CH1_OP6 = i;
 			* EXT_OPN2_45_ADSR__LT__CH2_OP6 = i;
 			* EXT_OPN2_46_ADSR__LT__CH3_OP6 = i;
 			i = 0x26;
 			* EXT_OPN2_48_ADSR__LT__CH1_OP3 = i;
 			* EXT_OPN2_49_ADSR__LT__CH2_OP3 = i;
 			* EXT_OPN2_4A_ADSR__LT__CH3_OP3 = i;
 			* EXT_OPN2_48_ADSR__LT__CH1_OP7 = i;
 			* EXT_OPN2_49_ADSR__LT__CH2_OP7 = i;
 			* EXT_OPN2_4A_ADSR__LT__CH3_OP7 = i;
 			i = 0x00;
 			* EXT_OPN2_4C_ADSR__LT__CH1_OP4 = i;
 			* EXT_OPN2_4D_ADSR__LT__CH2_OP4 = i;
 			* EXT_OPN2_4E_ADSR__LT__CH3_OP4 = i;
 			* EXT_OPN2_4C_ADSR__LT__CH1_OP8 = i;
 			* EXT_OPN2_4D_ADSR__LT__CH2_OP8 = i;
 			* EXT_OPN2_4E_ADSR__LT__CH3_OP8 = i;
 			i = 0x5F;
 			* EXT_OPN2_50_ADSR__SR_AR__CH1_OP1 = i;
 			* EXT_OPN2_51_ADSR__SR_AR__CH2_OP1 = i;
 			* EXT_OPN2_52_ADSR__SR_AR__CH3_OP1 = i;
 			* EXT_OPN2_50_ADSR__SR_AR__CH1_OP5 = i;
 			* EXT_OPN2_51_ADSR__SR_AR__CH2_OP5 = i;
 			* EXT_OPN2_52_ADSR__SR_AR__CH3_OP5 = i;
 			i = 0x99;
 			* EXT_OPN2_54_ADSR__SR_AR__CH1_OP2 = i;
 			* EXT_OPN2_55_ADSR__SR_AR__CH2_OP2 = i;
 			* EXT_OPN2_56_ADSR__SR_AR__CH3_OP2 = i;
 			* EXT_OPN2_54_ADSR__SR_AR__CH1_OP6 = i;
 			* EXT_OPN2_55_ADSR__SR_AR__CH2_OP6 = i;
 			* EXT_OPN2_56_ADSR__SR_AR__CH3_OP6 = i;
 			i = 0x5F;
 			* EXT_OPN2_58_ADSR__SR_AR__CH1_OP3 = i;
 			* EXT_OPN2_59_ADSR__SR_AR__CH2_OP3 = i;
 			* EXT_OPN2_5A_ADSR__SR_AR__CH3_OP3 = i;
 			* EXT_OPN2_58_ADSR__SR_AR__CH1_OP7 = i;
 			* EXT_OPN2_59_ADSR__SR_AR__CH2_OP7 = i;
 			* EXT_OPN2_5A_ADSR__SR_AR__CH3_OP7 = i;
 			i = 0x94;
 			* EXT_OPN2_5C_ADSR__SR_AR__CH1_OP4 = i;
 			* EXT_OPN2_5D_ADSR__SR_AR__CH2_OP4 = i;
 			* EXT_OPN2_5E_ADSR__SR_AR__CH3_OP4 = i;
 			* EXT_OPN2_5C_ADSR__SR_AR__CH1_OP8 = i;
 			* EXT_OPN2_5D_ADSR__SR_AR__CH2_OP8 = i;
 			* EXT_OPN2_5E_ADSR__SR_AR__CH3_OP8 = i;
 			i = 0x07;
 			* EXT_OPN2_60_ADSR__AM_D1R__CH1_OP1 = i;
 			* EXT_OPN2_61_ADSR__AM_D1R__CH2_OP1 = i;
 			* EXT_OPN2_62_ADSR__AM_D1R__CH3_OP1 = i;
 			* EXT_OPN2_60_ADSR__AM_D1R__CH1_OP5 = i;
 			* EXT_OPN2_61_ADSR__AM_D1R__CH2_OP5 = i;
 			* EXT_OPN2_62_ADSR__AM_D1R__CH3_OP5 = i;
 			i = 0x07;
 			* EXT_OPN2_64_ADSR__AM_D1R__CH1_OP2 = i;
 			* EXT_OPN2_65_ADSR__AM_D1R__CH2_OP2 = i;
 			* EXT_OPN2_66_ADSR__AM_D1R__CH3_OP2 = i;
 			* EXT_OPN2_64_ADSR__AM_D1R__CH1_OP6 = i;
 			* EXT_OPN2_65_ADSR__AM_D1R__CH2_OP6 = i;
 			* EXT_OPN2_66_ADSR__AM_D1R__CH3_OP6 = i;
 			i = 0x05;
 			* EXT_OPN2_68_ADSR__AM_D1R__CH1_OP3 = i;
             * EXT_OPN2_69_ADSR__AM_D1R__CH2_OP3 = i;
             * EXT_OPN2_6A_ADSR__AM_D1R__CH3_OP3 = i;
 			* EXT_OPN2_68_ADSR__AM_D1R__CH1_OP7 = i;
             * EXT_OPN2_69_ADSR__AM_D1R__CH2_OP7 = i;
             * EXT_OPN2_6A_ADSR__AM_D1R__CH3_OP7 = i;
 			i = 0x07;
 			* EXT_OPN2_6C_ADSR__AM_D1R__CH1_OP4 = i;
             * EXT_OPN2_6D_ADSR__AM_D1R__CH2_OP4 = i;
             * EXT_OPN2_6E_ADSR__AM_D1R__CH3_OP4 = i;
 			* EXT_OPN2_6C_ADSR__AM_D1R__CH1_OP8 = i;
 			* EXT_OPN2_6D_ADSR__AM_D1R__CH2_OP8 = i;
 			* EXT_OPN2_6E_ADSR__AM_D1R__CH3_OP8 = i;
 			i = 0x02;
 			* EXT_OPN2_70_ADSR__D2R__CH1_OP1 = i;
             * EXT_OPN2_71_ADSR__D2R__CH2_OP1 = i;
             * EXT_OPN2_72_ADSR__D2R__CH3_OP1 = i;
 			* EXT_OPN2_70_ADSR__D2R__CH1_OP5 = i;
             * EXT_OPN2_71_ADSR__D2R__CH2_OP5 = i;
             * EXT_OPN2_72_ADSR__D2R__CH3_OP5 = i;
 			i = 0x02;
 			* EXT_OPN2_74_ADSR__D2R__CH1_OP2 = i;
             * EXT_OPN2_75_ADSR__D2R__CH2_OP2 = i;
             * EXT_OPN2_76_ADSR__D2R__CH3_OP2 = i;
 			* EXT_OPN2_74_ADSR__D2R__CH1_OP6 = i;
             * EXT_OPN2_75_ADSR__D2R__CH2_OP6 = i;
             * EXT_OPN2_76_ADSR__D2R__CH3_OP6 = i;
 			i = 0x02;
 			* EXT_OPN2_78_ADSR__D2R__CH1_OP3 = i;
             * EXT_OPN2_79_ADSR__D2R__CH2_OP3 = i;
             * EXT_OPN2_7A_ADSR__D2R__CH3_OP3 = i;
 			* EXT_OPN2_78_ADSR__D2R__CH1_OP7 = i;
             * EXT_OPN2_79_ADSR__D2R__CH2_OP7 = i;
             * EXT_OPN2_7A_ADSR__D2R__CH3_OP7 = i;
 			i = 0x02;
 			* EXT_OPN2_7C_ADSR__D2R__CH1_OP4 = i;
             * EXT_OPN2_7D_ADSR__D2R__CH2_OP4 = i;
             * EXT_OPN2_7E_ADSR__D2R__CH3_OP4 = i;
 			* EXT_OPN2_7C_ADSR__D2R__CH1_OP8 = i;
             * EXT_OPN2_7D_ADSR__D2R__CH2_OP8 = i;
 			* EXT_OPN2_7E_ADSR__D2R__CH3_OP8 = i;

 			i = 0x11;
 			* EXT_OPN2_80_ADSR__D1L_RR__CH1_OP1 = i;
             * EXT_OPN2_81_ADSR__D1L_RR__CH2_OP1 = i;
             * EXT_OPN2_82_ADSR__D1L_RR__CH3_OP1 = i;
 			* EXT_OPN2_80_ADSR__D1L_RR__CH1_OP5 = i;
             * EXT_OPN2_81_ADSR__D1L_RR__CH2_OP5 = i;
             * EXT_OPN2_82_ADSR__D1L_RR__CH3_OP5 = i;
 			i = 0x11;
 			* EXT_OPN2_84_ADSR__D1L_RR__CH1_OP2 = i;
             * EXT_OPN2_85_ADSR__D1L_RR__CH2_OP2 = i;
             * EXT_OPN2_86_ADSR__D1L_RR__CH3_OP2 = i;
 			* EXT_OPN2_84_ADSR__D1L_RR__CH1_OP6 = i;
             * EXT_OPN2_85_ADSR__D1L_RR__CH2_OP6 = i;
             * EXT_OPN2_86_ADSR__D1L_RR__CH3_OP6 = i;
 			i = 0x11;
 			* EXT_OPN2_88_ADSR__D1L_RR__CH1_OP3 = i;
             * EXT_OPN2_89_ADSR__D1L_RR__CH2_OP3 = i;
             * EXT_OPN2_8A_ADSR__D1L_RR__CH3_OP3 = i;
 			* EXT_OPN2_88_ADSR__D1L_RR__CH1_OP7 = i;
             * EXT_OPN2_89_ADSR__D1L_RR__CH2_OP7 = i;
             * EXT_OPN2_8A_ADSR__D1L_RR__CH3_OP7 = i;
 			i = 0xA6;
 			* EXT_OPN2_8C_ADSR__D1L_RR__CH1_OP4 = i;
             * EXT_OPN2_8D_ADSR__D1L_RR__CH2_OP4 = i;
             * EXT_OPN2_8E_ADSR__D1L_RR__CH3_OP4 = i;
 			* EXT_OPN2_8C_ADSR__D1L_RR__CH1_OP8 = i;
             * EXT_OPN2_8D_ADSR__D1L_RR__CH2_OP8 = i;
             * EXT_OPN2_8E_ADSR__D1L_RR__CH3_OP8 = i;
 			i = 0x00;
 			* EXT_OPN2_90_ADSR__D1L_RR__CH1_OP1 = i;
 			* EXT_OPN2_91_ADSR__D1L_RR__CH2_OP1 = i;
             * EXT_OPN2_92_ADSR__D1L_RR__CH3_OP1 = i;
             * EXT_OPN2_94_ADSR__D1L_RR__CH1_OP2 = i;
             * EXT_OPN2_95_ADSR__D1L_RR__CH2_OP2 = i;
             * EXT_OPN2_96_ADSR__D1L_RR__CH3_OP2 = i;
             * EXT_OPN2_98_ADSR__D1L_RR__CH1_OP3 = i;
             * EXT_OPN2_99_ADSR__D1L_RR__CH2_OP3 = i;
             * EXT_OPN2_9A_ADSR__D1L_RR__CH3_OP3 = i;
             * EXT_OPN2_9C_ADSR__D1L_RR__CH1_OP4 = i;
             * EXT_OPN2_9D_ADSR__D1L_RR__CH2_OP4 = i;
             * EXT_OPN2_9E_ADSR__D1L_RR__CH3_OP4 = i;
             * EXT_OPN2_90_ADSR__D1L_RR__CH4_OP1 = i;
             * EXT_OPN2_91_ADSR__D1L_RR__CH5_OP1 = i;
             * EXT_OPN2_92_ADSR__D1L_RR__CH6_OP1 = i;
             * EXT_OPN2_94_ADSR__D1L_RR__CH4_OP2 = i;
             * EXT_OPN2_95_ADSR__D1L_RR__CH5_OP2 = i;
             * EXT_OPN2_96_ADSR__D1L_RR__CH6_OP2 = i;
             * EXT_OPN2_98_ADSR__D1L_RR__CH4_OP3 = i;
             * EXT_OPN2_99_ADSR__D1L_RR__CH5_OP3 = i;
             * EXT_OPN2_9A_ADSR__D1L_RR__CH6_OP3 = i;
             * EXT_OPN2_9C_ADSR__D1L_RR__CH4_OP4 = i;
             * EXT_OPN2_9D_ADSR__D1L_RR__CH5_OP4 = i;
             * EXT_OPN2_9E_ADSR__D1L_RR__CH6_OP4 = i;

 			i = 0x32;
 			* EXT_OPN2_B0_CH1_FEEDBACK_ALGO = i;
             * EXT_OPN2_B1_CH2_FEEDBACK_ALGO = i;
             * EXT_OPN2_B2_CH3_FEEDBACK_ALGO = i;
 			i = 0xC0;
 			* EXT_OPN2_B4_CH4_L_R_AMS_FMS = i;
             * EXT_OPN2_B5_CH5_L_R_AMS_FMS = i;
             * EXT_OPN2_B6_CH6_L_R_AMS_FMS = i;

 			* EXT_OPN2_A4_CH1_OCTAVE_FREQ_H = 0x23;
 			* EXT_OPN2_A5_CH2_OCTAVE_FREQ_H = 0x22;
 			* EXT_OPN2_A6_CH3_OCTAVE_FREQ_H = 0x22;

 			* EXT_OPN2_A0_CH1_FREQ_L = 0x9C;
             * EXT_OPN2_A1_CH2_FREQ_L = 0x9C;
             * EXT_OPN2_A2_CH3_FREQ_L = 0x24;


 			*EXT_OPN2_28_KEY_ON_OFF = 0x00;


 			*EXT_OPN2_28_KEY_ON_OFF = 0xF0;
 			*EXT_OPN2_28_KEY_ON_OFF = 0xF1;
 			*EXT_OPN2_28_KEY_ON_OFF = 0xF2;
 			*EXT_OPN2_28_KEY_ON_OFF = 0xF4;
 			*EXT_OPN2_28_KEY_ON_OFF = 0xF5;
 			*EXT_OPN2_28_KEY_ON_OFF = 0xF6;

 			// 400ms Delay (prolly not quite the time it needs)
 			for (j=0 ; j<262144; j++);

 			* EXT_OPN2_28_KEY_ON_OFF = 0x00;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x01;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x02;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x04;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x05;
 			* EXT_OPN2_28_KEY_ON_OFF = 0x06;
 			// 400ms Delay (prolly not quite the time it needs)
 			for (j=0 ; j<262144; j++);
 }
#endif

#if HAS_OPM
/*
 * Test tone for OPM: register, value
 */
const unsigned char opm_tone_on[] = {
    0x01, 0x00,
    0x0F, 0x00,             /* Turn off the noise generator */
    0x14, 0x00,             /* Turn off the timers and their interrupts */
    0x18, 0x00,             /* Turn off LFO */
    0x19, 0x00,             /* Set AM depth to 0 */
    0x19, 0x80,             /* Set PM depth to 0 */
    0x1B, 0x00,             /* CT, W = 0 */

    0x20, 0x07,             /* Channel 1: No feedback, pattern #7 */
    0x21, 0x4A,             /* Key code: concert A (440Hz) */
    0x22, 0x00,             /* Key fraction: 0 */
    0x23, 0x00,             /* Phase and amplitude modulation off */

    0x08, 0x08,             /* M1, Channel 1: on */

    0x00, 0x00              /* END */
};

/*
 * Test tone for OPM: register, value
 */
const unsigned char opm_tone_off[] = {
    0x08, 0x00,             /* M1, Channel 1: off */

    0x00, 0x00              /* END */
};

/*
 * Play a sound on the OPM
 */
short opm_test(short channel, int argc, const char * argv[]) {
    short i;
    unsigned char reg;
    unsigned char data;
    long target_time;
    volatile unsigned char * opm_base = OPM_INT_BASE;

    Test_EXT_FM2151();
    //
    // if (argc >= 2) {
    //     /* Allow the user to select the external OPM */
    //     if ((strcmp(argv[1], "ext") == 0) || (strcmp(argv[1], "EXT") == 0)) {
    //         opm_base = OPM_EXT_BASE;
    //     }
    // }
    //
    // /* Clear all the OPM registers */
    // for (i = 0; i < 0x100; i++) {
    //     opm_base[reg] = 0;
    // }
    //
    // i = 0;
    // while (1) {
    //     reg = opm_tone_on[i++];
    //     if (reg == 0x00) {
    //         break;
    //     } else {
    //         data = opm_tone_on[i++];
    //         opm_base[reg] = data;
    //     }
    // }
    //
    // target_time = sys_time_jiffies() + 60;
    // while (target_time > sys_time_jiffies()) ;
    //
    // i = 0;
    // while (1) {
    //     reg = opm_tone_off[i++];
    //     if (reg == 0x00) {
    //         break;
    //     } else {
    //         data = opm_tone_off[i++];
    //         opm_base[reg] = data;
    //     }
    // }

    return 0;
}

/*
 * Test tone for OPN: register, value
 */
const unsigned char opn_tone_on[] = {
    0x22, 0x00,             /* Turn off LFO */
    0x27, 0x00,             /* Turn off timers */

    0x28, 0x00,             /* All channels off */
    0x28, 0x01,
    0x28, 0x02,
    0x28, 0x04,
    0x28, 0x05,
    0x28, 0x06,

    0x2B, 0x00,             /* Turn off DAC */

    0x30, 0x71,             /* DT1/MUL */
    0x34, 0x0D,             /* DT1/MUL */
    0x38, 0x33,             /* DT1/MUL */
    0x3C, 0x01,             /* DT1/MUL */

    0x40, 0x23,             /* Total level */
    0x44, 0x2D,
    0x48, 0x26,
    0x4C, 0x00,

    0x50, 0x5F,             /* RS/AR */
    0x54, 0x99,
    0x58, 0x5F,
    0x5C, 0x94,

    0x60, 0x05,             /* AM/D1R */
    0x64, 0x05,
    0x68, 0x05,
    0x6C, 0x07,

    0x70, 0x02,             /* DR2 */
    0x74, 0x02,
    0x78, 0x02,
    0x7C, 0x02,

    0x80, 0x11,             /* D1L/RR */
    0x84, 0x11,
    0x88, 0x11,
    0x8C, 0xA6,

    0x90, 0x00,             /* Proprietary register: always 0 */
    0x94, 0x00,
    0x98, 0x00,
    0x9C, 0x00,

    0xB0, 0x32,             /* Feedback algorithm */
    0xB4, 0xC0,             /* Both speakers on */

    0x28, 0x00,             /* Key off */

    0xA4, 0x22,             /* Set frequency */
    0xA0, 0x69,

    0x28, 0x10,             /* Operator 1, Channel 1: on */

    0x00, 0x00              /* END */
};

/*
 * Test tone for OPN: register, value
 */
const unsigned char opn_tone_off[] = {
    0x28, 0x00,             /* Operator 1, Channel 1: off */
    0x00, 0x00              /* END */
};

/*
 * Play a sound on the OPN
 */
short opn_test(short channel, int argc, const char * argv[]) {
    short i;
    unsigned char reg;
    unsigned char data;
    long target_time;
    volatile unsigned char * opn_base = OPN2_INT_BASE;

    Test_EXT_FM2612();
    //
    // if (argc >= 2) {
    //     /* Allow the user to select the external OPM */
    //     if ((strcmp(argv[1], "ext") == 0) || (strcmp(argv[1], "EXT") == 0)) {
    //         opn_base = OPN2_EXT_BASE;
    //     }
    // }
    //
    // i = 0;
    // while (1) {
    //     reg = opn_tone_on[i++];
    //     if (reg == 0x00) {
    //         break;
    //     } else {
    //         data = opn_tone_on[i++];
    //         opn_base[reg] = data;
    //     }
    // }
    //
    // target_time = sys_time_jiffies() + 60;
    // while (target_time > sys_time_jiffies()) ;
    //
    // i = 0;
    // while (1) {
    //     reg = opn_tone_off[i++];
    //     if (reg == 0x00) {
    //         break;
    //     } else {
    //         data = opn_tone_off[i++];
    //         opn_base[reg] = data;
    //     }
    // }

    return 0;
}
#endif


const uint16_t opl3_tone_on[] = {
#if 1 // 7-channel tone with enveloppes and feeback
//0x01,0x00,              /* initialise */
0x00bd, 0x40, 0x00bd, 0xc0, 0x0043, 0x00, 0x0023, 0x03, 0x0023, 0x23, 0x0063, 0xf0, 0x0063, 0xff, 0x0083, 0x00, 0x0083, 0x05, 0x0043, 0x2b, 0x0023, 0x23, 0x0023, 0x23, 0x00c0, 0x00, 0x00c0, 0x06, 0x0040, 0x00, 0x0020, 0x02, 0x0020, 0x22, 0x0060, 0xf0, 0x0060, 0xf2, 0x0080, 0x40, 0x0080, 0x45, 0x0040, 0x00, 0x0020, 0x22, 0x0020, 0x22, 0x00b0, 0x0a, 0x00a0, 0x8c, 0x00b1, 0x0b, 0x00a1, 0x35, 0x00b2, 0x0b, 0x00a2, 0xd1, 0x00b3, 0x0e, 0x00a3, 0x8b, 0x00b4, 0x06, 0x00a4, 0x8e, 0x00b5, 0x02, 0x00a5, 0x93, 0x00c0, 0x36, 0x00c0, 0x37, 0x00b0, 0x2a, 0x0044, 0x00, 0x0024, 0x03, 0x0024, 0x23, 0x0064, 0xf0, 0x0064, 0xff, 0x0084, 0x00, 0x0084, 0x05, 0x0044, 0x2b, 0x0024, 0x23, 0x0024, 0x23, 0x00c0, 0x36, 0x00c1, 0x06, 0x0041, 0x00, 0x0021, 0x02, 0x0021, 0x22, 0x0061, 0xf0, 0x0061, 0xf2, 0x0081, 0x40, 0x0081, 0x45, 0x0041, 0x00, 0x0021, 0x22, 0x0021, 0x22, 0x00b0, 0x2a, 0x00a0, 0x8c, 0x00b1, 0x0b, 0x00a1, 0x35, 0x00b2, 0x0b, 0x00a2, 0xd1, 0x00b3, 0x0e, 0x00a3, 0x8b, 0x00b4, 0x06, 0x00a4, 0x8e, 0x00b5, 0x02, 0x00a5, 0x93, 0x00c1, 0x36, 0x00c1, 0x37, 0x00b1, 0x2b, 0x0045, 0x00, 0x0025, 0x03, 0x0025, 0x23, 0x0065, 0xf0, 0x0065, 0xff, 0x0085, 0x00, 0x0085, 0x05, 0x0045, 0x2b, 0x0025, 0x23, 0x0025, 0x23, 0x00c0, 0x36, 0x00c2, 0x06, 0x0042, 0x00, 0x0022, 0x02, 0x0022, 0x22, 0x0062, 0xf0, 0x0062, 0xf2, 0x0082, 0x40, 0x0082, 0x45, 0x0042, 0x00, 0x0022, 0x22, 0x0022, 0x22, 0x00b0, 0x2a, 0x00a0, 0x8c, 0x00b1, 0x2b, 0x00a1, 0x35, 0x00b2, 0x0b, 0x00a2, 0xd1, 0x00b3, 0x0e, 0x00a3, 0x8b, 0x00b4, 0x06, 0x00a4, 0x8e, 0x00b5, 0x02, 0x00a5, 0x93, 0x00c2, 0x36, 0x00c2, 0x37, 0x00b2, 0x2b, 0x004b, 0x00, 0x002b, 0x03, 0x002b, 0x23, 0x006b, 0xf0, 0x006b, 0xff, 0x008b, 0x00, 0x008b, 0x05, 0x004b, 0x2b, 0x002b, 0x23, 0x002b, 0x23, 0x00c0, 0x36, 0x00c3, 0x06, 0x0048, 0x00, 0x0028, 0x02, 0x0028, 0x22, 0x0068, 0xf0, 0x0068, 0xf2, 0x0088, 0x40, 0x0088, 0x45, 0x0048, 0x00, 0x0028, 0x22, 0x0028, 0x22, 0x00b0, 0x2a, 0x00a0, 0x8c, 0x00b1, 0x2b, 0x00a1, 0x35, 0x00b2, 0x2b, 0x00a2, 0xd1, 0x00b3, 0x0e, 0x00a3, 0x8b, 0x00b4, 0x06, 0x00a4, 0x8e, 0x00b5, 0x02, 0x00a5, 0x93, 0x00c3, 0x36, 0x00c3, 0x37, 0x00b3, 0x2e, 0x004c, 0x00, 0x002c, 0x03, 0x002c, 0x23, 0x006c, 0xf0, 0x006c, 0xff, 0x008c, 0x00, 0x008c, 0x05, 0x004c, 0x2b, 0x002c, 0x23, 0x002c, 0x23, 0x00c0, 0x36, 0x00c4, 0x06, 0x0049, 0x00, 0x0029, 0x02, 0x0029, 0x22, 0x0069, 0xf0, 0x0069, 0xf2, 0x0089, 0x40, 0x0089, 0x45, 0x0049, 0x00, 0x0029, 0x22, 0x0029, 0x22, 0x00b0, 0x2a, 0x00a0, 0x8c, 0x00b1, 0x2b, 0x00a1, 0x35, 0x00b2, 0x2b, 0x00a2, 0xd1, 0x00b3, 0x2e, 0x00a3, 0x8b, 0x00b4, 0x06, 0x00a4, 0x8e, 0x00b5, 0x02, 0x00a5, 0x93, 0x00c4, 0x36, 0x00c4, 0x37, 0x00b4, 0x26, 0x004d, 0x00, 0x002d, 0x03, 0x002d, 0x23, 0x006d, 0xf0, 0x006d, 0xff, 0x008d, 0x00, 0x008d, 0x05, 0x004d, 0x2b, 0x002d, 0x23, 0x002d, 0x23, 0x00c0, 0x36, 0x00c5, 0x06, 0x004a, 0x00, 0x002a, 0x02, 0x002a, 0x22, 0x006a, 0xf0, 0x006a, 0xf2, 0x008a, 0x40, 0x008a, 0x45, 0x004a, 0x00, 0x002a, 0x22, 0x002a, 0x22, 0x00b0, 0x2a, 0x00a0, 0x8c, 0x00b1, 0x2b, 0x00a1, 0x35, 0x00b2, 0x2b, 0x00a2, 0xd1, 0x00b3, 0x2e, 0x00a3, 0x8b, 0x00b4, 0x26, 0x00a4, 0x8e, 0x00b5, 0x02, 0x00a5, 0x93, 0x00c5, 0x36, 0x00c5, 0x37, 0x00b5, 0x22, 0x0053, 0x00, 0x0033, 0x03, 0x0033, 0x23, 0x0073, 0xf0, 0x0073, 0xff, 0x0093, 0x00, 0x0093, 0x05, 0x0053, 0x2b, 0x0033, 0x23, 0x0033, 0x23, 0x00c0, 0x36, 0x00c6, 0x06, 0x0050, 0x00, 0x0030, 0x02, 0x0030, 0x22, 0x0070, 0xf0, 0x0070, 0xf2, 0x0090, 0x40, 0x0090, 0x45, 0x0050, 0x00, 0x0030, 0x22, 0x0030, 0x22, 0x00b0, 0x2a, 0x00a0, 0x8c, 0x00b1, 0x2b, 0x00a1, 0x35, 0x00b2, 0x2b, 0x00a2, 0xd1, 0x00b3, 0x2e, 0x00a3, 0x8b, 0x00b4, 0x26, 0x00a4, 0x8e, 0x00b5, 0x22, 0x00a5, 0x93, 0x00c6, 0x36, 0x00c6, 0x37, 0x00b6, 0x20,
#else // single tone
    0x01,0x00,              /* initialise */    
    0x05,0x01,              /* OPL3 mode, necessary for stereo */
    0xc0,0x31,              /* both channel, parallel connection */

    /* Carrier settings
     * no amplitude modulation (D7=0), no vibrato (D6=0)
     * sustained envelope type (D5=1), KSR=0 (D4=0)
     * frequency multiplier=1 (D4-D0=1) */
    0x23,0x21,
    /* no volume decrease with pitch (D7-D6=0)
     * no attenuation (D5-D0=0) */
    0x43,0x00,
    /* fast attack (D7-D4=0xF) and decay (D3-D0=0xF) */
    0x63,0xff,
    /* high sustain level (D7-D4=0), slow release rate (D3-D0=5) */
    0x83,0x05,

    /* Modulator settings
     * sustained envelope type, frequency multiplier=0 */
    0x20,0x20,
    /* maximum attenuation, no volume decrease with pitch */
    0x40,0x3f,
    /* Since the modulator signal is attenuated as much as possible, these
     * next two values shouldn't have any effect. */
    0x60,0x44,
    0x80,0x05,

    /* 440Hz */
    0xa0,0x41,

    /* block 0, key on */
    0xb0,0x32,
#endif
    /* end of sequence marker */
    0xffff
};

const uint8_t opl3_tone_off[] = {
    /* block 0 to 6, key off */
    0xb0, 0x00, 0xb1, 0x00, 0xb2, 0x00, 0xb3, 0x00, 0xb4, 0x00, 0xb5, 0x00, 0xb6, 0x00,
    /* end of sequence marker */
    0xff
};

/*
 * Play a sound on the OPL3
 */
short opl3_test(short channel, int argc, const char * argv[]) {
    short i;
    unsigned short reg;
    unsigned char data;
    long target_time;

    // Delay between sending bytes. If none, it seems (on the A2560U at least) some commands are missed.
    // Maybe the FIFO managed by Beatrix to feed the YM262 gets too full?
    // Even a tiny delay is enough to avoid the issue.
    long delay = argc > 0 ? atol(argv[1]) : 10;

    print(channel, "Loading data into OPL3...\n");

    i = 0;
    while (1) {
        reg = opl3_tone_on[i++];
        if (reg == 0xffff)
            break;

        data = opl3_tone_on[i++];
        //print_hex_32(channel, (unsigned long)(&OPL3_PORT[reg]));print(channel, " ");print_hex_8(channel, data);print(channel, "\n");
        OPL3_PORT[reg] = data;
        
        if (delay)
            for (int j=0; j<delay; j++);
    }

    print(channel, "Playing now. Press 'ESC' to quit.");
    while (sys_kbd_scancode() != 0x01 /*ESC scancode*/)
        ;

    i = 0;
    while (1) {
        reg = opl3_tone_off[i++];
        if (reg == 0xff) {
            break;
        } else {
            data = opl3_tone_off[i++];
            OPL3_PORT[reg] = data;
            if (delay)
                for (int j=0; j<delay; j++);
        }
    }

    return 0;
}

#if HAS_MIDI_PORTS
/*
 * Perform a transmit test on the MIDI ports
 */
short midi_tx_test(short channel, int argc, const char * argv[]) {
    const char note_on[] = { 0x90, 0x3c, 0x3f };
    const char note_off[] = { 0x80, 0x3c, 0x00 };
    char message[80];
    unsigned short scancode = 0;
    int i;

    print(channel, "Press '1' to start, 'ESC' to quit.\n");
    while (sys_kbd_scancode() != 0x02) ;

    short midi = sys_chan_open(CDEV_MIDI, "", 0);
    if (midi > 0) {
        while (sys_kbd_scancode() != 0x01) {
            long target_jiffies = 0;

            for (i = 0; i < 3; i++) {
                sys_chan_write_b(midi, note_on[i]);
                print_hex_8(channel, note_on[i]);
                print_c(channel, ' ');
            }

            target_jiffies = sys_time_jiffies() + 60;
            while (target_jiffies > sys_time_jiffies()) ;

            for (i = 0; i < 3; i++) {
                sys_chan_write_b(midi, note_off[i]);
                print_hex_8(channel, note_off[i]);
                print_c(channel, ' ');
            }

            target_jiffies = sys_time_jiffies() + 60;
            while (target_jiffies > sys_time_jiffies()) ;

            print_c(channel, '\n');
        }

        sys_chan_close(midi);
    } else {
        sprintf(message, "Couldn't open MIDI port: %s\n", sys_err_message(midi));
        print(channel, message);
        return 0;
    }
    return 0;
}


/*
 * Perform a receive test on the MIDI ports
 */
short midi_rx_test(short channel, int argc, const char * argv[]) {
    char message[80];
    unsigned short scancode = 0;
    int i;

    print(channel, "Press '1' to start, and 'ESC' to exit test.\n");
    while (sys_kbd_scancode() != 0x02) ;

    short midi = sys_chan_open(CDEV_MIDI, "", 0);
    if (midi > 0) {

        i = 0;
        while (scancode != 0x01) {
            short input = sys_chan_read_b(midi);
            if (input < 0) {
                sprintf(message, "Could not read from MIDI port: %s\n", sys_err_message(input));
                print(channel, message);
                return 0;
            }

            if ((input != 0xf8) && (input != 0xfe)) {
                if ((i % 16) == 0) {
                    sprintf(message, "\n%02X", input);
                    sys_chan_write(channel, message, strlen(message));
                } else {
                    sprintf(message, " %02X", input);
                    sys_chan_write(channel, message, strlen(message));
                }

                i++;
            }

            scancode = sys_kbd_scancode();
        }

        print_c(channel, '\n');
        sys_chan_close(midi);
    } else {
        sprintf(message, "Couldn't open MIDI port: %s\n", sys_err_message(midi));
        print(channel, message);
        return 0;
    }

    return 0;
}

/*
 * Perform a loopback test on the MIDI ports
 */
short midi_loop_test(short channel, int argc, const char * argv[]) {
    char message[80];
    unsigned short scancode = 0;
    unsigned char output;
    short result = 0;

    print(channel, "Plug a MIDI loopback cable between MIDI IN and MIDI OUT.\nThen press '1' to start.\n");
    print(channel, "Press ESC to exit test.\n");

    while (sys_kbd_scancode() != 0x02) ;

    short midi = sys_chan_open(CDEV_MIDI, "", 0);
    if (midi > 0) {
        output = 1;
        while (scancode != 0x01) {
            print(channel, "Sending: ");
            result = sys_chan_write_b(midi, output);
            if (result) {
                sprintf(message, "Unable to write a byte to the MIDI ports: %s\n", sys_err_message(result));
                print(channel, message);
                return 0;
            }
            sprintf(message, "%02X --> ", output);
            print(channel, message);

            short input = sys_chan_read_b(midi);
            if (input < 0) {
                sprintf(message, "Unable to read a byte to the MIDI ports: %s\n", sys_err_message(input));
                print(channel, message);
                return 0;
            }
            sprintf(message, "%02X\n", (unsigned char)input);
            sys_chan_write(channel, message, strlen(message));

            scancode = sys_kbd_scancode();

            output++;
            if (output > 0xfe) {
                output = 1;
            }
        }

        sys_chan_write(channel, "\n", 1);
        sys_chan_close(midi);
    } else {
        sprintf(message, "Couldn't open MIDI port: %s\n", sys_err_message(midi));
        print(channel, message);
        return 0;
    }
    return 0;
}

#endif
