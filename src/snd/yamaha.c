/*
 * Master file for the Yamaha sound chips
 */

#include "features.h"
#include "sound_reg.h"
#include "snd/yamaha.h"

/*
 * Write a byte to a Yamaha sound chip register
 *
 * Inputs:
 * dev = the Yamaha device to access SND_OPN2, SND_OPM, SND_OPL3
 * position = whether external (physical chip) or internal (FPGA), and left/right channel
 * port = the number of the register to update
 * value = the byte value to write to the port
 */
void ym_write(enum e_ym_snd_dev dev, enum e_ym_position position, short port, unsigned char value) {
    switch (dev) {
#if HAS_OPN
        case SND_OPN2:
            switch (position) {
                case EXTERNAL:
                    OPN2_EXT_BASE[port] = value;
                    break;

                case INTERNAL:
                    OPN2_INT_BASE[port] = value;
                    break;

                default:
                    break;
            }
            break;
#endif
#if HAS_OPM
        case SND_OPM:
        switch (position) {
            case EXTERNAL:
                OPM_EXT_BASE[port] = value;
                break;

            case INTERNAL:
                OPM_INT_BASE[port] = value;
                break;

            default:
                break;
        }
            break;
#endif
        case SND_OPL3:
            OPL3_PORT[port] = value;
            break;

        default:
            break;
    }
}

/*
 * Initialize and mute all the Yamaha sound chips
 */
void ym_init() {
    short position;
    short port;

#if HAS_OPN
    /* OPN */
    for (position = EXTERNAL; position <= INTERNAL; position++) {
        for (port = 0x40; port <= 0x4F; port++) {
            ym_write(SND_OPN2, position, port, 0x7F);       /* Maximum attentuation */
        }
    }
#endif

#if HAS_OPM
    /* OPM */
    for (position = EXTERNAL; position <= INTERNAL; position++) {
        for (port = 0x60; port <= 0x6F; port++) {
            ym_write(SND_OPM, position, port, 0x7F);        /* Maximum attentuation */
        }
    }
#endif

    /* OPL3 */
    for (port = 0x40; port <= 0x55; port++) {
        ym_write(SND_OPL3, position, port, 0x3F);       /* Maximum attenuation */
    }
}
