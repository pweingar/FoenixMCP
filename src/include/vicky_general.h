/*
 * Include file for the basic set of VICKY registers
 */

#ifndef __vicky_general_h
#define __vicky_general_h

#include "types.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "A2560K/VICKYIII_a2560k.h"
#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/VICKYIII_a2560u.h"
#elif MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS
#include "C256/vickyii_c256.h"
#endif

#endif
