/*
 * Definitions for the SN76489 PSG
 */

#include "psg.h"
#include "sound_reg.h"

/*
 * Mute all voices on the PSG
 */
void psg_mute_all() {
    short chip;
    short voice;

    for (chip = 0; chip < 4; chip++) {
        /* Set attenuation on all voices to full */
        for (voice = 0; voice < 4; voice++) {
            psg_attenuation(chip, voice, 15);
        }
    }
}

/*
 * Set the frequency of one of the three tone voices
 *
 * Inputs:
 * chip = the chip to use (0 = external, 1 = internal left, 2 = internal right, 3 = internal stereo)
 * voice = the number of the voice (0 - 3)
 * frequency = the frequency
 */
void psg_tone(short chip, unsigned short voice, int frequency) {
    volatile unsigned char * port = PSG_PORT;     /* By default: external */

    if (voice < 3) {
#if MODEL == MODEL_FOENIX_A2560K
        switch (chip) {
            case 1:
                port = PSG_INT_L_PORT;
                break;

            case 2:
                port = PSG_INT_R_PORT;
                break;

            case 3:
                port = PSG_INT_S_PORT;
                break;

            default:
                break;
        }
#endif

        int n = 0;
        if (frequency != 0) {
            n = 357954500 / (32 * frequency);
        }
        *port = (unsigned char)(0x80 | ((voice & 0x03) << 5) | (n & 0x0f));
        *port = (unsigned char)((n & 0x3f0) >> 4);
    }
}

/*
 * Set the attenuation of a voice
 *
 * Inputs:
 * chip = the chip to use (0 = external, 1 = internal left, 2 = internal right, 3 = internal stereo)
 * voice = the number of the voice (0 - 3)
 * attenuation = volume level 0 = loudest, 15 = silent
 */
void psg_attenuation(short chip, unsigned short voice, short attenuation) {
    volatile unsigned char * port = PSG_PORT;     /* By default: external */

#if MODEL == MODEL_FOENIX_A2560K
    switch (chip) {
        case 1:
            port = PSG_INT_L_PORT;
            break;

        case 2:
            port = PSG_INT_R_PORT;
            break;

        case 3:
            port = PSG_INT_S_PORT;
            break;

        default:
            break;
    }
#endif

    *port = (unsigned char)(0x90 | ((voice & 0x03) << 5) | (attenuation & 0x0f));
}
