/*
 * Definitions for access to the SDC controller for the A2560U and A2560U+
 */

#ifndef __SDC_A2560U_H
#define __SDC_A2560U_H

#define GABE_SDC_REG            ((volatile unsigned short *)0x00B0051A)
#define GABE_SDC_PRESENT        0x0010      /* Is an SD card present? --- 0:Yes, 1:No */
#define GABE_SDC_WPROT          0x0020      /* Is the SD card write protected? --- 0:Yes, 1:No */

#define SDC_VERSION_REG         ((unsigned char *)0x00B00300)
#define SDC_CONTROL_REG         ((unsigned char *)0x00B00301)
#define SDC_TRANS_TYPE_REG      ((unsigned char *)0x00B00302)

#define SDC_TRANS_CONTROL_REG   ((unsigned char *)0x00B00303)
#define SDC_TRANS_STATUS_REG    ((unsigned char *)0x00B00304)
#define SDC_TRANS_ERROR_REG     ((unsigned char *)0x00B00305)
#define SDC_DIRECT_ACCESS_REG   ((unsigned char *)0x00B00306)
#define SDC_SD_ADDR_7_0_REG     ((unsigned char *)0x00B00307)
#define SDC_SD_ADDR_15_8_REG    ((unsigned char *)0x00B00308)
#define SDC_SD_ADDR_23_16_REG   ((unsigned char *)0x00B00309)
#define SDC_SD_ADDR_31_24_REG   ((unsigned char *)0x00B0030A)

#define SDC_SPI_CLK_DEL_REG     ((unsigned char *)0x00B0030B)

#define SDC_RX_FIFO_DATA_REG    ((unsigned char *)0x00B00310)
#define SDC_RX_FIFO_DATA_CNT_HI ((unsigned char *)0x00B00312)
#define SDC_RX_FIFO_DATA_CNT_LO ((unsigned char *)0x00B00313)
#define SDC_RX_FIFO_CTRL_REG    ((unsigned char *)0x00B00314)

#define SDC_TX_FIFO_DATA_REG    ((unsigned char *)0x00B00320)
#define SDC_TX_FIFO_CTRL_REG    ((unsigned char *)0x00B00324)

#endif
