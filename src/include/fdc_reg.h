/*
 * Define the locations of all the FDC registers
 */

#ifndef __FDC_REG_H
#define __FDC_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "A2560K/fdc_a2560k.h"
#endif

 /* Read/Write - Digital Output Register */

#define FDC_DOR_DSEL0   0x01    /* Drive 0 Select */
#define FDC_DOR_DSEL1   0x02    /* Drive 1 Select */
#define FDC_DOR_NRESET  0x04    /* Reset the FDC */
#define FDC_DOR_DMAEN   0x08    /* Enable DMA */
#define FDC_DOR_MOT0    0x10    /* Turn on motor 0 */
#define FDC_DOR_MOT1    0x20    /* Turn on motor 1 */
#define FDC_DOR_MOT2    0x40    /* Turn on motor 2 */
#define FDC_DOR_MOT3    0x80    /* Turn on motor 3 */

/* Read - Main Status Register */
#define FDC_MSR_DRV0BSY 0x01    /* Indicates if drive 0 is busy */
#define FDC_MSR_DRV1BSY 0x02    /* Indicates if drive 1 is busy */
#define FDC_MSR_CMDBSY  0x10    /* Indicates if a command is in progress */
#define FDC_MSR_NONDMA  0x20
#define FDC_MSR_DIO     0x40    /* Data direction: 1 = read, 0 = write */
#define FDC_MSR_RQM     0x80    /* 1 = host can transfer data, 0 = host must wait */

/* Write - Data Select Register
 * Bit[0..1] = data rate
 * Bit[2..4] = precompensation select
 */
#define FDC_DSR_LOPWR   0x40    /* Turn on low power mode */
#define FDC_DSR_RESET   0x80    /* Software reset of the FDC */

/* Read - Digital Input Register */
#define FDC_DIR_DSKCHG  0x80    /* Indicates if the disk has changed */

/* Write - Configuration Control Register
 * Bit[0..1] = Data rate
 */

#endif
