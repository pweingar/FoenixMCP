/*
 * System independent routines for controlling the built in indicator LEDs
 *
 * Indicators on the C256 and most of the A2560 lines of computers will be built in to
 * the board and contolled by GABE. They will usually be simple off-on indicators.
 *
 * On the A2560K, the indicators are RGB LEDs on the keyboard or build into the machine.
 */

#ifndef __INDICATORS_H
#define __INDICATORS_H

#define IND_OFF             0x00        /* The LED should be off */
#define IND_ON              0x01        /* The LED should be on for a normal condition */
#define IND_ERROR           0x02        /* The LED should be on for an error condition (if possible) */

#define IND_POWER           0x00        /* Number for the power LED */
#define IND_SDC             0x01        /* Number for the SD Card's LED */
#define IND_HDC             0x02        /* Number for the hard drive (IDE) LED */
#define IND_FDC             0x03        /* Number for the flopy drive LED */

/*
 * Initialize the indicators
 *
 * Generally, this means the indicators will all be turned off
 */
extern void ind_init();

/*
 * Set an indicator to the given state
 *
 * Inputs:
 * ind_number = the number of the indicator to change
 * state = the state the indicator should take (on, off, error)
 */
extern void ind_set(short ind_number, short state);

#endif
