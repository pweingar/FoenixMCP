/**
 * @brief Definitions for functions to manage DMA transfers
 * 
 */

#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>

/**
 * @brief Fill video memory using a linear VDMA operation
 * 
 * @param dest the address of memory to fill (must be in video RAM)
 * @param value the value to put into memory
 * @param count the number of bytes to fill
 */
extern void vdma_fill_linear(uint8_t * dest, uint8_t value, long count);

#endif
