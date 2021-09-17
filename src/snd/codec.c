/*
 * Definitions of functions to support the CODEC
 */

#include "snd/codec.h"

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
}
