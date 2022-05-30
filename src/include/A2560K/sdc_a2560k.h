/*
 * Definitions for access to the SDC controller
 */

#ifndef __SDC_A2560K_H
#define __SDC_A2560K_H

#define GABE_SDC_REG            ((volatile unsigned int *)0xFEC00518)
#define GABE_SDC_PRESENT        0x01000000      /* Is an SD card present? --- 0:Yes, 1:No */
#define GABE_SDC_WPROT          0x02000000      /* Is the SD card write protected? --- 0:Yes, 1:No */
#define GABE_BOOTMODE_0         0x00000001      /* DIP switch: boot mode 0 */
#define GABE_BOOTMODE_1         0x00000002      /* DIP switch: boot mode 1 */
#define GABE_DIP_USER_0         0x00000100      /* DIP switch: User 0 */
#define GABE_DIP_USER_1         0x00000200      /* DIP switch: User 1 */

#define SDC_VERSION_REG         ((unsigned char *)0xFEC00300)
#define SDC_CONTROL_REG         ((unsigned char *)0xFEC00301)
#define SDC_TRANS_TYPE_REG      ((unsigned char *)0xFEC00302)

#define SDC_TRANS_CONTROL_REG   ((unsigned char *)0xFEC00303)
#define SDC_TRANS_STATUS_REG    ((unsigned char *)0xFEC00304)
#define SDC_TRANS_ERROR_REG     ((unsigned char *)0xFEC00305)
#define SDC_DIRECT_ACCESS_REG   ((unsigned char *)0xFEC00306)
#define SDC_SD_ADDR_7_0_REG     ((unsigned char *)0xFEC00307)
#define SDC_SD_ADDR_15_8_REG    ((unsigned char *)0xFEC00308)
#define SDC_SD_ADDR_23_16_REG   ((unsigned char *)0xFEC00309)
#define SDC_SD_ADDR_31_24_REG   ((unsigned char *)0xFEC0030A)

#define SDC_SPI_CLK_DEL_REG     ((unsigned char *)0xFEC0030B)

#define SDC_RX_FIFO_DATA_REG    ((unsigned char *)0xFEC00310)
#define SDC_RX_FIFO_DATA_CNT_HI ((unsigned char *)0xFEC00312)
#define SDC_RX_FIFO_DATA_CNT_LO ((unsigned char *)0xFEC00313)
#define SDC_RX_FIFO_CTRL_REG    ((unsigned char *)0xFEC00314)

#define SDC_TX_FIFO_DATA_REG    ((unsigned char *)0xFEC00320)
#define SDC_TX_FIFO_CTRL_REG    ((unsigned char *)0xFEC00324)

#endif
