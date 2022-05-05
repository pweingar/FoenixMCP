/*
 * Include file for the basic set of VICKY registers
 */

#ifndef __vicky_general_h
#define __vicky_general_h

#include "types.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX
#include "A2560K/VICKYIII_a2560k.h"
#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/VICKYIII_a2560u.h"
#endif


#endif
