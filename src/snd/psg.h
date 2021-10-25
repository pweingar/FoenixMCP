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
 * voice = the number of the voice (0 - 3)
 * frequency = the frequency
 */
extern void psg_tone(unsigned short voice, int frequency);

/*
 * Set the attenuation of a voice
 *
 * Inputs:
 * voice = the number of the voice (0 - 3)
 * attenuation = volume level 0 = loudest, 15 = silent
 */
extern void psg_attenuation(unsigned short voice, short attenuation);

#endif
