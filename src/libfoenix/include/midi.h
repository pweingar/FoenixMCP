/*
 * Declarations for the MIDI ports
 */

#ifndef __MIDI_H
#define __MIDI_H

int16_t midi_init();
int16_t midi_wait_can_read();
int16_t midi_wait_can_write();
int16_t midi_can_read();
int16_t midi_can_write();
void    midi_write(uint8_t b);
uint8_t midi_read();

#endif
