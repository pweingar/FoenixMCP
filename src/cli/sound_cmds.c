/*
 * Commands to work with the sound devices
 */

#include "timers.h"
#include "sound_cmds.h"
#include "sound_reg.h"
#include "snd/psg.h"

/*
 * Play a sound on the PSG
 */
short psg_test(short channel, int argc, char * argv[]) {
    int i;
    long target_time;

    psg_tone(0, 34923);
    psg_attenuation(0, 0);

    target_time = timers_jiffies() + 20;
    while (target_time > timers_jiffies()) ;

    psg_tone(1, 44000);
    psg_attenuation(1, 0);

    target_time = timers_jiffies() + 20;
    while (target_time > timers_jiffies()) ;

    psg_tone(2, 52325);
    psg_attenuation(2, 0);

    target_time = timers_jiffies() + 60;
    while (target_time > timers_jiffies()) ;

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

    target_time = timers_jiffies() + 60;
    while (target_time > timers_jiffies()) ;

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
