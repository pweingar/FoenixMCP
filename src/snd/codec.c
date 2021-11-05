/*
 * Definitions of functions to support the CODEC
 */

#include "snd/codec.h"

static unsigned byte volume = 0xff;

/*
 * Wait for the CODEC to be ready to receive more data.
 */
void codec_wait() {
    while (*CODEC == 0x8000);
}

void init_codec() {
	*CODEC = 0x1A00; 			// R13 - Turn On Headphones
	codec_wait();
    *CODEC = 0x2A1F; 			// R21 - Enable All the Analog In
	codec_wait();
	*CODEC = 0x2301; 			// R17
	codec_wait();
	*CODEC = 0x2C07;			// R22
	codec_wait();
	*CODEC = 0x1402; 			// R10
	codec_wait();
	*CODEC = 0x1602; 			// R11
	codec_wait();
	*CODEC = 0x1845; 			// R12
	codec_wait();

    codec_set_volume(0xff);
}

/*
 * Set the master digital volume
 *
 * Inputs:
 * vol = level of attenuation (0xff = full volume, 0x00 = mute)
 */
void codec_set_volume(unsigned char vol) {
    volume = vol;

    *CODEC = 0x0A00 | (0xFF - (vol & 0xFF));
    *CODEC = 0x0400 | ((vol >> 1) & 0xff);

    codec_wait();
}

/*
 * Return the current volume
 */
unsigned char codec_get_volume() {
    return volume;
}
