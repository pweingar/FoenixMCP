/**
 * @file bitmap.c
 * @author your name (you@domain.com)
 * @brief Simple bitmap management code
 * @version 0.1
 * @date 2023-10-02
 * 
 */

#include <stdint.h>

#include "bitmap.h"
#include "dma.h"
#include "vicky_general.h"

#define NUM_BITMAP_PLANES 2

static uint8_t bm_visible = 0;		// Track whether a given bitmap plane is visible

/**
 * @brief Set the visibility of a given bitmap plane, and the CLUT to use
 * 
 * @param plane the number of the bitmap to update
 * @param clut the number of the CLUT to use for the bitmap
 * @param is_visible 0 to hide the bitmap, any other number to show it
 */
void bm_set_visibility(short plane, short clut, short is_visible) {
	uint8_t new_control_value = (clut & 0x03) << 1 | (is_visible) ? 1 : 0;

	switch(plane) {
		case 0:
			*bm0_control = new_control_value;
			if (is_visible) {
				bm_visible |= 0x01;
			} else {
				bm_visible &= ~0x01;
			}
			break;

		case 1:
			*bm1_control = new_control_value;
			if (is_visible) {
				bm_visible |= 0x02;
			} else {
				bm_visible &= ~0x02;
			}
			break;

		default:
			break;
	}

	if (bm_visible) {
		tvky_mstr_ctrl->raw = tvky_mstr_ctrl->raw | VKY_MCR_TEXT_OVERLAY | VKY_MCR_GRAPHICS | VKY_MCR_BITMAP;
	} else {
		tvky_mstr_ctrl->raw = tvky_mstr_ctrl->raw & ~(VKY_MCR_TEXT_OVERLAY | VKY_MCR_GRAPHICS | VKY_MCR_BITMAP);
	}
}

/**
 * @brief Set the 
 * 
 * @param plane the number of the bitmap to set
 * @param src the address of the bitmap data to use (should be in video RAM)
 */
void bm_set_data(short plane, uint8_t * src) {
	uint32_t src_raw = (uint32_t)src - (uint32_t)vram_base;

	switch (plane) {
		case 0:
			bm0_address[0] = (uint8_t)(src_raw & 0xff);
			bm0_address[1] = (uint8_t)((src_raw >> 8) & 0xff);
			bm0_address[2] = (uint8_t)((src_raw >> 16) & 0xff);
			break;

		case 1:
			bm1_address[0] = (uint8_t)(src_raw & 0xff);
			bm1_address[1] = (uint8_t)((src_raw >> 8) & 0xff);
			bm1_address[2] = (uint8_t)((src_raw >> 16) & 0xff);
			break;

		default:
			break;
	}
}

/**
 * @brief Fill the bitmap data with a given color
 * 
 * @param dest the area of video RAM to fill
 * @param color the color index to fill it with
 * @param width the width of the image in pixels
 * @param height the height of the image in pixels
 */
void bm_fill(uint8_t * dest, uint8_t color, int width, int height) {
	vdma_fill_linear(dest, color, (long)width * (long)height);
}

/**
 * @brief Load the color lookup table
 * 
 * @param clut number of the CLUT to load
 * @param src pointer to the source data for the CLUT (in B, G, R, A order)
 */
void bm_load_clut(short clut, uint8_t * src) {
	uint8_t * base = (uint8_t *)((uint32_t)VKY_GR_CLUT_0 + 4l * 256l * (uint32_t)clut);

	tvky_bg_color->blue = src[0];
	tvky_bg_color->green = src[1];
	tvky_bg_color->red = src[2];

	for (int i = 0; i < 4 * 256; i++) {
		base[i] = src[i];
	}
}

/**
 * @brief Load RLE bitmap data into video RAM
 * 
 * @param dest address to load with the expanded bitmap data (should be in video RAM)
 * @param src address of the RLE data to expand
 * @param width the width of the image in pixels
 * @param height the height of the image in pixels
 */
void bm_load_rle(uint8_t * dest, uint8_t * src, int width, int height) {
	uint32_t my_base = (uint32_t)dest;

	// Clear the image map
	bm_fill(dest, 0, width, height);

	uint8_t count = *(src++);
	while (count != 0) {
		uint8_t value = *(src++);
		if (value != 0) {
			volatile uint8_t * my_pointer = (uint8_t *)my_base;
			for (int i = 0; i < count; i++) {
				my_pointer[i] = value;
			}
		}
		my_base += count;

		count = *(src++);
	}
}

/**
 * @brief Initialize the bitmap system
 * 
 */
void bm_init() {
	bm_visible = 0;
}