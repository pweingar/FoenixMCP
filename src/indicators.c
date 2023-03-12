/*
 * System independent routines for controlling the built in indicator LEDs
 *
 * Indicators on the C256 and most of the A2560 lines of computers will be built in to
 * the board and contolled by GABE. They will usually be simple off-on indicators.
 *
 * On the A2560K, the indicators are RGB LEDs on the keyboard or build into the machine.
 */

#include "gabe_reg.h"
#include "indicators.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "dev/kbd_mo.h"
#endif

#if ( MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X)
short ind_state_color(short state) {
    switch (state) {
        case IND_ON:
            /* Green for on */
#if MODEL == MODEL_FOENIX_A2560K            
            return 0x02;
#elif MODEL == MODEL_FOENIX_GENX
            return 0x05;
#elif MODEL == MODEL_FOENIX_A2560X
            return 0x06;
#else            
            return 0x03;
#endif
        case IND_ERROR:
            /* Red for error */
            return 0x04;

        default:
            /* Anything else is off */
            return 0;
    }
}

void ind_set_power(short state) {
    switch (state) {
        case IND_ON:
            /* Dark green for on */
#if MODEL == MODEL_FOENIX_A2560K
            *RGB_LED = 0x00004000;
#elif MODEL == MODEL_FOENIX_GENX
            *RGB_LED = 0x00200020;
#elif MODEL == MODEL_FOENIX_A2560X
            *RGB_LED = 0x00202000;
#else
            *RGB_LED = 0x00004020;
#endif
            break;

        case IND_ERROR:
            /* Red for error */
            *RGB_LED = 0x00FF0000;
            break;

        default:
            /* Anything else is off */
            *RGB_LED = 0;
            break;
    }
}

#endif 

#if MODEL == MODEL_FOENIX_A2560K
void ind_set_fdc(short state) {
    kbdmo_set_fdc_led(ind_state_color(state));
}

void ind_set_sdc(short state) {
    kbdmo_set_sdc_led(ind_state_color(state));
}

void ind_set_hdc(short state) {
    kbdmo_set_hdc_led(ind_state_color(state));
}
#elif MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
// This is for GEN X Status LED
void ind_set_fdc(short state) {
    genx_set_fdc_led(ind_state_color(state));
}

void ind_set_sdc(short state) {
    genx_set_sdc_led(ind_state_color(state));
}

void ind_set_hdc(short state) {
    genx_set_hdc_led(ind_state_color(state));
}
#else 
void ind_set_fdc(short state) {

}

void ind_set_sdc(short state) {

}

void ind_set_hdc(short state) {

}
#endif

/*
 * Set an indicator to the given state
 *
 * Inputs:
 * ind_number = the number of the indicator to change
 * state = the state the indicator should take (on, off, error)
 */
void ind_set(short ind_number, short state) {
    switch (ind_number) {
#if MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
        case IND_POWER:
            ind_set_power(state);
            break;

        case IND_FDC:
            ind_set_fdc(state);
            break;
#endif
        case IND_SDC:
            ind_set_sdc(state);
            break;

        case IND_HDC:
            ind_set_hdc(state);
            break;

        default:
            break;
    }
}

/*
 * Initialize the indicators
 *
 * Generally, this means the indicators will all be turned off
 */
void ind_init() {
    ind_set(IND_POWER, IND_ON);
    ind_set(IND_FDC, IND_OFF);
    ind_set(IND_SDC, IND_OFF);
    ind_set(IND_HDC, IND_OFF);
}
