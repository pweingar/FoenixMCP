/*
 * Declarations for the SN76489 PSG
 */

#ifndef __PSG_H
#define __PSG_H

/*
 * Mute all voices on the PSG
 */
extern void psg_mute_all();

/*
 * Set the frequency of one of the three tone voices
 *
 * Inputs:
 * chip = the chip to use (0 = external, 1 = internal left, 2 = internal right, 3 = internal stereo)
 * voice = the number of the voice (0 - 3)
 * frequency = the frequency
 */
extern void psg_tone(short chip, unsigned short voice, int frequency);

/*
 * Set the attenuation of a voice
 *
 * Inputs:
 * chip = the chip to use (0 = external, 1 = internal left, 2 = internal right, 3 = internal stereo)
 * voice = the number of the voice (0 - 3)
 * attenuation = volume level 0 = loudest, 15 = silent
 */
extern void psg_attenuation(short chip, unsigned short voice, short attenuation);

#endif
