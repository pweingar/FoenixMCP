/*
 * Commands to work with the sound devices
 */

#ifndef __SOUND_CMDS_H
#define __SOUND_CMDS_H

/*
 * Play a sound on the PSG
 */
extern short psg_test(short channel, int argc, char * argv[]);

/*
 * Play a sound on the OPL3
 */
extern short opl3_test(short channel, int argc, char * argv[]);

#endif
