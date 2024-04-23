/*
 * Definitions for the SuperIO MIDI ports
 */

#ifndef __MIDI_REG_H
#define __MIDI_REG_H

#include <stdint.h>

#include "hardware_ids.h"

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
#define MIDI_DATA   ((volatile uint8_t * const)0xFEC02330)
#define MIDI_STAT   ((volatile uint8_t * const)0xFEC02331)
#define MIDI_CMD    ((volatile uint8_t * const)0xFEC02331)

#define MIDI_STAT_TX_BUSY   0x40
#define MIDI_STAT_RX_EMPTY  0x80
#endif

#endif
