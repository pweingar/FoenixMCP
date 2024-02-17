/*
 * Gather and return information about the system
 */

#ifndef __SYS_GENERAL_H
#define __SYS_GENERAL_H

#include "constants.h"
#include "features.h"
#include "sys_types.h"
#include "version.h"

/* IDs for the various Foenix machines supported */

#define MODEL_FOENIX_FMX            0
#define MODEL_FOENIX_C256U          1
#define MODEL_FOENIX_GENX           4
#define MODEL_FOENIX_C256U_PLUS     5
#define MODEL_FOENIX_A2560U_PLUS    6
#define MODEL_FOENIX_A2560X         8
#define MODEL_FOENIX_A2560U         9
#define MODEL_FOENIX_A2560K         11
#define MODEL_FOENIX_A2560M         12

/* IDs for the CPUs supported */

#define CPU_WDC65816                0xFF  /* CPU code for the Western Design Center 65816 */
#define CPU_M68000                  0x00  /* Motorola 680X0 chips... */
#define CPU_M68020                  0x01
#define CPU_M68EC020                0x02
#define CPU_M68030                  0x03
#define CPU_M680EC30                0x04
#define CPU_M68040                  0x05
#define CPU_M68040V                 0x06
#define CPU_M680EC40                0x07
#define CPU_486DX2_50               0x08  /* Intel 486 chips... */
#define CPU_486DX2_60               0x09
#define CPU_486DX4                  0x0A
#define CPU_MC68EC060               0x0B  /* New things Coming... */

/* Clock speeds */

#define SYSCLK_14MHZ                0x00    /* 14.318 MHz */
#define SYSCLK_20MHZ                0x01    /* 20 MHz */
#define SYSCLK_25MHZ                0x02    /* 25 MHz */
#define SYSCLK_33MHZ                0x03    /* 33 MHz */
#define SYSCLK_40MHZ                0x04    /* 40 MHz */
#define SYSCLK_50MHZ                0x05    /* 50 MHz */
#define SYSCLK_66MHZ                0x06    /* 66 MHz */
#define SYSCLK_80MHZ                0x07    /* 88 MHz */

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
extern void sys_get_information(p_sys_info info);

#if MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
/*
 * Set the color of the LED for the floppy drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void genx_set_fdc_led(short colors);

/*
 * Set the color of the LED for the SD card slot
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void genx_set_sdc_led(short colors);

/*
 * Set the color of the LED for the IDE hard drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
extern void genx_set_hdc_led(short colors);
#endif

#endif
