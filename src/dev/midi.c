/*
 * Definitions for the MIDI ports
 */

#include "midi_reg.h"
#include "dev/midi.h"
#include "simpleio.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K

/*
 * Return true if there is data waiting to be read
 */
short midi_input_not_ready() {
    return (*MIDI_STAT & MIDI_STAT_RX_EMPTY);
}

/*
 * Return true if there is data waiting to be read
 */
short midi_output_busy() {
    return (*MIDI_STAT & MIDI_STAT_RX_EMPTY);
}


/*
 * Initilialize the MIDI port
 */
short midi_init() {
    unsigned char dummy;

    while (midi_output_busy()) ;

    *MIDI_CMD = 0xFF;       /* Reset the MIDI port */

    /* Wait for the ACK */
    for (dummy = 0; dummy != 0xFE; ) {
        while (midi_input_not_ready()) ;
        dummy = *MIDI_DATA;
    }

    while (midi_output_busy()) ;

    *MIDI_CMD = 0x3F;       /* Switch the MIDI port into UART mode */

    /* Wait for the ACK */
    for (dummy = 0; dummy != 0xFE; ) {
        while (midi_input_not_ready()) ;
        dummy = *MIDI_DATA;
    }

    return 0;
}

/*
 * Send a byte to the MIDI port
 *
 * Inputs:
 * b = the byte to send
 */
short midi_put(unsigned char b) {
    while (midi_output_busy()) ;

    /* Send the byte */
    *MIDI_DATA = b;

    return 0;
}

/*
 * Get a byte from the MIDI port
 *
 * Returns:
 * b = the byte to send
 */
unsigned char midi_get_poll() {
    while (midi_input_not_ready()) ;
    return *MIDI_DATA;
}

#endif
