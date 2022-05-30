/*
 * Master include file for the Yamaha sound chips
 */

#ifndef __YAMAHA_H
#define __YAMAHA_H

/*
 * The Yamaha sound chip protocol to access
 */
enum e_ym_snd_dev {
    SND_OPN2 = 0,
    SND_OPM,
    SND_OPL3
};

/*
 * The particular implementation to access
 */
enum e_ym_position {
    EXTERNAL,               /* A physical Yamaha chip installed on the board */
    INTERNAL,               /* FPGA implementation */
};

/*
 * Write a byte to a Yamaha sound chip register
 *
 * Inputs:
 * dev = the Yamaha device to access SND_OPN2, SND_OPM, SND_OPL3
 * position = whether external (physical chip) or internal (FPGA)
 * port = the number of the register to update
 * value = the byte value to write to the port
 */
extern void ym_write(enum e_ym_snd_dev dev, enum e_ym_position position, short port, unsigned char value);

/*
 * Initialize and mute all the Yamaha sound chips
 */
extern void ym_init();

#endif
