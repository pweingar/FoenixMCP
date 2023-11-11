#ifndef _FEATURES_H
#define _FEATURES_H

#include "sys_general.h"

// This files, defines, for each Foenix model, the available features.
// This drives compilation switches to include or drivers, tests in the CLI, etc.

#if MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X || MODEL == MODEL_FOENIX_A2560K
    #define HAS_SUPERIO 1

    // Actually, for the X this depends on the submodel. E.g. Pizza Box doesn't have floppy.
    #define HAS_FLOPPY 1

    // Actually, for the X this depends on the submodel. E.g. Pizza Box doesn't have floppy.
    #define HAS_PARALLEL_PORT 1

    #define HAS_DUAL_SCREEN 1

    #define HAS_MIDI_PORTS 1
    #define HAS_EXTERNAL_SIDS 1

    // Sounchips
    #define HAS_OPN 1
    #define HAS_OPM 1
    #define HAS_SNES_GAMEPAD 1
    #define HAS_OPL3 1
#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
    #define HAS_OPL3 1
#elif MODEL == MODEL_FOENIX_C256U_PLUS || MODEL == MODEL_FOENIX_C256U
    #define HAS_OPL3 1
#elif MODEL == MODEL_FOENIX_C256U_FMX
    #define HAS_SUPERIO 1
    #define HAS_PARALLEL_PORT 1
    #define HAS_MIDI_PORTS 1
    #define HAS_EXTERNAL_SIDS 1
    #define HAS_OPL3 1
#endif


/* Defaults -----------------------------------------------------------------*/

#ifndef HAS_SUPERIO // The Super I/O controller chip for PS/2 serial/parallel/floppy etc.
    #define HAS_SUPERIO 0
#endif

#ifndef HAS_FLOPPY // Floppy disk drive presence
    #define HAS_FLOPPY 0
#endif

#ifndef HAS_PARALLEL_PORT // Parallel/Centronics port presence
    #define HAS_PARALLEL_PORT 0
#endif

#ifndef HAS_MIDI_PORTS // Midi IN/OUT
    #define HAS_MIDI_PORTS 0
#endif

#ifndef HAS_DUAL_SCREEN // Computer has 2 screen ports
    #define HAS_DUAL_SCREEN 0
#endif

#ifndef HAS_OPN // OPN soundchip presence
    #define HAS_OPN 0
#endif

#ifndef HAS_OPM // OPM soundchip presence
    #define HAS_OPM 0
#endif

#ifndef HAS_OPL3 // YMF262 OPL3 soundchip presence: all Foenix computer have it... we could remove that switch altogether.
    #define HAS_OPL3 1
#endif

#ifndef HAS_SNES_GAMEPAD // Super Nintendo gamepad connectors (even if through adapter)
    #define HAS_SNES_GAMEPAD 0
#endif

#endif