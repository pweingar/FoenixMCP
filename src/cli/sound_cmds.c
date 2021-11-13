/*
 * Commands to work with the sound devices
 */

#include "timers.h"
#include "sound_cmds.h"
#include "sound_reg.h"
#include "snd/psg.h"
#include "snd/opl2.h"
#include "dev/midi.h"

/*
 * Play a sound on the PSG
 */
short psg_test(short channel, int argc, char * argv[]) {
    int i;
    long target_time;

    psg_tone(0, 34923);
    psg_attenuation(0, 0);

    target_time = sys_time_jiffies() + 6;
    while (target_time > sys_time_jiffies()) ;

    psg_tone(1, 44000);
    psg_attenuation(1, 0);

    target_time = sys_time_jiffies() + 6;
    while (target_time > sys_time_jiffies()) ;

    psg_tone(2, 52325);
    psg_attenuation(2, 0);

    target_time = sys_time_jiffies() + 60;
    while (target_time > sys_time_jiffies()) ;

    psg_attenuation(0, 15);
    psg_tone(0, 0);
    psg_attenuation(1, 15);
    psg_tone(1, 0);
    psg_attenuation(2, 15);
    psg_tone(2, 0);

    return 0;
}

const unsigned char opl3_tone_on[] = {
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

    /* end of sequence marker */
    0xff
};

const unsigned char opl3_tone_off[] = {
    /* block 0, key off */
    0xb0,0x00,

    /* end of sequence marker */
    0xff
};

/*
 * Play a sound on the OPL3
 */
short opl3_test(short channel, int argc, char * argv[]) {
    short i;
    unsigned char reg;
    unsigned char data;
    long target_time;

    i = 0;
    while (1) {
        reg = opl3_tone_on[i++];
        if (reg == 0xff) {
            break;
        } else {
            data = opl3_tone_on[i++];
            OPL3_PORT[reg] = data;
        }
    }

    target_time = sys_time_jiffies() + 60;
    while (target_time > sys_time_jiffies()) ;

    i = 0;
    while (1) {
        reg = opl3_tone_off[i++];
        if (reg == 0xff) {
            break;
        } else {
            data = opl3_tone_off[i++];
            OPL3_PORT[reg] = data;
        }
    }

    return 0;
}


/*
 * Perform a transmit test on the MIDI ports
 */
short midi_tx_test(short channel, int argc, char * argv[]) {
#if MODEL == MODEL_FOENIX_A2560K
    const char note_on[] = { 0x90, 0x3c, 0xff };
    const char note_off[] = { 0x80, 0x3c, 0x00 };
    char message[80];
    unsigned short scancode = 0;
    int i;

    midi_init();

    for (i = 0; i < 3; i++) {
        midi_put(note_on[i]);
    }

    for (i = 0; i < 65535 * 20; i++) ;

    for (i = 0; i < 3; i++) {
        midi_put(note_off[i]);
    }
#endif
    return 0;
}

/*
 * Perform a receive test on the MIDI ports
 */
