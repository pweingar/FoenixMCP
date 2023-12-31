/*
 * Define locations for the registers for the real time clock chip
 */

#ifndef __RTC_REG_H
#define __RTC_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X

#define RTC_BASE        ((volatile unsigned char *)0xFEC00080)
#define RTC_SEC         ((volatile unsigned char *)0xFEC00080)
#define RTC_ALRM_SEC    ((volatile unsigned char *)0xFEC00081)
#define RTC_MIN         ((volatile unsigned char *)0xFEC00082)
#define RTC_ALRM_MIN    ((volatile unsigned char *)0xFEC00083)
#define RTC_HOUR        ((volatile unsigned char *)0xFEC00084)
#define RTC_ALRM_HOUR   ((volatile unsigned char *)0xFEC00085)
#define RTC_DAY         ((volatile unsigned char *)0xFEC00086)
#define RTC_ALRM_DAY    ((volatile unsigned char *)0xFEC00087)
#define RTC_DAY_OF_WEEK ((volatile unsigned char *)0xFEC00088)
#define RTC_MONTH       ((volatile unsigned char *)0xFEC00089)
#define RTC_YEAR        ((volatile unsigned char *)0xFEC0008A)
#define RTC_RATES       ((volatile unsigned char *)0xFEC0008B)
#define RTC_ENABLES     ((volatile unsigned char *)0xFEC0008C)
#define RTC_FLAGS       ((volatile unsigned char *)0xFEC0008D)
#define RTC_CTRL        ((volatile unsigned char *)0xFEC0008E)
#define RTC_CENTURY     ((volatile unsigned char *)0xFEC0008F)

#elif MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS

#define RTC_BASE        ((volatile unsigned char *)0x00B00080)
#define RTC_SEC         ((volatile unsigned char *)0x00B00080)
#define RTC_ALRM_SEC    ((volatile unsigned char *)0x00B00082)
#define RTC_MIN         ((volatile unsigned char *)0x00B00084)
#define RTC_ALRM_MIN    ((volatile unsigned char *)0x00B00086)
#define RTC_HOUR        ((volatile unsigned char *)0x00B00088)
#define RTC_ALRM_HOUR   ((volatile unsigned char *)0x00B0008A)
#define RTC_DAY         ((volatile unsigned char *)0x00B0008C)
#define RTC_ALRM_DAY    ((volatile unsigned char *)0x00B0008E)
#define RTC_DAY_OF_WEEK ((volatile unsigned char *)0x00B00090)
#define RTC_MONTH       ((volatile unsigned char *)0x00B00092)
#define RTC_YEAR        ((volatile unsigned char *)0x00B00094)
#define RTC_RATES       ((volatile unsigned char *)0x00B00096)
#define RTC_ENABLES     ((volatile unsigned char *)0x00B00098)
#define RTC_FLAGS       ((volatile unsigned char *)0x00B0009A)
#define RTC_CTRL        ((volatile unsigned char *)0x00B0009C)
#define RTC_CENTURY     ((volatile unsigned char *)0x00B0009E)

#elif MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS

#define RTC_BASE        ((volatile unsigned char *)0xaf0800)
#define RTC_SEC         ((volatile unsigned char *)0xaf0800)
#define RTC_ALRM_SEC    ((volatile unsigned char *)0xaf0801)
#define RTC_MIN         ((volatile unsigned char *)0xaf0802)
#define RTC_ALRM_MIN    ((volatile unsigned char *)0xaf0803)
#define RTC_HOUR        ((volatile unsigned char *)0xaf0804)
#define RTC_ALRM_HOUR   ((volatile unsigned char *)0xaf0805)
#define RTC_DAY         ((volatile unsigned char *)0xaf0806)
#define RTC_ALRM_DAY    ((volatile unsigned char *)0xaf0807)
#define RTC_DAY_OF_WEEK ((volatile unsigned char *)0xaf0808)
#define RTC_MONTH       ((volatile unsigned char *)0xaf0809)
#define RTC_YEAR        ((volatile unsigned char *)0xaf080a)
#define RTC_RATES       ((volatile unsigned char *)0xaf080b)
#define RTC_ENABLES     ((volatile unsigned char *)0xaf080c)
#define RTC_FLAGS       ((volatile unsigned char *)0xaf080d)
#define RTC_CTRL        ((volatile unsigned char *)0xaf080e)
#define RTC_CENTURY     ((volatile unsigned char *)0xaf080f)

#endif

/* Rate fields and settings */
#define RTC_RATES_WD    0xf0
#define RTC_RATES_RS    0x0f
#define RTC_RATE_976us  0x06
#define RTC_RATE_4ms    0x08
#define RTC_RATE_15ms   0x0A
#define RTC_RATE_500ms  0x0F

/* Enable bits */
#define RTC_AIE         0x08
#define RTC_PIE         0x04
#define RTC_PWRIE       0x02
#define RTC_ABE         0x01

/* Flag bits */
#define RTC_AF          0x08
#define RTC_PF          0x04
#define RTC_PWRF        0x02
#define RTC_BVF         0x01

/* Control bits */
#define RTC_UTI         0x08
#define RTC_STOP        0x04
#define RTC_2412        0x02
#define RTC_DSE         0x01

#endif
