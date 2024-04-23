/**
 * @file timers_reg.h
 *
 * Load register definitions for the various timers on the Foenix
 */

#ifndef __TIMERS_REG_H
#define __TIMERS_REG_H

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
#include "A2560K/timers_a2560k.h"

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/timers_a2560u.h"

#elif MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS
#include "C256/timers_c256.h"
#endif

#endif
