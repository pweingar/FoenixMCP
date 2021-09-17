/*
 * Declarations for the CODEC
 */

#ifndef __CODEC_H
#define __CODEC_H

#define CODEC ((volatile unsigned short *)0x00C20E00)

/*
 * Initialize the CODEC registers
 */
extern void init_codec();

#endif
