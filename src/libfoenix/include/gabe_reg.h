/**
 * @file gabe_reg.h
 *
 * Define the various GABE control registers
 */

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K    // The Trinity Registers are 32bits in the A2560K
#include "A2560K/gabe_a2560k.h"

#elif MODEL == MODEL_FOENIX_A2560X || MODEL == MODEL_FOENIX_GENX    // The Trinity Registers are 16bits wide in the GenX/A2560X
#include "A2560X/gabe_a2560x.h"

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/gabe_a2560u.h"

#elif MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS
#include "C256/gabe_c256.h"

#endif
