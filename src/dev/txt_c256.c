/** @file txt_c256.c
 *
 * Text screen driver for C256's main screen (U, U+, and FMX)
 * The driver for the EVID will be in a separate file.
 */

#include <string.h>
#include <stdio.h>
#include "log.h"
#include "utilities.h"
#include "C256/vickyii_c256.h"
#include "dev/txt_screen.h"
#include "dev/txt_c256.h"

extern const unsigned char MSX_CP437_8x8_bin[];

/* Default text color lookup table values (AARRGGBB) */
const unsigned long c256_lut[VKY3_B_LUT_SIZE] = {
    0xFF000000,	// Black (transparent)
	0xFF800000, // Mid-Tone Red
	0xFF008000, // Mid-Tone Green
	0xFF808000, // Mid-Tone Yellow
	0xFF000080, // Mid-Tone Blue
	0xFFAA5500, // Mid-Tone Orange
	0xFF008080, // Mid-Tone Cian
	0xFF808080, // 50% Grey
	0xFF555555, // Dark Grey
    0xFFFF0000, // Bright Red
	0xFF55FF55, // Bright Green
	0xFFFFFF55, // Bright Yellow
	0xFF5555FF, // Bright Blue
	0xFFFF7FFF, // Bright Orange
	0xFF55FFFF, // Bright Cyan
	0xFFFFFFFF 	// White
};

/*
 * Driver level variables for the screen
 */

unsigned char c256_enable_set_sizes;    /* Flag to enable set_sizes to actually do its computation */
t_txt_capabilities c256_caps;           /* The capabilities of Channel B */
t_extent c256_resolutions[6];           /* The list of display resolutions */
t_extent c256_fonts[1];                 /* The list of font resolutions */
t_rect c256_region;                     /* The current region */
t_point c256_cursor;                    /* The current cursor position */
t_extent c256_resolution;               /* The current display resolution */
t_extent c256_font_size;                /* The current font size */
t_extent c256_max_size;                 /* The size of the screen in characters (without border removed) */
t_extent c256_visible_size;             /* The size of the visible screen in characters (with border removed) */
short c256_border_width;                /* Width of the border on one side */
short c256_border_height;               /* Height of the border on one side */
unsigned char c256_color;               /* The current color */
unsigned char c256_msr_shadow;          /* A shadow register for the Master Control Register low byte */
unsigned char c256_gamma_shadow;        /* A shadow register for the Gamma and Sync control register */

/**
 * Gets the description of a screen's capabilities
 *
 * @return a pointer to the read-only description (0 on error)
 */
const p_txt_capabilities txt_c256_get_capabilities() {
    return &c256_caps;
}

/**
 * Calculate the size of the text screen in rows and columns so that
 * the kernel printing routines can work correctly.
 *
 * NOTE: this should be called whenever the VKY3 Channel B registers are changed
 */
void txt_c256_set_sizes() {
    if (c256_enable_set_sizes) {
        /* Only recalculate after initialization is mostly completed */

        /*
         * Calculate the maximum number of characters visible on the screen
         * This controls text layout in memory
         */
        c256_max_size.width = c256_resolution.width / c256_font_size.width;
        c256_max_size.height = c256_resolution.height / c256_font_size.height;

        /*
         * Calculate the characters that are visible in whole or in part
         */
        short border_width = (2 * c256_border_width) / c256_font_size.width;
        short border_height = (2 * c256_border_height) / c256_font_size.height;
        c256_visible_size.width = c256_max_size.width - border_width;
        c256_visible_size.height = c256_max_size.height - border_height;
    }
}

/**
 * Get the display resolutions
 *
 * @param text_size the size of the screen in visible characters (may be null)
 * @param pixel_size the size of the screen in pixels (may be null)
 */
void txt_c256_get_sizes(p_extent text_size, p_extent pixel_size) {
    if (text_size) {
        text_size->width = c256_visible_size.width;
        text_size->height = c256_visible_size.height;
    }

    if (pixel_size) {
        pixel_size->width = c256_resolution.width;
        pixel_size->height = c256_resolution.height;
    }
}

/**
 * Set the display mode for the screen
 *
 * @param mode a bitfield of desired display mode options
 *
 * @return 0 on success, any other number means the mode is invalid for the screen
 */
