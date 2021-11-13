/*
 * Declarations for the MIDI ports
 */

#ifndef __MIDI_H
#define __MIDI_H

/*
 * Initilialize the MIDI port
 */
extern short midi_init();

/*
 * Send a byte to the MIDI port
 *
 * Inputs:
 * b = the byte to send
 */
extern short midi_put(unsigned char b);

/*
 * Get a byte from the MIDI port
 *
 * Returns:
 * b = the byte to send
 */
extern unsigned char midi_get_poll();

#endif
