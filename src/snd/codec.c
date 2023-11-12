/*
 * Definitions of functions to support the CODEC
 */

#include "snd/codec.h"

static unsigned char volume = 0xff;

/*
 * Wait for the CODEC to be ready to receive more data.
 */
void codec_wait() {
    while (*CODEC == 0x8000);
}

void init_codec() {
	const uint16_t codec_init_data[] = {
	0x2e00, // Master reset
	0x1a00, // R13 - Turn On Headphones
	0x2a1f, // R21 - Enable All the Analog In
	0x2301, // R17 - Auto-level control: WM8776_R17_ADC_ANALOG_AUTO_LEVEL_CONTROL | LCEN_ENABLED | HLD_MASK,
	0x2c07, // R22 - Enable all output sources
	0x1402, // R10 - DAC interface control
	0x1602, // R11 - ADC inteface control
	0x1845  // R12 - ADC rate 768, DAC rate 512
	};

	for (int i = 0; i<sizeof(codec_init_data)/sizeof(uint16_t); i++) {
		codec_wait();
		*CODEC = codec_init_data[i];
	}

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

    codec_wait();
	*CODEC = 0x0A00 | (0xFF - vol);

	codec_wait();
	*CODEC = 0x0400 | (vol >> 1);
}

/*
 * Return the current volume
 */
unsigned char codec_get_volume() {
    return volume;
}
