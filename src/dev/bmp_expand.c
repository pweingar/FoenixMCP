#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vicky_general.h"

/**
 * @brief 
 * 
 */

struct bmp_color_s {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t reserved;
};

struct bmp_infoheader_s {
	uint32_t infoheader_size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t image_size;
	uint32_t x_pixels_per_m;
	uint32_t y_pixels_per_m;
	uint32_t colors_used;
	uint32_t colors_important;
	struct bmp_color_s color_table[];
};

struct bmp_header_s {
	char signature[2];
	uint32_t file_size;
	uint32_t reserved;
	uint32_t offset;
	struct bmp_infoheader_s info_header;
};

const uint16_t BMP_BC_MONO = 1;				// Monochrome
const uint16_t BMP_BC_INDEX_16 = 4;			// 4-bit indexed color (16 colors total)
const uint16_t BMP_BC_INDEX_256 = 8;		// 8-bit indexed color (256 colors total)
const uint16_t BMP_BC_RGB_16 = 16;			// 16-bit RGB
const uint16_t BMP_BC_RGB_24 = 24;			// 24-bit RGB (True Color)

const uint32_t BMP_RGB = 0;					// No compression
const uint32_t BMP_RLE8 = 1;				// 8-bit run-length-encoded
const uint32_t BMP_RLE4 = 2;				// 4-bit run-length-encoded

enum bmp_error_e {
	BMP_OK = 0,								// BMP file was processed without error
	BMP_BAD_FILETYPE,						// File does not have a BMP signature
	BMP_BAD_FORMAT							// BMP file is not in a supported format (color format or compression)
};

/**
 * @brief Set the components of a color in a graphics color lookup table
 * 
 * NOTE: this routine might need to be swapped out for different hardware
 * 
 * @param clut the number of the graphics CLUT to update
 * @param color the number of the color to update
 * @param red the red component
 * @param green the green component
 * @param blue the blue component
 */
static void bm_set_color(short clut, short color, uint8_t red, uint8_t green, uint8_t blue) {
	volatile uint8_t * clut_p = VKY_GR_CLUT_0 + (clut * 256 * 4);
	uint32_t offset = color * 4;
	clut_p[offset] = blue;
	clut_p[offset+1] = green;
	clut_p[offset+2] = red;
	clut_p[offset+3] = 0xff;
}

/**
 * @brief Load a color lookup table
 * 
 * @param src_colors pointer to the BMP color table
 * @param numcolors the number of colors in the BMP color table
 * @param clut the number of the color palette to load
 */
static void load_palette(struct bmp_color_s * src_colors, int numcolors, int clut) {
	for (int color = 0; color < numcolors; color++) {
		bm_set_color(clut, color, src_colors[color].red, src_colors[color].green, src_colors[color].blue);
	}
}

/**
 * @brief Decompress the BMP 256 color image data, given 8-bit RLE compression
 * 
 * @param img_data pointer to the BMP compressed image data
 * @param width the width of the image to build up
 * @param height the height of the image to build up
 * @param destination pointer to the destination to expand the image into
 */
static void expand_index8_rle8(char * img_data, uint32_t width, uint32_t height, char * destination) {
	int i = 0;
	uint32_t row = 0;
	uint32_t column = 0;
	uint32_t img_index = 0;

	// Calculate address of the first row
	char * dest_row = (char *)((uint32_t)destination + (height - row - 1) * width);

	// Default the image to color 0
	memset(destination, 0, width * height);

	// Process the data blocks in the image data until we reach the end of the image
	while (1) {
		char count = img_data[img_index++];
		char value = img_data[img_index++];

		if (count > 0) {
			// We have a run of bytes
			for (i = 0; i < count; i++) {
				dest_row[column++] = value;
				if (column >= width) {
					column = 0;
					row -= 1;
					dest_row = (char *)((uint32_t)destination + (height - row - 1) * width);
				}
			}

		} else {
			// Count == 0: we have to process the escape sequence
			switch (value) {
				case 0:
					// End of line... handle padding
					img_index += (3 - img_index % 4);
					break;

				case 1:
					// End of bitmap... we're done
					return;

				case 2:
					// Delta... adjust row and column
					char dx = img_data[img_index++];
					char dy = img_data[img_index++];

					row += dy;
					column += dx;
					dest_row = (char *)((uint32_t)destination + (height - row - 1) * width);
					break;

				default:
					// value indicates the number of bytes to copy raw
					for (i = 0; i < value; i++) {
						char b = img_data[img_index++];
						dest_row[column++] = b;
						if (column >= width) {
							column = 0;
							row -= 1;
							dest_row = (char *)((uint32_t)destination + (height - row - 1) * width);
						}

						if (img_index % 2) {
							// Skip over padding, if we're on an odd byte
							img_index++;
						}
					}
					break;
			}
		}
	}

	return BMP_OK;
}

/**
 * @brief Render a BMP file into a destination bitmap image and a destination color palette
 * 
 * @param bmp_data pointer to the raw BMP data
 * @param dest_img pointer to the area in memory to load the expanded bitmap data
 * @param clut the number of the graphics CLUT to use for the image
 * @return enum bmp_error_e whether or not the image could be processed
 */
enum bmp_error_e bm_bmp_render(char * bmp_data, char * dest_img, short clut) {
	struct bmp_header_s * bmp_header  = (struct bmp_header_s *)bmp_data;
	if ((bmp_header->signature[0] == 'B') & (bmp_header->signature[0] == 'M')) {
		// We have a BMP file... calculate the starting address of the image data
		uint8_t * img_data = (uint8_t *)(bmp_data + bmp_header->offset);

		// Get the statistics on the BMP image
		uint32_t width = bmp_header->info_header.width;
		uint32_t height = bmp_header->info_header.height;
		uint32_t bit_count = bmp_header->info_header.bit_count;
		uint32_t compression = bmp_header->info_header.compression;

		// Dispatch to the correct expansion routine based on the color mode and the compression
		switch (bit_count) {
			case BMP_BC_INDEX_256:
				switch (compression) {
					case BMP_RLE8:
						// 256 color indexed compressed using RLE8
						load_palette(bmp_header->info_header.color_table, (int)bmp_header->info_header.colors_used, clut);
						expand_index8_rle8(img_data, width, height, dest_img);
						break;

					default:
						// Unsupported compression for 256 color mode
						return BMP_BAD_FORMAT;
				}

			default:
				// Unsupported color mode
				return BMP_BAD_FORMAT;
		}

		return BMP_OK;
	} else {
		return BMP_BAD_FILETYPE;
	}
}