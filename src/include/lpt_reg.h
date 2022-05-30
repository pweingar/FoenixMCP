/**
 * @file lpt_reg.h
 * Define the registers for the parallel port
 */

#ifndef __LPT_REG_H
#define __LPT_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#define LPT_DATA_PORT   ((volatile unsigned char *)0xFEC02378)
#define LPT_STAT_PORT   ((volatile unsigned char *)0xFEC02379)
#define LPT_CTRL_PORT   ((volatile unsigned char *)0xFEC0237A)
#endif

#define LPT_STAT_nBUSY  0x80
#define LPT_STAT_nACK   0x40
#define LPT_STAT_PO     0x20
#define LPT_STAT_SELECT 0x10
#define LPT_STAT_nERROR 0x08
#define LPT_STAT_IRQ    0x04

#define LPT_CTRL_STROBE 0x01
#define LPT_CTRL_AL     0x02
#define LPT_CTRL_mINIT  0x04
#define LPT_CTRL_SELECT 0x08
#define LPT_CTRL_IRQE   0x10
#define LPT_CTRL_BI     0x20

#define LPT_INIT_ON     0x04            /* Start the printer initialization process */
#define LPT_INIT_OFF    0x0C            /* Stop the printer initialization process */
#define LPT_STROBE_ON   0x0D            /* Strobe the printer */
#define LPT_STROBE_OFF  0x0C            /* Drop the strobe to the printer */

#endif
