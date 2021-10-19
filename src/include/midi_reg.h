/*
 * Definitions for the SuperIO MIDI ports
 */

#ifndef __MIDI_REG_H
#define __MIDI_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#define MIDI_DATA   ((volatile unsigned char *)0x00C02330)
#define MIDI_STAT   ((volatile unsigned char *)0x00C02331)
#define MIDI_CMD    ((volatile unsigned char *)0x00C02331)

#define MIDI_STAT_TX_BUSY   0x80
#define MIDI_STAT_RX_EMPTY  0x40
#endif

#endif
