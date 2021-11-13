/*
 * Definitions for the SN76489 PSG
 */

#include "psg.h"
#include "sound_reg.h"

/*
 * Mute all voices on the PSG
 */
void psg_mute_all() {
    short voice;

    /* Set attenuation on all voices to full */
    for (voice = 0; voice < 3; voice++) {
        psg_attenuation(voice, 15);
    }
}

/*
 * Set the frequency of one of the three tone voices
 *
 * Inputs:
 * voice = the number of the voice (0 - 2)
 * frequency = the frequency in Hz * 100
 */
void psg_tone(unsigned short voice, int frequency) {
    if (voice < 3) {
        int n = 0;
        if (frequency != 0) {
            n = 357954500 / (32 * frequency);
        }
        *PSG_PORT = (unsigned char)(0x80 | ((voice & 0x03) << 5) | (n & 0x0f));
        *PSG_PORT = (unsigned char)((n & 0x3f0) >> 4);
    }
}

/*
 * Set the attenuation of a voice
 *
 * Inputs:
 * voice = the number of the voice (0 - 3)
 * attenuation = volume level 0: loudest, 15: silent
 */
void psg_attenuation(unsigned short voice, short attenuation) {
    *PSG_PORT = (unsigned char)(0x90 | ((voice & 0x03) << 5) | (attenuation & 0x0f));
}
