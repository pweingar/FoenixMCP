/*
 * Definitions for the MIDI ports
 */

#include "midi_reg.h"
#include "dev/midi.h"

/*
 * Initilialize the MIDI port
 */
short midi_init() {
    *MIDI_CMD = 0xff;       /* Send the command to reset the MIDI port */

    // while ((*MIDI_STAT & MIDI_STAT_RX_EMPTY) == 0) {
    //     /* While there is data, throw it out */
    //     unsigned char dummy = *MIDI_DATA;
    // }

    *MIDI_CMD = 0x3F;       /* Switch the MIDI port into UART mode */

    // while ((*MIDI_STAT & MIDI_STAT_RX_EMPTY) == 0) {
    //     /* While there is data, throw it out */
    //     unsigned char dummy = *MIDI_DATA;
    // }

    return 0;
}

/*
 * Send a byte to the MIDI port
 *
 * Inputs:
 * b = the byte to send
 */
short midi_put(unsigned char b) {
    while ((*MIDI_STAT & MIDI_STAT_TX_BUSY) != 0) {
        /* Wait until the MIDI transmitter is not busy */
        ;
    }

    /* Send the byte */
    *MIDI_DATA = b;

    while ((*MIDI_STAT & MIDI_STAT_TX_BUSY) != 0) {
        /* Wait until the MIDI transmitter is not busy */
        ;
    }

    return 0;
}

/*
 * Get a byte from the MIDI port
 *
 * Returns:
 * b = the byte to send
 */
short midi_get_poll() {
    if (*MIDI_STAT & MIDI_STAT_RX_EMPTY) {
        /* There's no data... return 0 */
        return 0;
    } else {
        /* Get and return the byte */
        return (short)*MIDI_DATA;
    }
}
