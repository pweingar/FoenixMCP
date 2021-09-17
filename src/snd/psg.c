/*
 * Definitions for the SN76489 PSG
 */

#include "psg.h"

/*
 * Mute all voices on the PSG
 */
void psg_mute_all() {
    /* Set attenuation on all voices to full */
    *PSG_PORT = 0x9F;
    *PSG_PORT = 0xBF;
    *PSG_PORT = 0xDF;
    *PSG_PORT = 0xFF;
}
