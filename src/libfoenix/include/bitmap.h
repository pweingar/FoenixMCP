/**
 * @brief Definitions for functions to manage bitmap displays
 * 
 */

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>

/**
 * @brief Set the visibility of a given bitmap plane, and the CLUT to use
 * 
 * @param plane the number of the bitmap to update
 * @param clut the number of the CLUT to use for the bitmap
 * @param is_visible 0 to hide the bitmap, any other number to show it
 */
extern void bm_set_visibility(short plane, short clut, short is_visible);

/**
 * @brief Set the 
 * 
 * @param plane the number of the bitmap to set
 * @param src the address of the bitmap data to use (should be in video RAM)
 */
extern void bm_set_data(short plane, uint8_t * src);

/**
 * @brief Fill the bitmap data with a given color
 * 
 * @param dest the area of video RAM to fill
 * @param color the color index to fill it with
 * @param width the width of the image in pixels
 * @param height the height of the image in pixels
 */
extern void bm_fill(uint8_t * dest, uint8_t color, int width, int height);

/**
 * @brief Load the color lookup table
 * 
 * @param clut number of the CLUT to load
 * @param src pointer to the source data for the CLUT (in B, G, R, A order)
 */
extern void bm_load_clut(short clut, uint8_t * src);

/**
 * @brief Load RLE bitmap data into video RAM
 * 
 * @param dest address to load with the expanded bitmap data (should be in video RAM)
 * @param src address of the RLE data to expand
 * @param width the width of the image in pixels
 * @param height the height of the image in pixels
 */
extern void bm_load_rle(uint8_t * dest, uint8_t * src, int width, int height);

/**
 * @brief Initialize the bitmap system
 * 
 */
extern void bm_init();

#endif
