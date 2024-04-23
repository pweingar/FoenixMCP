/*
 * Definitions to access the PATA port on the A2560U and A2560U+
 */

#ifndef __PATA_A2560U_H
#define __PATA_A2560U_H

#define PATA_DATA_16         ((volatile unsigned short *)0x00B00400)
#define PATA_DATA_8          ((volatile unsigned char *)0x00B00400)
#define PATA_ERROR           ((volatile unsigned char *)0x00B00402)
#define PATA_SECT_CNT        ((volatile unsigned char *)0x00B00404)
#define PATA_SECT_SRT        ((volatile unsigned char *)0x00B00406)
#define PATA_CLDR_LO         ((volatile unsigned char *)0x00B00408)
#define PATA_CLDR_HI         ((volatile unsigned char *)0x00B0040A)
#define PATA_HEAD            ((volatile unsigned char *)0x00B0040C)
#define PATA_CMD_STAT        ((volatile unsigned char *)0x00B0040E)

#endif
