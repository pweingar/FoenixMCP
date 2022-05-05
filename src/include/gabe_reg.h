/**
 * @file gabe_reg.h
 *
 * Define the various GABE control registers
 */

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX
#include "A2560K/gabe_a2560k.h"

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/gabe_a2560u.h"
#endif