short txt_c256_set_mode(short mode) {
    /* Turn off anything not set */
    c256_msr_shadow &= ~(VKY2_MSTR_CTRL_TEXT | VKY2_MSTR_CTRL_OVERLAY | VKY2_MSTR_CTRL_GRAPHICS | VKY2_MSTR_CTRL_BITMAP | VKY2_MSTR_CTRL_TILE | VKY2_MSTR_CTRL_SPRITE);
    c256_gamma_shadow &= ~(VKY2_SYNC_OFF);

    if (mode & TXT_MODE_SLEEP) {
        /* Put the monitor to sleep: overrides all other option bits */
        c256_gamma_shadow |= VKY2_SYNC_OFF;
        *VKY2_GAMMA_CTRL = c256_gamma_shadow;
        return 0;

    } else {
        if (mode & ~(TXT_MODE_TEXT | TXT_MODE_BITMAP | TXT_MODE_SPRITE | TXT_MODE_TILE)) {
            /* A mode bit was set beside one of the supported ones... */
            return -1;

        } else {
            if (mode & TXT_MODE_TEXT) {
                c256_msr_shadow |= VKY2_MSTR_CTRL_TEXT;
            }

            if (mode & TXT_MODE_BITMAP) {
                c256_msr_shadow |= VKY2_MSTR_CTRL_GRAPHICS | VKY2_MSTR_CTRL_BITMAP;
            }

            if (mode & TXT_MODE_SPRITE) {
                c256_msr_shadow |= VKY2_MSTR_CTRL_GRAPHICS | VKY2_MSTR_CTRL_SPRITE;
            }

            if (mode & TXT_MODE_TILE) {
                c256_msr_shadow |= VKY2_MSTR_CTRL_GRAPHICS | VKY2_MSTR_CTRL_TILE;
            }

            if ((c256_msr_shadow & (VKY2_MSTR_CTRL_GRAPHICS | VKY2_MSTR_CTRL_TEXT)) == (VKY2_MSTR_CTRL_GRAPHICS | VKY2_MSTR_CTRL_TEXT)) {
                c256_msr_shadow |= VKY2_MSTR_CTRL_OVERLAY;
            }

            *VKY2_MSTR_CTRL_L = c256_msr_shadow;
            return 0;
        }
    }
}

/**
 * Set the display resolution of the screen
 *
 * @param width the desired horizontal resolution in pixels
 * @param height the desired veritical resolution in pixels
 *
 * @return 0 on success, any other number means the resolution is unsupported
 */
