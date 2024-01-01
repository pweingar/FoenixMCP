/*
 * Definitions for access to the SDC controller
 */

#ifndef __SDC_C256_H
#define __SDC_C256_H

#include <stdint.h>

#define GABE_SDC_REG            ((volatile uint8_t *)0xafe812)
#define GABE_SDC_PRESENT        0x01		/* Is an SD card present? --- 0:Yes, 1:No */
#define GABE_SDC_WPROT          0x02		/* Is the SD card write protected? --- 0:Yes, 1:No */

#define SDC_VERSION_REG         ((volatile uint8_t *)0xafea00)
#define SDC_CONTROL_REG         ((volatile uint8_t *)0xafea01)
#define SDC_TRANS_TYPE_REG      ((volatile uint8_t *)0xafea02)

#define SDC_TRANS_CONTROL_REG   ((volatile uint8_t *)0xafea03)
#define SDC_TRANS_STATUS_REG    ((volatile uint8_t *)0xafea04)
#define SDC_TRANS_ERROR_REG     ((volatile uint8_t *)0xafea05)
#define SDC_DIRECT_ACCESS_REG   ((volatile uint8_t *)0xafea06)
#define SDC_SD_ADDR_7_0_REG     ((volatile uint8_t *)0xafea07)
#define SDC_SD_ADDR_15_8_REG    ((volatile uint8_t *)0xafea08)
#define SDC_SD_ADDR_23_16_REG   ((volatile uint8_t *)0xafea09)
#define SDC_SD_ADDR_31_24_REG   ((volatile uint8_t *)0xafea0A)

#define SDC_SPI_CLK_DEL_REG     ((volatile uint8_t *)0xafea0B)

#define SDC_RX_FIFO_DATA_REG    ((volatile uint8_t *)0xafea10)
#define SDC_RX_FIFO_DATA_CNT_HI ((volatile uint8_t *)0xafea12)
#define SDC_RX_FIFO_DATA_CNT_LO ((volatile uint8_t *)0xafea13)
#define SDC_RX_FIFO_CTRL_REG    ((volatile uint8_t *)0xafea14)

#define SDC_TX_FIFO_DATA_REG    ((volatile uint8_t *)0xafea20)
#define SDC_TX_FIFO_CTRL_REG    ((volatile uint8_t *)0xafea24)

#endif
