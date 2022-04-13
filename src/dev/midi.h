/*
 * Declarations for the MIDI ports
 */

#ifndef __MIDI_H
#define __MIDI_H

/**
 * Initilialize the MIDI port
 *
 * @return 0 on success, any other number is an error
 */
extern short midi_init();

/**
 * Send a byte to the MIDI port
 *
 * @param b the byte to send
 * @return 0 on success, any other number is an error
 */
extern short midi_put(unsigned char b);

/**
 * Get a byte from the MIDI port
 *
 * @return the byte read (-1 on error)
 */
extern char midi_get_poll();

#endif
