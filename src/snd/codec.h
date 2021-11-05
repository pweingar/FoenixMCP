/*
 * Declarations for the CODEC
 */

#ifndef __CODEC_H
#define __CODEC_H

#include "sound_reg.h"

/*
 * Initialize the CODEC registers
 */
extern void init_codec();

/*
 * Set the master digital volume
 *
 * Inputs:
 * vol = level of attenuation (0xff = full volume, 0x00 = mute)
 */
void codec_set_volume(unsigned char vol);

/*
 * Return the current volume
 */
extern unsigned char codec_get_volume();

#endif