short txt_c256_set_resolution(short width, short height) {
    int i;
    unsigned char msr_h_shadow = 0;

    // If no size specified, set it based on the DIP switch
    if ((width == 0) || (height == 0)) {
        if ((*VKY2_GAMMA_CTRL & VKY2_HIRES_DP_SW_VAL) == 0) {
            width = 800;
            height = 600;
        } else {
            width = 640;
            height = 480;
        }
    }

    short base_height = height;

    // Kick the PLL
    // If VICKY is generating a 40MHz signal, we need to switch the bit to go to 40MHz before
    // clearing it to go back to 25MHz.
    if (*VKY2_MSTR_CTRL_H & VKY2_MSTR_CTRL_40MHZ) {
        *VKY2_MSTR_CTRL_H |= VKY2_MSTR_CTRL_40MHZ;
        *VKY2_MSTR_CTRL_H &= ~(VKY2_MSTR_CTRL_40MHZ);
    }

    for (i = 0; i < c256_caps.resolution_count; i++) {
        if ((c256_caps.resolutions[i].width == width) && (c256_caps.resolutions[i].height == height)) {
            if (height < 400) {
                /* We're in pixel doubling range */
                msr_h_shadow |= VKY2_MSTR_CTRL_PIXDBL;

                /* Figure out what the base resolution is */
                base_height *= 2;
            }

            // Use the height to determine the resolution we should set
            switch (base_height) {
                case 480:   // 640x480 or 320x240 (mode = 00)
                    break;

                case 600:   // 800x600 or 400x300 (mode = 01)
                    msr_h_shadow |= VKY2_MSTR_CTRL_40MHZ;
                    break;

                default:
                    // Shouldn't get here... bad resolution
                    return -1;
            }

            // Update the kernel variables
            c256_resolution.width = width;
            c256_resolution.height = height;

            // Recalculate the size of the screen
            txt_c256_set_sizes();

            // Kick the PLL
            if (*VKY2_MSTR_CTRL_H & VKY2_MSTR_CTRL_40MHZ) {
                *VKY2_MSTR_CTRL_H &= ~(VKY2_MSTR_CTRL_PIXDBL | VKY2_MSTR_CTRL_40MHZ);
                *VKY2_MSTR_CTRL_H |= VKY2_MSTR_CTRL_40MHZ;
            }
            *VKY2_MSTR_CTRL_H &= ~(VKY2_MSTR_CTRL_PIXDBL | VKY2_MSTR_CTRL_40MHZ);

            // Update the register
            *VKY2_MSTR_CTRL_H = msr_h_shadow;
            return 0;
        }
    }
    return -1;
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param width the horizontal size of one side of the border (0 - 32 pixels)
 * @param height the vertical size of one side of the border (0 - 32 pixels)
 */
void txt_c256_set_border(short width, short height) {
    if ((width > 0) || (height > 0)) {
        c256_border_width = width;
        c256_border_height = height;
        *VKY2_BRDR_CTRL = VKY2_BRDR_ENABLE;
        *VKY2_BRDR_X_SIZE = width & 0x1f;
        *VKY2_BRDR_Y_SIZE = width & 0x1f;

        // Recalculate the size of the screen
        txt_c256_set_sizes();

    } else {
        c256_border_width = 0;
        c256_border_height = 0;
        *VKY2_BRDR_CTRL = 0;
        *VKY2_BRDR_X_SIZE = 0;
        *VKY2_BRDR_Y_SIZE = 0;

        // Recalculate the size of the screen
        txt_c256_set_sizes();
    }
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param red the red component of the color (0 - 255)
 * @param green the green component of the color (0 - 255)
 * @param blue the blue component of the color (0 - 255)
 */
void txt_c256_set_border_color(unsigned char red, unsigned char green, unsigned char blue) {
    *VKY2_BRDR_COLOR_R = red;
    *VKY2_BRDR_COLOR_G = green;
    *VKY2_BRDR_COLOR_B = blue;
}

/**
 * Load a font as the current font for the screen
 *
 * @param width width of a character in pixels
 * @param height of a character in pixels
 * @param data pointer to the raw font data to be loaded
 */
short txt_c256_set_font(short width, short height, const unsigned char * data) {
    if ((width == 8) && (height == 8)) {
        int i;

        /* The size is valid... set the font */
        c256_font_size.width = width;
        c256_font_size.height = height;

        /* Copy the font data... this assumes a width of one byte! */
        for (i = 0; i < 256 * height; i++) {
            VKY2_FONT_MEMORY[i] = data[i];
        }

        return 0;

    } else {
        return -1;
    }
}

/**
 * Set the appearance of the cursor
 *
 * @param enable 0 to hide, any other number to make visible
 * @param rate the blink rate for the cursor (0=1s, 1=0.5s, 2=0.25s, 3=1/5s)
 * @param c the character in the current font to use as a cursor
 */
void txt_c256_set_cursor(short enable, short rate, char c) {
    if (enable) {
        *VKY2_TXT_CRSR_CTRL = ((rate & 0x03) << 1) | VKY2_TXT_CRSR_ENABLE;
        *VKY2_TXT_CRSR_CHARACTER = c;
        *VKY2_TXT_CRSR_COLOR = c256_color;
    } else {
        *VKY2_TXT_CRSR_CTRL = 0;
    }
}

/**
 * Set if the cursor is visible or not
 *
 * @param enable 0 to hide, any other number to make visible
 */
 void txt_c256_set_cursor_visible(short enable) {
     if (enable) {
         *VKY2_TXT_CRSR_CTRL |= VKY2_TXT_CRSR_ENABLE;
     } else {
         *VKY2_TXT_CRSR_CTRL &= ~VKY2_TXT_CRSR_ENABLE;
     }
 }

/**
 * Set a region to restrict further character display, scrolling, etc.
 * Note that a null region pointer and a region of zero size will reset the region to the full size of the screen.
 *
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
 short txt_c256_set_region(p_rect region) {
     if ((region->size.width == 0) || (region->size.height == 0)) {
         /* Set the region to the default (full screen) */
         c256_region.origin.x = 0;
         c256_region.origin.y = 0;
         c256_region.size.width = c256_visible_size.width;
         c256_region.size.height = c256_visible_size.height;

     } else {
         c256_region.origin.x = region->origin.x;
         c256_region.origin.y = region->origin.y;
         c256_region.size.width = region->size.width;
         c256_region.size.height = region->size.height;
     }

     return 0;
 }

