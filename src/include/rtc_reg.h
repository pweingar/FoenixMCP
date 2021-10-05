/*
 * Define locations for the registers for the real time clock chip
 */

#ifndef __RTC_REG_H
#define __RTC_REG_H

#define RTC_BASE        ((volatile unsigned char *)0x00C00080)

#define RTC_SEC         ((volatile unsigned char *)0x00C00080)
#define RTC_ALRM_SEC    ((volatile unsigned char *)0x00C00081)
#define RTC_MIN         ((volatile unsigned char *)0x00C00082)
#define RTC_ALRM_MIN    ((volatile unsigned char *)0x00C00083)
#define RTC_HOUR        ((volatile unsigned char *)0x00C00084)
#define RTC_ALRM_HOUR   ((volatile unsigned char *)0x00C00085)
#define RTC_DAY         ((volatile unsigned char *)0x00C00086)
#define RTC_ALRM_DAY    ((volatile unsigned char *)0x00C00087)
#define RTC_DAY_OF_WEEK ((volatile unsigned char *)0x00C00088)
#define RTC_MONTH       ((volatile unsigned char *)0x00C00089)
#define RTC_YEAR        ((volatile unsigned char *)0x00C0008A)
#define RTC_RATES       ((volatile unsigned char *)0x00C0008B)
#define RTC_ENABLES     ((volatile unsigned char *)0x00C0008C)
#define RTC_FLAGS       ((volatile unsigned char *)0x00C0008D)
#define RTC_CTRL        ((volatile unsigned char *)0x00C0008E)
#define RTC_CENTURY     ((volatile unsigned char *)0x00C0008F)

/* Rate fields and settings */
#define RTC_RATES_WD    0xf0
#define RTC_RATES_RS    0x0f
#define RTC_RATE_976us  0x06

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
