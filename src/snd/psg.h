/*
 * Declarations for the SN76489 PSG
 */

#ifndef __PSG_H
#define __PSG_H

#define PSG_PORT    ((volatile unsigned char *)0x00C20100)  /* Control register for the SN76489 */

/*
 * Mute all voices on the PSG
 */
extern void psg_mute_all();

#endif
