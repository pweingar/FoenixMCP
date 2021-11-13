/*
 * Include file to access the SDC port on the Foenix machine
 */

#ifndef __SDC_REG_H
#define __SDC_REG_H

#include "types.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "A2560K/sdc_a2560k.h"
#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "A2560U/sdc_a2560u.h"
#endif

/*
 * Bit definitions
 */

// SDC_TRANS_TYPE_REG
#define SDC_TRANS_DIRECT      0x00   // 00 = Direct Access
#define SDC_TRANS_INIT_SD     0x01   // 01 = Init SD
#define SDC_TRANS_READ_BLK    0x02   // 10 = RW_READ_BLOCK (512 Bytes)
#define SDC_TRANS_WRITE_BLK   0x03   // 11 = RW_WRITE_SD_BLOCK

// SDC_TRANS_CONTROL_REG
#define SDC_TRANS_START       0x01
// SDC_TRANS_STATUS_REG
#define SDC_TRANS_BUSY        0x01   //  1= Transaction Busy
// SDC_TRANS_ERROR_REG
#define SDC_TRANS_INIT_NO_ERR   0x00   // Init Error Report [1:0]
#define SDC_TRANS_INIT_CMD0_ERR 0x01
#define SDC_TRANS_INIT_CMD1_ERR 0x02

#define SDC_TRANS_RD_NO_ERR     0x00   // Read Error Report [3:2]
#define SDC_TRANS_RD_CMD_ERR    0x04
#define SDC_TRANS_RD_TOKEN_ERR  0x08

#define SDC_TRANS_WR_NO_ERR     0x00   // Write Report Error  [5:4]
#define SDC_TRANS_WR_CMD_ERR    0x10
#define SDC_TRANS_WR_DATA_ERR   0x20
#define SDC_TRANS_WR_BUSY_ERR   0x30

#endif
