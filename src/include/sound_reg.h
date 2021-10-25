/*
 * Master include file for all the sound device registers
 */

#ifndef __SOUND_REG_H
#define __SOUND_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "A2560K/sound_a2560k.h"

#elif MODEL == MODEL_FOENIX_A2560U || MODEL_FOENIX_A2560U_PLUS
#include "A2560U/sound_a2560u.h"
#endif

#endif
