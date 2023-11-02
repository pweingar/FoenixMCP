/*
 * Definitions of functions to support the CODEC
 */

#include "stdint.h"
#include "sound_reg.h"
#include "snd/codec.h"

static unsigned char volume = 0xff;

/**
 * @brief Send a value to a CODEC register
 * 
 * @param data the register ID and data to write
 */
static void codec_write(uint16_t data) {
	*CODEC = data;
	*CODEC_WR_CTRL = 0x01;

	while (*CODEC_WR_CTRL)
		;
}

/*
 * Set the master digital volume
 *
 * Inputs:
 * vol = level of attenuation (0xff = full volume, 0x00 = mute)
 */
void codec_set_volume(unsigned char vol) {
    volume = vol;

	codec_write(0x0A00 | (0xFF - vol));
	codec_write(0x0400 | (vol >> 1));
}

/*
 * Return the current volume
 */
unsigned char codec_get_volume() {
    return volume;
}

/*
 * Initialize the CODEC registers
 */
void init_codec() {
	codec_write(0x1A00);			// R13 - Turn On Headphones
    codec_write(0x2A1F); 			// R21 - Enable All the Analog In
	codec_write(0x2301); 			// R17
	codec_write(0x2C07);			// R22 - Enable all output sources
	codec_write(0x1402); 			// R10
	codec_write(0x1602); 			// R11
	codec_write(0x1845); 			// R12

    codec_set_volume(0xff);
}