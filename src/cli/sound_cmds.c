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
short opm_test(short channel, int argc, char * argv[]) {
    short i;
    unsigned char reg;
    unsigned char data;
    long target_time;
    unsigned char * opm_base = OPM_INT_BASE;

    if (argc >= 2) {
        /* Allow the user to select the external OPM */
        if ((strcmp(argv[1], "ext") == 0) || (strcmp(argv[1], "EXT") == 0)) {
            opm_base = OPM_EXT_BASE;
        }
    }

    /* Clear all the OPM registers */
    for (i = 0; i < 0x100; i++) {
        opm_base[reg] = 0;
    }

    i = 0;
    while (1) {
        reg = opm_tone_on[i++];
        if (reg == 0x00) {
            break;
        } else {
            data = opm_tone_on[i++];
            opm_base[reg] = data;
        }
    }

    target_time = sys_time_jiffies() + 60;
    while (target_time > sys_time_jiffies()) ;

    i = 0;
    while (1) {
        reg = opm_tone_off[i++];
        if (reg == 0x00) {
            break;
        } else {
            data = opm_tone_off[i++];
            opm_base[reg] = data;
        }
    }

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
short opn_test(short channel, int argc, char * argv[]) {
    short i;
    unsigned char reg;
    unsigned char data;
    long target_time;
    unsigned char * opn_base = OPN2_INT_BASE;

    if (argc >= 2) {
        /* Allow the user to select the external OPM */
        if ((strcmp(argv[1], "ext") == 0) || (strcmp(argv[1], "EXT") == 0)) {
            opn_base = OPN2_EXT_BASE;
        }
    }

    i = 0;
    while (1) {
        reg = opn_tone_on[i++];
        if (reg == 0x00) {
            break;
        } else {
            data = opn_tone_on[i++];
            opn_base[reg] = data;
        }
    }

    target_time = sys_time_jiffies() + 60;
    while (target_time > sys_time_jiffies()) ;

    i = 0;
    while (1) {
        reg = opn_tone_off[i++];
        if (reg == 0x00) {
            break;
        } else {
            data = opn_tone_off[i++];
            opn_base[reg] = data;
        }
    }

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