/**
 * get the current region
 *
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
short txt_c256_get_region(p_rect region) {
    region->origin.x = c256_region.origin.x;
    region->origin.y = c256_region.origin.y;
    region->size.width = c256_region.size.width;
    region->size.height = c256_region.size.height;

    return 0;
}

/**
 * Set the default foreground and background colors for printing
 *
 * @param foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param background the Text LUT index of the new current background color (0 - 15)
 */
short txt_c256_set_color(unsigned char foreground, unsigned char background) {
    c256_color = ((foreground & 0x0f) << 4) | (background & 0x0f);
    return 0;
}

/**
 * Get the default foreground and background colors for printing
 *
 * @param pointer to the foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param pointer to the background the Text LUT index of the new current background color (0 - 15)
 */
short txt_c256_get_color(unsigned char * foreground, unsigned char * background) {
    *foreground = (c256_color & 0xf0) >> 4;
    *background = c256_color & 0x0f;
    return 0;
}


/**
 * Scroll the text in the current region
 *
 * @param screen the number of the text device
 * @param horizontal the number of columns to scroll (negative is left, positive is right)
 * @param vertical the number of rows to scroll (negative is down, positive is up)
 */
void txt_c256_scroll(short horizontal, short vertical) {
    short x, x0, x1, x2, x3, dx;
    short y, y0, y1, y2, y3, dy;

    /*
     * Determine limits of rectangles to move and fill and directions of loops
     * x0 and y0 are the positions of the first cell to be over-written
     * x1 and y1 are the positions of the first cell to be copyed... TEXT[x0,y0] := TEXT[x1,y1]
     * x2 and y2 are the position of the last cell to be over-written
     * x3 and y3 are the position of the last cell to be copied... TEXT[x2,y2] := TEXT[x3,y3]
     *
     * When blanking, the rectangles (x2,y0) - (x3,y3) and (x0,y2) - (x2,y3) are cleared
     */

    // Determine the row limits

    if (vertical >= 0) {
        y0 = c256_region.origin.y;
        y1 = y0 + vertical;
        y3 = c256_region.origin.y + c256_region.size.height;
        y2 = y3 - vertical;
        dy = 1;
    } else {
        y0 = c256_region.origin.y + c256_region.size.height - 1;
        y1 = y0 + vertical;
        y3 = c256_region.origin.y - 1;
        y2 = y3 - vertical;
        dy = -1;
    }

    // Determine the column limits

    if (horizontal >= 0) {
        x0 = c256_region.origin.x;
        x1 = x0 + horizontal;
        x3 = c256_region.origin.x + c256_region.size.width;
        x2 = x3 - horizontal;
        dx = 1;
    } else {
        x0 = c256_region.origin.x + c256_region.size.width - 1;
        x1 = x0 + horizontal;
        x3 = c256_region.origin.x - 1;
        x2 = x3 - horizontal;
        dx = -1;
    }

    /* Copy the rectangle */

    for (y = y0; y != y2; y += dy) {
        int row_dst = y * c256_max_size.width;
        int row_src = (y + vertical) * c256_max_size.width;

        for (x = x0; x != x2; x += dx) {
            int offset_dst = row_dst + x;
            int offset_src = row_src + x + horizontal;

            VKY2_TEXT_MATRIX[offset_dst] = VKY2_TEXT_MATRIX[offset_src];
            VKY2_COLOR_MATRIX[offset_dst] = VKY2_COLOR_MATRIX[offset_src];
        }
    }

    /* Clear the rectangles */

    if (horizontal != 0) {
        for (y = y0; y != y3; y += dy) {
            int row_dst = y * c256_max_size.width;
            for (x = x2; x != x3; x += dx) {
                VKY2_TEXT_MATRIX[row_dst + x] = ' ';
                VKY2_COLOR_MATRIX[row_dst + x] = c256_color;
            }
        }
    }

    if (vertical != 0) {
        for (y = y2; y != y3; y += dy) {
            int row_dst = y * c256_max_size.width;
            for (x = x0; x != x3; x += dx) {
                VKY2_TEXT_MATRIX[row_dst + x] = ' ';
                VKY2_COLOR_MATRIX[row_dst + x] = c256_color;
            }
        }
    }
}

