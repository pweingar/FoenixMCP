/*
 * Definitions to access the PATA port on the C256
 */

#ifndef __PATA_C256_H
#define __PATA_C256_H

#include <stdint.h>

#define PATA_DATA_16         ((volatile uint16_t *)0xAFE838)
#define PATA_DATA_8          ((volatile uint8_t *)0xAFE830)
#define PATA_ERROR           ((volatile uint8_t *)0xAFE831)
#define PATA_SECT_CNT        ((volatile uint8_t *)0xAFE832)
#define PATA_SECT_SRT        ((volatile uint8_t *)0xAFE833)
#define PATA_CLDR_LO         ((volatile uint8_t *)0xAFE834)
#define PATA_CLDR_HI         ((volatile uint8_t *)0xAFE835)
#define PATA_HEAD            ((volatile uint8_t *)0xAFE836)
#define PATA_CMD_STAT        ((volatile uint8_t *)0xAFE837)

#endif
