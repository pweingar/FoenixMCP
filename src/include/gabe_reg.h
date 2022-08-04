/**
 * @file gabe_reg.h
 *
 * Define the various GABE control registers
 */

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "A2560K/gabe_a2560k.h"

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/gabe_a2560u.h"

#elif MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256UPLUS
#include "C256/gabe_c256.h"
#endif
