/**
 * @file exp_reg.h
 * @brief Define the registers to access the expansion port
 * @version 0.1
 * @date 2023-08-30
 * 
 */

#ifndef __EXP_REG_H_
#define __EXP_REG_H_

#if MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS
#include "C256/exp_c256.h"
#endif

#endif