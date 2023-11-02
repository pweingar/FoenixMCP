/**
 * @file dma_c256.h
 * @brief Definitions of the video and system DMA registers
 * @version 0.1
 * @date 2023-10-02
 * 
 */

#ifndef __DMA_C256_H_
#define __DMA_C256_H_

#define VDMA_CTRL			((volatile __attribute__((far)) uint8_t *)0xaf0400)
#define VDMA_CTRL_EN		0x01
#define VDMA_CTRL_2D		0x02
#define VDMA_CTRL_FILL		0x04
#define VDMA_CTRL_IEN		0x08	// Interrupt Enable
#define VDMA_CTRL_SRC_SRAM	0x10
#define VDMA_CTRL_DST_SRAM	0x20
#define VDMA_CTRL_TRF		0x80

#define VDMA_STAT			((volatile __attribute__((far)) uint8_t *)0xaf0401)
#define VDMA_STAT_ERR_SIZE	0x01
#define VDMA_STAT_ERR_DST	0x02
#define VDMA_STAT_ERR_SRC	0x04
#define VDMA_STAT_TFR_BUSY	0x80

#define VDMA_FILL_VALUE		((volatile __attribute__((far)) uint8_t *)0xaf0401)
#define VDMA_SRC_ADDR		((volatile __attribute__((far)) uint8_t *)0xaf0402)
#define VDMA_DST_ADDR		((volatile __attribute__((far)) uint8_t *)0xaf0405)
#define VDMA_SIZE			((volatile __attribute__((far)) uint8_t *)0xaf0408)
#define VDMA_SIZE_X			((volatile __attribute__((far)) uint16_t *)0xaf0408)
#define VDMA_SIZE_Y			((volatile __attribute__((far)) uint16_t *)0xaf040a)
#define VDMA_SRC_STRIDE		((volatile __attribute__((far)) uint16_t *)0xaf040c)
#define VDMA_DST_STRIDE		((volatile __attribute__((far)) uint16_t *)0xaf040e)

#endif