/**
 * Fill the current region with a character in the current color
 *
 * @param screen the number of the text device
 * @param c the character to fill the region with
 */
void txt_c256_fill(char c) {
    int x;
    int y;

    for (y = 0; y < c256_region.size.height; y++) {
        int offset_row = (c256_region.origin.y + y) * c256_max_size.width;
        for (x = 0; x < c256_region.size.width; x++) {
            int offset = offset_row + c256_region.origin.x + x;
            VKY2_TEXT_MATRIX[offset] = c;
            VKY2_COLOR_MATRIX[offset] = c256_color;
        }
    }
}

/**
 * Set the position of the cursor to (x, y) relative to the current region
 * If the (x, y) coordinate is outside the region, it will be clipped to the region.
 * If y is greater than the height of the region, the region will scroll until that relative
 * position would be within view.
 *
 * @param x the column for the cursor
 * @param y the row for the cursor
 */
void txt_c256_set_xy(short x, short y) {
    /* Make sure X is within range for the current region... "print" a newline if not */
    if (x < 0) {
        x = 0;
    } else if (x >= c256_region.size.width) {
        x = 0;
        y++;
    }

    /* Make sure Y is within range for the current region... scroll if not */
    if (y < 0) {
        y = 0;
    } else if (y >= c256_region.size.height) {
        txt_c256_scroll(0, y - c256_region.size.height + 1);
        y = c256_region.size.height - 1;
    }

    c256_cursor.x = x;
    c256_cursor.y = y;

    /* Set register */
    *VKY2_TXT_CRSR_X = c256_region.origin.x + x;
    *VKY2_TXT_CRSR_Y = c256_region.origin.y; + y;
}

/**
 * Get the position of the cursor (x, y) relative to the current region
 *
 * @param screen the number of the text device
 * @param position pointer to a t_point record to fill out
 */
void txt_c256_get_xy(p_point position) {
    position->x = c256_cursor.x;
    position->y = c256_cursor.y;
}

/**
 * Print a character to the current cursor position in the current color
 *
 * @param screen the number of the text device
 * @param c the character to print
 */
void txt_c256_put(char c) {
    short x;
    short y;
    unsigned int offset;

    x = c256_region.origin.x + c256_cursor.x;
    y = c256_region.origin.y + c256_cursor.y;
    offset = y * c256_max_size.width + x;
    VKY2_TEXT_MATRIX[offset] = c;
    VKY2_COLOR_MATRIX[offset] = c256_color;

    txt_c256_set_xy(c256_cursor.x + 1, c256_cursor.y);
}

/**
 * Initialize the screen
 */
