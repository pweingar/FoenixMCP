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

/*
 * Test the OPL2
 */
extern short opl2_test(short channel, int argc, char * argv[]);

/*
 * Perform a transmit test on the MIDI ports
 */
extern short midi_tx_test(short channel, int argc, char * argv[]);

/*
 * Perform a receive test on the MIDI ports
 */
extern short midi_rx_test(short channel, int argc, char * argv[]);

/*
 * Perform a loopback test on the MIDI ports
 */
extern short midi_loop_test(short channel, int argc, char * argv[]);

#endif
