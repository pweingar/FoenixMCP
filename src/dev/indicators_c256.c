/**
 * @file indicators_c256.c
 * @brief Indicator control logic for the C256 line
 * @version 0.1
 * @date 2023-08-30
 * 
 * Indicators on the C256 include the SD card LED and the power LED. They can be on or off.
 * An error state will equate to being off. This code also sets the two status LEDs to their
 * default blinking state.
 * 
 */

#include "indicators.h"
#include "gabe_reg.h"

/*
 * Set an indicator to the given state
 *
 * Inputs:
 * ind_number = the number of the indicator to change
 * state = the state the indicator should take (on, off, error)
 */
void ind_set(short ind_number, short state) {
	uint8_t bit = 0;

	// Figure out which bit to manipulate
	switch (ind_number) {
		case IND_POWER:
			bit = GABE_CTRL_PWR_LED;
			break;

		case IND_SDC:
			bit = GABE_CTRL_SDC_LED;
			break;

		default:
			// Any other indicator is ignored
			return;			
	}

	if (state == IND_ON) {
		// Flip the bit on
		*GABE_CTRL_REG |= bit;

	} else {
		// Flip the bit off
		*GABE_CTRL_REG &= ~bit;
	}
}

/*
 * Initialize the indicators
 *
 * Generally, this means the indicators will all be turned off
 */
void ind_init() {
	*GABE_CTRL_REG = 0;
	*GABE_LED_FLASH_CTRL = 0x03;

	ind_set(IND_POWER, IND_ON);
	ind_set(IND_SDC, IND_OFF);
}