void txt_c256_init() {
    char buffer[255];
    t_rect region;
    int i;

    // Kick the PLL
    // If VICKY is generating a 40MHz signal, we need to switch the bit to go to 40MHz before
    // clearing it to go back to 25MHz.
    // if (*VKY3_B_MCR & VKY3_B_CLK40) {
    //     *VKY3_B_MCR |= VKY3_B_PLL | VKY3_B_MODE1;
    //     *VKY3_B_MCR &= ~(VKY3_B_PLL | VKY3_B_MODE1);
    // }

    c256_resolution.width = 0;
    c256_resolution.height = 0;
    c256_font_size.width = 0;
    c256_font_size.height = 0;

    /* Disable the set_sizes call for now */
    c256_enable_set_sizes = 0;

    /* Start with nothing on */
    c256_msr_shadow = 0;
    c256_gamma_shadow = 0;

    /* Define the capabilities */

    /* Specify the screen number */
    c256_caps.number = TXT_SCREEN_C256;

    /* This screen can be nothing, sleep, or any combination of text, sprite, bitmap, and tile */
    c256_caps.supported_modes = TXT_MODE_TEXT | TXT_MODE_SPRITE | TXT_MODE_BITMAP | TXT_MODE_TILE | TXT_MODE_SLEEP;

    /* Resolutions supported: 320x240, 400x300, 640x480, 800x600 */
    c256_resolutions[0].width = 320;
    c256_resolutions[0].height = 240;
    c256_resolutions[1].width = 400;
    c256_resolutions[1].height = 300;
    c256_resolutions[2].width = 640;
    c256_resolutions[2].height = 480;
    c256_resolutions[3].width = 800;
    c256_resolutions[3].height = 600;
    c256_caps.resolution_count = 4;
    c256_caps.resolutions = c256_resolutions;

    /* Channel B supports 8x8 fonts ONLY */
    c256_fonts[0].width = 8;
    c256_fonts[0].height = 8;
    c256_caps.font_size_count = 1;
    c256_caps.font_sizes = c256_fonts;

    /* Initialze the color lookup tables */
    for (i = 0; i < VKY3_B_LUT_SIZE; i++) {
        int offset = i * 4;
        unsigned long color = c256_lut[i];
        unsigned char red = (unsigned char)(color & 0xff0000 >> 16);
        unsigned char green = (unsigned char)(color & 0x00ff00 >> 8);
        unsigned char blue = (unsigned char)(color & 0x0000ff);

        VKY2_TXT_FORE_LUT[offset] = blue;
        VKY2_TXT_FORE_LUT[offset + 1] = green;
        VKY2_TXT_FORE_LUT[offset + 2] = red;

        VKY2_TXT_BACK_LUT[offset] = blue;
        VKY2_TXT_BACK_LUT[offset + 1] = green;
        VKY2_TXT_BACK_LUT[offset + 2] = red;
    }

    /* Set the mode to text */
    txt_c256_set_mode(TXT_MODE_TEXT);

    /* Set the resolution */
    txt_c256_set_resolution(640, 480);                  /* Default resolution is 640x480 */

    /* Set the default color: light grey on blue */
    txt_c256_set_color(0x07, 0x04);

    /* Set the font */
    txt_c256_set_font(8, 8, MSX_CP437_8x8_bin);         /* Use 8x8 font */

    /* Set the cursor */
    txt_c256_set_cursor(1, 0, 0xB1);

    /* Set the border */
    txt_c256_set_border(16, 16);                            /* Set up the border */
    txt_c256_set_border_color(0, 0, 0x3f);

    /*
     * Enable set_sizes, now that everything is set up initially
     * And calculate the size of the screen
     */
    c256_enable_set_sizes = 1;
    txt_c256_set_sizes();

    /* Set region to default */
    region.origin.x = 0;
    region.origin.y = 0;
    region.size.width = 0;
    region.size.height = 0;
    txt_c256_set_region(&region);

    /* Home the cursor */
    txt_c256_set_xy(0, 0);

    /* Clear the screen */
    txt_c256_fill(' ');
}

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
short txt_c256_install() {
    t_txt_device device;

    device.number = TXT_SCREEN_C256;
    device.name = "MAIN SCREEN";

    device.init = txt_c256_init;
    device.get_capabilities = txt_c256_get_capabilities;
    device.set_mode = txt_c256_set_mode;
    device.set_sizes = txt_c256_set_sizes;
    device.set_resolution = txt_c256_set_resolution;
    device.set_border = txt_c256_set_border;
    device.set_border_color = txt_c256_set_border_color;
    device.set_font = txt_c256_set_font;
    device.set_cursor = txt_c256_set_cursor;
    device.set_cursor_visible = txt_c256_set_cursor_visible;
    device.set_region = txt_c256_set_region;
    device.get_region = txt_c256_get_region;
    device.set_color = txt_c256_set_color;
    device.get_color = txt_c256_get_color;
    device.set_xy = txt_c256_set_xy;
    device.get_xy = txt_c256_get_xy;
    device.put = txt_c256_put;
    device.scroll = txt_c256_scroll;
    device.fill = txt_c256_fill;
    device.get_sizes = txt_c256_get_sizes;

    return txt_register(&device);
}