short midi_rx_test(short channel, int argc, char * argv[]) {
#if MODEL == MODEL_FOENIX_A2560K
    char message[80];
    unsigned short scancode = 0;
    int i;

    midi_init();

    sprintf(message, "Press '1' to start, and 'ESC' to exit test.\n");
    sys_chan_write(channel, message, strlen(message));

    while (sys_kbd_scancode() != 0x02) ;

    i = 0;
    while (scancode != 0x01) {
        unsigned char input = midi_get_poll();
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

    sys_chan_write(channel, "\n", 1);
#endif
    return 0;
}


/*
 * Perform a loopback test on the MIDI ports
 */
short midi_loop_test(short channel, int argc, char * argv[]) {
#if MODEL == MODEL_FOENIX_A2560K
    char message[80];
    unsigned short scancode = 0;
    unsigned char output;

    midi_init();

    sprintf(message, "Plug a MIDI loopback cable between MIDI IN and MIDI OUT.\nThen press '1' to start.\n");
    sys_chan_write(channel, message, strlen(message));

    sprintf(message, "Press ESC to exit test.\n");
    sys_chan_write(channel, message, strlen(message));

    while (sys_kbd_scancode() != 0x02) ;

    output = 1;
    while (scancode != 0x01) {
        sprintf(message, "Sending: ");
        sys_chan_write(channel, message, strlen(message));
        midi_put(output);
        sprintf(message, "%02X --> ", output);
        sys_chan_write(channel, message, strlen(message));

        unsigned char input = midi_get_poll();
        sprintf(message, "%02X\n", input);
        sys_chan_write(channel, message, strlen(message));

        scancode = sys_kbd_scancode();

        output++;
        if (output > 0xfe) {
            output = 1;
        }
    }

    sys_chan_write(channel, "\n", 1);
#endif
    return 0;
}

/*
 * Test the OPL2
 */
short opl2_test(short channel, int argc, char * argv[]) {
    unsigned char i;

    OPN2_INT_BASE[OPN2_22_LFO] = 0;     /* LFO off */
    OPN2_INT_BASE[OPN2_27_CHANEL_3_MODE] = 0;   /* chanel 3 in normal mode */

    /* switch off all chanal */
    for (i = 0; i < 7; i++) {
        OPN2_INT_BASE[OPN2_28_KEY_ON_OFF] = i;
    }

    /* ADC off */
    OPN2_INT_BASE[OPN2_2B_ADC_EN] = 0;

    /* DT1/MUL */
    OPN2_INT_BASE[OPN2_30_ADSR__DT1_MUL__CH1_OP1] = 0x71;
    OPN2_INT_BASE[OPN2_31_ADSR__DT1_MUL__CH2_OP1] = 0x71;
    OPN2_INT_BASE[OPN2_32_ADSR__DT1_MUL__CH3_OP1] = 0x71;
    OPN2_INT_BASE[OPN2_30_ADSR__DT1_MUL__CH1_OP5] = 0x71;
    OPN2_INT_BASE[OPN2_31_ADSR__DT1_MUL__CH2_OP5] = 0x71;
    OPN2_INT_BASE[OPN2_32_ADSR__DT1_MUL__CH3_OP5] = 0x71;

    OPN2_INT_BASE[OPN2_34_ADSR__DT1_MUL__CH1_OP2] = 0x0D;
    OPN2_INT_BASE[OPN2_35_ADSR__DT1_MUL__CH2_OP2] = 0x0D;
    OPN2_INT_BASE[OPN2_36_ADSR__DT1_MUL__CH3_OP2] = 0x0D;
    OPN2_INT_BASE[OPN2_34_ADSR__DT1_MUL__CH1_OP6] = 0x0D;
    OPN2_INT_BASE[OPN2_35_ADSR__DT1_MUL__CH2_OP6] = 0x0D;
    OPN2_INT_BASE[OPN2_36_ADSR__DT1_MUL__CH3_OP6] = 0x0D;

    OPN2_INT_BASE[OPN2_38_ADSR__DT1_MUL__CH1_OP3] = 0x33;
    OPN2_INT_BASE[OPN2_39_ADSR__DT1_MUL__CH2_OP3] = 0x33;
    OPN2_INT_BASE[OPN2_3A_ADSR__DT1_MUL__CH3_OP3] = 0x33;
    OPN2_INT_BASE[OPN2_38_ADSR__DT1_MUL__CH1_OP7] = 0x33;
    OPN2_INT_BASE[OPN2_39_ADSR__DT1_MUL__CH2_OP7] = 0x33;
    OPN2_INT_BASE[OPN2_3A_ADSR__DT1_MUL__CH3_OP7] = 0x33;

    OPN2_INT_BASE[OPN2_3C_ADSR__DT1_MUL__CH1_OP4] = 0x01;
    OPN2_INT_BASE[OPN2_3D_ADSR__DT1_MUL__CH2_OP4] = 0x01;
    OPN2_INT_BASE[OPN2_3E_ADSR__DT1_MUL__CH3_OP4] = 0x01;
    OPN2_INT_BASE[OPN2_3C_ADSR__DT1_MUL__CH1_OP8] = 0x01;
    OPN2_INT_BASE[OPN2_3D_ADSR__DT1_MUL__CH2_OP8] = 0x01;
    OPN2_INT_BASE[OPN2_3E_ADSR__DT1_MUL__CH3_OP8] = 0x01;

    /* Total Level */
    OPN2_INT_BASE[OPN2_40_ADSR__LT__CH1_OP1] = 0x23;
    OPN2_INT_BASE[OPN2_41_ADSR__LT__CH2_OP1] = 0x23;
    OPN2_INT_BASE[OPN2_42_ADSR__LT__CH3_OP1] = 0x23;
    OPN2_INT_BASE[OPN2_40_ADSR__LT__CH1_OP5] = 0x23;
    OPN2_INT_BASE[OPN2_41_ADSR__LT__CH2_OP5] = 0x23;
    OPN2_INT_BASE[OPN2_42_ADSR__LT__CH3_OP5] = 0x23;

    OPN2_INT_BASE[OPN2_44_ADSR__LT__CH1_OP2] = 0x2D;
    OPN2_INT_BASE[OPN2_45_ADSR__LT__CH2_OP2] = 0x2D;
    OPN2_INT_BASE[OPN2_46_ADSR__LT__CH3_OP2] = 0x2D;
    OPN2_INT_BASE[OPN2_44_ADSR__LT__CH1_OP6] = 0x2D;
    OPN2_INT_BASE[OPN2_45_ADSR__LT__CH2_OP6] = 0x2D;
    OPN2_INT_BASE[OPN2_46_ADSR__LT__CH3_OP6] = 0x2D;

    OPN2_INT_BASE[OPN2_48_ADSR__LT__CH1_OP3] = 0x26;
    OPN2_INT_BASE[OPN2_49_ADSR__LT__CH2_OP3] = 0x26;
    OPN2_INT_BASE[OPN2_4A_ADSR__LT__CH3_OP3] = 0x26;
    OPN2_INT_BASE[OPN2_48_ADSR__LT__CH1_OP7] = 0x26;
    OPN2_INT_BASE[OPN2_49_ADSR__LT__CH2_OP7] = 0x26;
    OPN2_INT_BASE[OPN2_4A_ADSR__LT__CH3_OP7] = 0x26;

    OPN2_INT_BASE[OPN2_4C_ADSR__LT__CH1_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_4D_ADSR__LT__CH2_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_4E_ADSR__LT__CH3_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_4C_ADSR__LT__CH1_OP8] = 0x00;
    OPN2_INT_BASE[OPN2_4D_ADSR__LT__CH2_OP8] = 0x00;
    OPN2_INT_BASE[OPN2_4E_ADSR__LT__CH3_OP8] = 0x00;

    /* RS/AR */

    OPN2_INT_BASE[OPN2_50_ADSR__SR_AR__CH1_OP1] = 0x5F;
    OPN2_INT_BASE[OPN2_51_ADSR__SR_AR__CH2_OP1] = 0x5F;
    OPN2_INT_BASE[OPN2_52_ADSR__SR_AR__CH3_OP1] = 0x5F;
    OPN2_INT_BASE[OPN2_50_ADSR__SR_AR__CH1_OP5] = 0x5F;
    OPN2_INT_BASE[OPN2_51_ADSR__SR_AR__CH2_OP5] = 0x5F;
    OPN2_INT_BASE[OPN2_52_ADSR__SR_AR__CH3_OP5] = 0x5F;

    OPN2_INT_BASE[OPN2_54_ADSR__SR_AR__CH1_OP2] = 0x99;
    OPN2_INT_BASE[OPN2_55_ADSR__SR_AR__CH2_OP2] = 0x99;
    OPN2_INT_BASE[OPN2_56_ADSR__SR_AR__CH3_OP2] = 0x99;
    OPN2_INT_BASE[OPN2_54_ADSR__SR_AR__CH1_OP6] = 0x99;
    OPN2_INT_BASE[OPN2_55_ADSR__SR_AR__CH2_OP6] = 0x99;
    OPN2_INT_BASE[OPN2_56_ADSR__SR_AR__CH3_OP6] = 0x99;

    OPN2_INT_BASE[OPN2_58_ADSR__SR_AR__CH1_OP3] = 0x5F;
    OPN2_INT_BASE[OPN2_59_ADSR__SR_AR__CH2_OP3] = 0x5F;
    OPN2_INT_BASE[OPN2_5A_ADSR__SR_AR__CH3_OP3] = 0x5F;
    OPN2_INT_BASE[OPN2_58_ADSR__SR_AR__CH1_OP7] = 0x5F;
    OPN2_INT_BASE[OPN2_59_ADSR__SR_AR__CH2_OP7] = 0x5F;
    OPN2_INT_BASE[OPN2_5A_ADSR__SR_AR__CH3_OP7] = 0x5F;

    OPN2_INT_BASE[OPN2_5C_ADSR__SR_AR__CH1_OP4] = 0x94;
    OPN2_INT_BASE[OPN2_5D_ADSR__SR_AR__CH2_OP4] = 0x94;
    OPN2_INT_BASE[OPN2_5E_ADSR__SR_AR__CH3_OP4] = 0x94;
    OPN2_INT_BASE[OPN2_5C_ADSR__SR_AR__CH1_OP8] = 0x94;
    OPN2_INT_BASE[OPN2_5D_ADSR__SR_AR__CH2_OP8] = 0x94;
    OPN2_INT_BASE[OPN2_5E_ADSR__SR_AR__CH3_OP8] = 0x94;

    /* AM/D1R */

    OPN2_INT_BASE[OPN2_60_ADSR__AM_D1R__CH1_OP1] = 0x07;
    OPN2_INT_BASE[OPN2_61_ADSR__AM_D1R__CH2_OP1] = 0x07;
    OPN2_INT_BASE[OPN2_62_ADSR__AM_D1R__CH3_OP1] = 0x07;
    OPN2_INT_BASE[OPN2_60_ADSR__AM_D1R__CH1_OP5] = 0x07;
    OPN2_INT_BASE[OPN2_61_ADSR__AM_D1R__CH2_OP5] = 0x07;
    OPN2_INT_BASE[OPN2_62_ADSR__AM_D1R__CH3_OP5] = 0x07;

    OPN2_INT_BASE[OPN2_64_ADSR__AM_D1R__CH1_OP2] = 0x07;
    OPN2_INT_BASE[OPN2_65_ADSR__AM_D1R__CH2_OP2] = 0x07;
    OPN2_INT_BASE[OPN2_66_ADSR__AM_D1R__CH3_OP2] = 0x07;
    OPN2_INT_BASE[OPN2_64_ADSR__AM_D1R__CH1_OP6] = 0x07;
    OPN2_INT_BASE[OPN2_65_ADSR__AM_D1R__CH2_OP6] = 0x07;
    OPN2_INT_BASE[OPN2_66_ADSR__AM_D1R__CH3_OP6] = 0x07;

    OPN2_INT_BASE[OPN2_68_ADSR__AM_D1R__CH1_OP3] = 0x05;
    OPN2_INT_BASE[OPN2_69_ADSR__AM_D1R__CH2_OP3] = 0x05;
    OPN2_INT_BASE[OPN2_6A_ADSR__AM_D1R__CH3_OP3] = 0x05;
    OPN2_INT_BASE[OPN2_68_ADSR__AM_D1R__CH1_OP7] = 0x05;
    OPN2_INT_BASE[OPN2_69_ADSR__AM_D1R__CH2_OP7] = 0x05;
    OPN2_INT_BASE[OPN2_6A_ADSR__AM_D1R__CH3_OP7] = 0x05;

    OPN2_INT_BASE[OPN2_6C_ADSR__AM_D1R__CH1_OP4] = 0x07;
    OPN2_INT_BASE[OPN2_6D_ADSR__AM_D1R__CH2_OP4] = 0x07;
    OPN2_INT_BASE[OPN2_6E_ADSR__AM_D1R__CH3_OP4] = 0x07;
    OPN2_INT_BASE[OPN2_6C_ADSR__AM_D1R__CH1_OP8] = 0x07;
    OPN2_INT_BASE[OPN2_6D_ADSR__AM_D1R__CH2_OP8] = 0x07;
    OPN2_INT_BASE[OPN2_6E_ADSR__AM_D1R__CH3_OP8] = 0x07;


    /* D2R */

    OPN2_INT_BASE[OPN2_70_ADSR__D2R__CH1_OP1] = 0x02;
    OPN2_INT_BASE[OPN2_71_ADSR__D2R__CH2_OP1] = 0x02;
    OPN2_INT_BASE[OPN2_72_ADSR__D2R__CH3_OP1] = 0x02;
    OPN2_INT_BASE[OPN2_70_ADSR__D2R__CH1_OP5] = 0x02;
    OPN2_INT_BASE[OPN2_71_ADSR__D2R__CH2_OP5] = 0x02;
    OPN2_INT_BASE[OPN2_72_ADSR__D2R__CH3_OP5] = 0x02;

    OPN2_INT_BASE[OPN2_74_ADSR__D2R__CH1_OP2] = 0x02;
    OPN2_INT_BASE[OPN2_75_ADSR__D2R__CH2_OP2] = 0x02;
    OPN2_INT_BASE[OPN2_76_ADSR__D2R__CH3_OP2] = 0x02;
    OPN2_INT_BASE[OPN2_74_ADSR__D2R__CH1_OP6] = 0x02;
    OPN2_INT_BASE[OPN2_75_ADSR__D2R__CH2_OP6] = 0x02;
    OPN2_INT_BASE[OPN2_76_ADSR__D2R__CH3_OP6] = 0x02;

    OPN2_INT_BASE[OPN2_78_ADSR__D2R__CH1_OP3] = 0x02;
    OPN2_INT_BASE[OPN2_79_ADSR__D2R__CH2_OP3] = 0x02;
    OPN2_INT_BASE[OPN2_7A_ADSR__D2R__CH3_OP3] = 0x02;
    OPN2_INT_BASE[OPN2_78_ADSR__D2R__CH1_OP7] = 0x02;
    OPN2_INT_BASE[OPN2_79_ADSR__D2R__CH2_OP7] = 0x02;
    OPN2_INT_BASE[OPN2_7A_ADSR__D2R__CH3_OP7] = 0x02;

    OPN2_INT_BASE[OPN2_7C_ADSR__D2R__CH1_OP4] = 0x02;
    OPN2_INT_BASE[OPN2_7D_ADSR__D2R__CH2_OP4] = 0x02;
    OPN2_INT_BASE[OPN2_7E_ADSR__D2R__CH3_OP4] = 0x02;
    OPN2_INT_BASE[OPN2_7C_ADSR__D2R__CH1_OP8] = 0x02;
    OPN2_INT_BASE[OPN2_7D_ADSR__D2R__CH2_OP8] = 0x02;
    OPN2_INT_BASE[OPN2_7E_ADSR__D2R__CH3_OP8] = 0x02;

    /* D1L/RR */

    OPN2_INT_BASE[OPN2_80_ADSR__D1L_RR__CH1_OP1] = 0x11;
    OPN2_INT_BASE[OPN2_81_ADSR__D1L_RR__CH2_OP1] = 0x11;
    OPN2_INT_BASE[OPN2_82_ADSR__D1L_RR__CH3_OP1] = 0x11;
    OPN2_INT_BASE[OPN2_80_ADSR__D1L_RR__CH1_OP5] = 0x11;
    OPN2_INT_BASE[OPN2_81_ADSR__D1L_RR__CH2_OP5] = 0x11;
    OPN2_INT_BASE[OPN2_82_ADSR__D1L_RR__CH3_OP5] = 0x11;

    OPN2_INT_BASE[OPN2_84_ADSR__D1L_RR__CH1_OP2] = 0x11;
    OPN2_INT_BASE[OPN2_85_ADSR__D1L_RR__CH2_OP2] = 0x11;
    OPN2_INT_BASE[OPN2_86_ADSR__D1L_RR__CH3_OP2] = 0x11;
    OPN2_INT_BASE[OPN2_84_ADSR__D1L_RR__CH1_OP6] = 0x11;
    OPN2_INT_BASE[OPN2_85_ADSR__D1L_RR__CH2_OP6] = 0x11;
    OPN2_INT_BASE[OPN2_86_ADSR__D1L_RR__CH3_OP6] = 0x11;

    OPN2_INT_BASE[OPN2_88_ADSR__D1L_RR__CH1_OP3] = 0x11;
    OPN2_INT_BASE[OPN2_89_ADSR__D1L_RR__CH2_OP3] = 0x11;
    OPN2_INT_BASE[OPN2_8A_ADSR__D1L_RR__CH3_OP3] = 0x11;
    OPN2_INT_BASE[OPN2_88_ADSR__D1L_RR__CH1_OP7] = 0x11;
    OPN2_INT_BASE[OPN2_89_ADSR__D1L_RR__CH2_OP7] = 0x11;
    OPN2_INT_BASE[OPN2_8A_ADSR__D1L_RR__CH3_OP7] = 0x11;

    OPN2_INT_BASE[OPN2_8C_ADSR__D1L_RR__CH1_OP4] = 0xA6;
    OPN2_INT_BASE[OPN2_8D_ADSR__D1L_RR__CH2_OP4] = 0xA6;
    OPN2_INT_BASE[OPN2_8E_ADSR__D1L_RR__CH3_OP4] = 0xA6;
    OPN2_INT_BASE[OPN2_8C_ADSR__D1L_RR__CH1_OP8] = 0xA6;
    OPN2_INT_BASE[OPN2_8D_ADSR__D1L_RR__CH2_OP8] = 0xA6;
    OPN2_INT_BASE[OPN2_8E_ADSR__D1L_RR__CH3_OP8] = 0xA6;

    /* Proprietary */

    OPN2_INT_BASE[OPN2_90_ADSR__D1L_RR__CH1_OP1] = 0x00;
    OPN2_INT_BASE[OPN2_91_ADSR__D1L_RR__CH2_OP1] = 0x00;
    OPN2_INT_BASE[OPN2_92_ADSR__D1L_RR__CH3_OP1] = 0x00;
    OPN2_INT_BASE[OPN2_90_ADSR__D1L_RR__CH4_OP1] = 0x00;
    OPN2_INT_BASE[OPN2_91_ADSR__D1L_RR__CH5_OP1] = 0x00;
    OPN2_INT_BASE[OPN2_92_ADSR__D1L_RR__CH6_OP1] = 0x00;

    OPN2_INT_BASE[OPN2_94_ADSR__D1L_RR__CH1_OP2] = 0x00;
    OPN2_INT_BASE[OPN2_95_ADSR__D1L_RR__CH2_OP2] = 0x00;
    OPN2_INT_BASE[OPN2_96_ADSR__D1L_RR__CH3_OP2] = 0x00;
    OPN2_INT_BASE[OPN2_94_ADSR__D1L_RR__CH4_OP2] = 0x00;
    OPN2_INT_BASE[OPN2_95_ADSR__D1L_RR__CH5_OP2] = 0x00;
    OPN2_INT_BASE[OPN2_96_ADSR__D1L_RR__CH6_OP2] = 0x00;

    OPN2_INT_BASE[OPN2_98_ADSR__D1L_RR__CH1_OP3] = 0x00;
    OPN2_INT_BASE[OPN2_99_ADSR__D1L_RR__CH2_OP3] = 0x00;
    OPN2_INT_BASE[OPN2_9A_ADSR__D1L_RR__CH3_OP3] = 0x00;
    OPN2_INT_BASE[OPN2_98_ADSR__D1L_RR__CH4_OP3] = 0x00;
    OPN2_INT_BASE[OPN2_99_ADSR__D1L_RR__CH5_OP3] = 0x00;
    OPN2_INT_BASE[OPN2_9A_ADSR__D1L_RR__CH6_OP3] = 0x00;

    OPN2_INT_BASE[OPN2_9C_ADSR__D1L_RR__CH1_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_9D_ADSR__D1L_RR__CH2_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_9E_ADSR__D1L_RR__CH3_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_9C_ADSR__D1L_RR__CH4_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_9D_ADSR__D1L_RR__CH5_OP4] = 0x00;
    OPN2_INT_BASE[OPN2_9E_ADSR__D1L_RR__CH6_OP4] = 0x00;


    /* Feedback/algorithm */
    OPN2_INT_BASE[OPN2_B0_CH1_FEEDBACK_ALGO] = 0x32;
    OPN2_INT_BASE[OPN2_B1_CH2_FEEDBACK_ALGO] = 0x32;
    OPN2_INT_BASE[OPN2_B2_CH3_FEEDBACK_ALGO] = 0x32;

    /* 	Both speakers on */
    OPN2_INT_BASE[OPN2_B4_CH1_L_R_AMS_FMS] = 0xC0;
    OPN2_INT_BASE[OPN2_B5_CH2_L_R_AMS_FMS] = 0xC0;
    OPN2_INT_BASE[OPN2_B6_CH3_L_R_AMS_FMS] = 0xC0;

    /* Set frequency */
    OPN2_INT_BASE[OPN2_A4_CH1_OCTAVE_FRECANCY_H] = 0x23;
    OPN2_INT_BASE[OPN2_A5_CH2_OCTAVE_FRECANCY_H] = 0x22;
    OPN2_INT_BASE[OPN2_A6_CH3_OCTAVE_FRECANCY_H] = 0x22;

    OPN2_INT_BASE[OPN2_A0_CH1_FRECANCY_L] = 0x96;
    OPN2_INT_BASE[OPN2_A1_CH2_FRECANCY_L] = 0x96;
    OPN2_INT_BASE[OPN2_A2_CH3_FRECANCY_L] = 0x24;

    /* Key off */
    OPN2_INT_BASE[OPN2_28_KEY_ON_OFF] = 0x00;


    for (i = 0xF0; i < 0xF7; i++) {
        OPN2_INT_BASE[OPN2_28_KEY_ON_OFF] = i;
    }

    long target_ticks = sys_time_jiffies();
    while (target_ticks > sys_time_jiffies() + 300) ;

    for (i = 0x00; i < 0x07; i++) {
        OPN2_INT_BASE[OPN2_28_KEY_ON_OFF] = i;
    }

    return 0;
}
