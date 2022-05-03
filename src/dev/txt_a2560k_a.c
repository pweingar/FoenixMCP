/** @file txt_a2560k_a.c
 *
 * Text screen driver for A2560K Channel A
 */

#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "log.h"
#include "utilities.h"
#include "A2560K/vky_chan_a.h"
#include "A2560K/vky_chan_b.h"
#include "dev/txt_screen.h"
#include "dev/txt_a2560k_a.h"

extern const unsigned char MSX_CP437_8x8_bin[];

/* Default text color lookup table values (AARRGGBB) */
const unsigned long a2560k_a_lut[VKY3_A_LUT_SIZE] = {
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

unsigned char a2560k_a_enable_set_sizes;    /* Flag to enable set_sizes to actually do its computation */
t_txt_capabilities a2560k_a_caps;           /* The capabilities of Channel A */
t_extent a2560k_a_resolutions[2];           /* The list of display resolutions */
t_extent a2560k_a_fonts[2];                 /* The list of font resolutions */
t_rect a2560k_a_region;                     /* The current region */
t_point a2560k_a_cursor;                    /* The current cursor position */
t_extent a2560k_a_resolution;               /* The current display resolution */
t_extent a2560k_a_font_size;                /* The current font size */
t_extent a2560k_a_max_size;                 /* The size of the screen in characters (without border removed) */
t_extent a2560k_a_visible_size;             /* The size of the visible screen in characters (with border removed) */
short a2560k_a_border_width;                /* Width of the border on one side */
short a2560k_a_border_height;               /* Height of the border on one side */
unsigned char a2560k_a_color;               /* The current color */
unsigned long msr_shadow;                   /* A shadow register for the Master Control Register */

/**
 * Gets the description of a screen's capabilities
 *
 * @return a pointer to the read-only description (0 on error)
 */
const p_txt_capabilities txt_a2560k_a_get_capabilities() {
    return &a2560k_a_caps;
}

/**
 * Calculate the size of the text screen in rows and columns so that
 * the kernel printing routines can work correctly.
 *
 * NOTE: this should be called whenever the VKY3 Channel A registers are changed
 */
void txt_a2560k_a_set_sizes() {
    if (a2560k_a_enable_set_sizes) {
        /* Only recalculate after initialization is mostly completed */

        /*
         * Calculate the maximum number of characters visible on the screen
         * This controls text layout in memory
         */
        a2560k_a_max_size.width = a2560k_a_resolution.width / a2560k_a_font_size.width;
        a2560k_a_max_size.height = a2560k_a_resolution.height / a2560k_a_font_size.height;

        // /* Set the font manager register */
        *VKY3_A_FM1 = (a2560k_a_max_size.height & 0xff) << 8 | (a2560k_a_max_size.width & 0xff);

        /*
         * Calculate the characters that are visible in whole or in part
         */
        if ((a2560k_a_border_width != 0) && (a2560k_a_border_height != 0)) {
            short border_width = (2 * a2560k_a_border_width) / a2560k_a_font_size.width;
            short border_height = (2 * a2560k_a_border_height) / a2560k_a_font_size.height;
            a2560k_a_visible_size.width = a2560k_a_max_size.width - border_width;
            a2560k_a_visible_size.height = a2560k_a_max_size.height - border_height;
        } else {
            a2560k_a_visible_size.width = a2560k_a_max_size.width;
            a2560k_a_visible_size.height = a2560k_a_max_size.height;
        }
    }
}

/**
 * Get the display resolutions
 *
 * @param text_size the size of the screen in visible characters (may be null)
 * @param pixel_size the size of the screen in pixels (may be null)
 */
void txt_a2560k_a_get_sizes(p_extent text_size, p_extent pixel_size) {
    if (text_size) {
        text_size->width = a2560k_a_visible_size.width;
        text_size->height = a2560k_a_visible_size.height;
    }

    if (pixel_size) {
        pixel_size->width = a2560k_a_resolution.width;
        pixel_size->height = a2560k_a_resolution.height;
    }
}

/**
 * Set the display mode for the screen
 *
 * @param mode a bitfield of desired display mode options
 *
 * @return 0 on success, any other number means the mode is invalid for the screen
 */
short txt_a2560k_a_set_mode(short mode) {
    /* Turn off anything not set */
    msr_shadow &= ~(TXT_MODE_SLEEP | TXT_MODE_TEXT);

    if (mode & TXT_MODE_SLEEP) {
        /* Put the monitor to sleep */
        msr_shadow |= VKY3_A_MCR_SLEEP;
        *VKY3_A_MCR = msr_shadow;
        return 0;

    } else if (mode & TXT_MODE_TEXT) {
        /* Put on text mode */
        msr_shadow |= VKY3_A_MCR_TEXT;
        *VKY3_A_MCR = msr_shadow;
        return 0;

    } else {
        /* Unsupported mode */
        return -1;
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
short txt_a2560k_a_set_resolution(short width, short height) {

    // If no size specified, set it based on the DIP switch
    if ((width == 0) || (height == 0)) {
        if ((*VKY3_B_MCR & VKY3_B_HIRES) == 0) {
            width = 1024;
            height = 768;
        } else {
            width = 800;
            height = 600;
        }
    }

    // Kick the PLL
    // If VICKY is generating a 40MHz signal, we need to switch the bit to go to 40MHz before
    // clearing it to go back to 25MHz.
    if (*VKY3_A_MCR & VKY3_A_CLK40) {
        *VKY3_A_MCR |= VKY3_A_1024x768;
        *VKY3_A_MCR &= ~(VKY3_A_1024x768);
    }

    /* Turn off resolution bits */
    msr_shadow &= ~(VKY3_A_1024x768);

    if ((width == 800) && (height == 600)) {
        a2560k_a_resolution.width = width;
        a2560k_a_resolution.height = height;

        // Recalculate the size of the screen
        txt_a2560k_a_set_sizes();

        *VKY3_A_MCR = msr_shadow;
        return 0;

    } else if ((width == 1024) && (height == 768)) {
        msr_shadow |= VKY3_A_1024x768;
        a2560k_a_resolution.width = width;
        a2560k_a_resolution.height = height;

        // Recalculate the size of the screen
        txt_a2560k_a_set_sizes();

        *VKY3_A_MCR = msr_shadow;
        return 0;

    } else {
        /* Unsupported resolution */
        return -1;
    }
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param width the horizontal size of one side of the border (0 - 32 pixels)
 * @param height the vertical size of one side of the border (0 - 32 pixels)
 */
void txt_a2560k_a_set_border(short width, short height) {
    if ((width > 0) || (height > 0)) {
        a2560k_a_border_width = width;
        a2560k_a_border_height = height;
        *VKY3_A_BCR = (height & 0x3f) << 16 | (width & 0x3f) << 8 | VKY3_A_BCR_ENABLE;

        // Recalculate the size of the screen
        txt_a2560k_a_set_sizes();

    } else {
        a2560k_a_border_width = 0;
        a2560k_a_border_height = 0;
        *VKY3_A_BCR = 0;

        // Recalculate the size of the screen
        txt_a2560k_a_set_sizes();
    }
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param red the red component of the color (0 - 255)
 * @param green the green component of the color (0 - 255)
 * @param blue the blue component of the color (0 - 255)
 */
void txt_a2560k_a_set_border_color(unsigned char red, unsigned char green, unsigned char blue) {
    *VKY3_A_BRDCOLOR = (unsigned long)(((red & 0xff) << 16) | ((green & 0xff) << 8) | (blue & 0xff));
}

/**
 * Load a font as the current font for the screen
 *
 * @param width width of a character in pixels
 * @param height of a character in pixels
 * @param data pointer to the raw font data to be loaded
 */
short txt_a2560k_a_set_font(short width, short height, const unsigned char * data) {
    if (((width == 8) && (height == 8)) || ((width == 8) && (height == 16))) {
        int i;

        /* The size is valid... set the font */
        a2560k_a_font_size.width = width;
        a2560k_a_font_size.height = height;

        /* Set the size of the character and container */
        *VKY3_A_FM0 = ((height & 0xff) << 24) | ((width & 0xff) << 16) | ((height & 0xff) << 8) | (width & 0xff);

        /* Copy the font data... this assumes a width of one byte! */
        /* TODO: generalize this for all possible font sizes */
        for (i = 0; i < 256 * height; i++) {
            VKY3_A_FONT_MEMORY[i] = data[i];
        }

        // Recalculate the size of the screen
        txt_a2560k_a_set_sizes();

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
void txt_a2560k_a_set_cursor(short enable, short rate, char c) {
    *VKY3_A_CCR = ((a2560k_a_color & 0xff) << 24) | ((c & 0xff) << 16) | ((rate & 0x03) << 1) | (enable & 0x01);
}

/**
 * Set if the cursor is visible or not
 *
 * @param enable 0 to hide, any other number to make visible
 */
void txt_a2560k_a_set_cursor_visible(short enable) {
    if (enable) {
        *VKY3_A_CCR |= 0x01;
    } else {
        *VKY3_A_CCR &= ~0x01;
    }
}

/**
 * get the current region
 *
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
short txt_a2560k_a_get_region(p_rect region) {
    region->origin.x = a2560k_a_region.origin.x;
    region->origin.y = a2560k_a_region.origin.y;
    region->size.width = a2560k_a_region.size.width;
    region->size.height = a2560k_a_region.size.height;

    return 0;
}

/**
 * Set a region to restrict further character display, scrolling, etc.
 * Note that a region of zero size will reset the region to the full size of the screen.
 *
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
short txt_a2560k_a_set_region(p_rect region) {
    if ((region->size.width == 0) || (region->size.height == 0)) {
        /* Set the region to the default (full screen) */
        a2560k_a_region.origin.x = 0;
        a2560k_a_region.origin.y = 0;
        a2560k_a_region.size.width = a2560k_a_visible_size.width;
        a2560k_a_region.size.height = a2560k_a_visible_size.height;

    } else {
        a2560k_a_region.origin.x = region->origin.x;
        a2560k_a_region.origin.y = region->origin.y;
        a2560k_a_region.size.width = region->size.width;
        a2560k_a_region.size.height = region->size.height;
    }

    return 0;
}

/**
 * Get the default foreground and background colors for printing
 *
 * @param pointer to the foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param pointer to the background the Text LUT index of the new current background color (0 - 15)
 */
short txt_a2560k_a_get_color(unsigned char * foreground, unsigned char * background) {
    *foreground = (a2560k_a_color & 0xf0) >> 4;
    *background = a2560k_a_color & 0x0f;
    return 0;
}

/**
 * Set the default foreground and background colors for printing
 *
 * @param foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param background the Text LUT index of the new current background color (0 - 15)
 */
short txt_a2560k_a_set_color(unsigned char foreground, unsigned char background) {
    a2560k_a_color = ((foreground & 0x0f) << 4) + (background & 0x0f);
    return 0;
}


/**
 * Scroll the text in the current region
 *
 * @param screen the number of the text device
 * @param horizontal the number of columns to scroll (negative is left, positive is right)
 * @param vertical the number of rows to scroll (negative is down, positive is up)
 */
void txt_a2560k_a_scroll(short horizontal, short vertical) {
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
        y0 = a2560k_a_region.origin.y;
        y1 = y0 + vertical;
        y3 = a2560k_a_region.origin.y + a2560k_a_region.size.height;
        y2 = y3 - vertical;
        dy = 1;
    } else {
        y0 = a2560k_a_region.origin.y + a2560k_a_region.size.height - 1;
        y1 = y0 + vertical;
        y3 = a2560k_a_region.origin.y - 1;
        y2 = y3 - vertical;
        dy = -1;
    }

    // Determine the column limits

    if (horizontal >= 0) {
        x0 = a2560k_a_region.origin.x;
        x1 = x0 + horizontal;
        x3 = a2560k_a_region.origin.x + a2560k_a_region.size.width;
        x2 = x3 - horizontal;
        dx = 1;
    } else {
        x0 = a2560k_a_region.origin.x + a2560k_a_region.size.width - 1;
        x1 = x0 + horizontal;
        x3 = a2560k_a_region.origin.x - 1;
        x2 = x3 - horizontal;
        dx = -1;
    }

    /* Copy the rectangle */

    for (y = y0; y != y2; y += dy) {
        int row_dst = y * a2560k_a_max_size.width;
        int row_src = (y + vertical) * a2560k_a_max_size.width;
        for (x = x0; x != x2; x += dx) {
            int offset_dst = row_dst + x;
            int offset_src = row_src + x + horizontal;
            VKY3_A_TEXT_MATRIX[offset_dst] = VKY3_A_TEXT_MATRIX[offset_src];
            VKY3_A_COLOR_MATRIX[offset_dst] = VKY3_A_COLOR_MATRIX[offset_src];
        }
    }

    /* Clear the rectangles */

    if (horizontal != 0) {
        for (y = y0; y != y3; y += dy) {
            int row_dst = y * a2560k_a_max_size.width;
            for (x = x2; x != x3; x += dx) {
                VKY3_A_TEXT_MATRIX[row_dst + x] = ' ';
                VKY3_A_COLOR_MATRIX[row_dst + x] = a2560k_a_color;
            }
        }
    }

    if (vertical != 0) {
        for (y = y2; y != y3; y += dy) {
            int row_dst = y * a2560k_a_max_size.width;
            for (x = x0; x != x3; x += dx) {
                VKY3_A_TEXT_MATRIX[row_dst + x] = ' ';
                VKY3_A_COLOR_MATRIX[row_dst + x] = a2560k_a_color;
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
void txt_a2560k_a_fill(char c) {
    int x;
    int y;

    for (y = 0; y < a2560k_a_region.size.height; y++) {
        int offset_row = (a2560k_a_region.origin.y + y) * a2560k_a_max_size.width;
        for (x = 0; x < a2560k_a_region.size.width; x++) {
            int offset = offset_row + a2560k_a_region.origin.x + x;
            VKY3_A_TEXT_MATRIX[offset] = c;
            VKY3_A_COLOR_MATRIX[offset] = a2560k_a_color;
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
void txt_a2560k_a_set_xy(short x, short y) {
    /* Make sure X is within range for the current region... "print" a newline if not */
    if (x < 0) {
        x = 0;
    } else if (x >= a2560k_a_region.size.width) {
        x = 0;
        y++;
    }

    /* Make sure Y is within range for the current region... scroll if not */
    if (y < 0) {
        y = 0;
    } else if (y >= a2560k_a_region.size.height) {
        txt_a2560k_a_scroll(0, y - a2560k_a_region.size.height + 1);
        y = a2560k_a_region.size.height - 1;
    }

    a2560k_a_cursor.x = x;
    a2560k_a_cursor.y = y;

    /* Set register */
    *VKY3_A_CPR = (((a2560k_a_region.origin.y + y) & 0xffff) << 16) | ((a2560k_a_region.origin.x + x) & 0xffff);
}

/**
 * Get the position of the cursor (x, y) relative to the current region
 *
 * @param screen the number of the text device
 * @param position pointer to a t_point record to fill out
 */
void txt_a2560k_a_get_xy(p_point position) {
    position->x = a2560k_a_cursor.x;
    position->y = a2560k_a_cursor.y;
}

/**
 * Print a character to the current cursor position in the current color
 *
 * @param screen the number of the text device
 * @param c the character to print
 */
void txt_a2560k_a_put(char c) {
    short x;
    short y;
    unsigned int offset;

    x = a2560k_a_region.origin.x + a2560k_a_cursor.x;
    y = a2560k_a_region.origin.y + a2560k_a_cursor.y;
    offset = y * a2560k_a_max_size.width + x;
    VKY3_A_TEXT_MATRIX[offset] = c;
    VKY3_A_COLOR_MATRIX[offset] = a2560k_a_color;

    txt_a2560k_a_set_xy(a2560k_a_cursor.x + 1, a2560k_a_cursor.y);
}

/**
 * Initialize the screen
 */
void txt_a2560k_a_init() {
    char buffer[255];
    t_rect region;
    int i;

    // Kick the PLL
    // If VICKY is generating a 40MHz signal, we need to switch the bit to go to 40MHz before
    // clearing it to go back to 25MHz.
    if (*VKY3_A_MCR & VKY3_A_CLK40) {
        *VKY3_A_MCR |= VKY3_A_1024x768;
        *VKY3_A_MCR &= ~(VKY3_A_1024x768);
    }

    a2560k_a_resolution.width = 0;
    a2560k_a_resolution.height = 0;
    a2560k_a_font_size.width = 0;
    a2560k_a_font_size.height = 0;

    /* Disable the set_sizes call for now */
    a2560k_a_enable_set_sizes = 0;

    /* Start with nothing on */
    msr_shadow = 0;

    /* Define the capabilities */

    /* Specify the screen number */
    a2560k_a_caps.number = TXT_SCREEN_A2560K_A;

    /* This screen can be text or can be put to sleep */
    a2560k_a_caps.supported_modes = TXT_MODE_TEXT | TXT_MODE_SLEEP;

    /* Resolutions supported: 800x600, 1024x768 */
    a2560k_a_resolutions[0].width = 800;
    a2560k_a_resolutions[0].height = 600;
    a2560k_a_resolutions[1].width = 1024;
    a2560k_a_resolutions[1].height = 768;
    a2560k_a_caps.resolution_count = 2;
    a2560k_a_caps.resolutions = a2560k_a_resolutions;

    /* At the moment, support only 8x8 and 8x16 fonts */
    /* TODO: add support for all possible font sizes */
    a2560k_a_fonts[0].width = 8;
    a2560k_a_fonts[0].height = 8;
    a2560k_a_fonts[1].width = 8;
    a2560k_a_fonts[1].height = 16;
    a2560k_a_caps.font_size_count = 2;
    a2560k_a_caps.font_sizes = a2560k_a_fonts;

    /* Initialze the color lookup tables */
    for (i = 0; i < VKY3_A_LUT_SIZE; i++) {
        VKY3_A_TEXT_LUT_FG[i] = a2560k_a_lut[i];
        VKY3_A_TEXT_LUT_BG[i] = a2560k_a_lut[i];
    }

    /* Set the mode to text */
    txt_a2560k_a_set_mode(TXT_MODE_TEXT);

    /* Set the resolution */
    txt_a2560k_a_set_resolution(800, 600);                      /* Default resolution is 800x600 */

    /* Set the default color: light grey on blue */
    txt_a2560k_a_set_color(0x07, 0x04);

    /* Set the font */
    txt_a2560k_a_set_font(8, 8, MSX_CP437_8x8_bin);             /* Use 8x8 font */

    /* Set the cursor */
    txt_a2560k_a_set_cursor(1, 0, 0xB1);

    /* Set the border */
    txt_a2560k_a_set_border(16, 16);                            /* Set up the border */
    txt_a2560k_a_set_border_color(0, 0, 0x3f);

    /*
     * Enable set_sizes, now that everything is set up initially
     * And calculate the size of the screen
     */
    a2560k_a_enable_set_sizes = 1;
    txt_a2560k_a_set_sizes();

    /* Set region to default */
    region.origin.x = 0;
    region.origin.y = 0;
    region.size.width = 0;
    region.size.height = 0;
    txt_a2560k_a_set_region(&region);

    /* Home the cursor */
    txt_a2560k_a_set_xy(0, 0);

    /* Clear the screen */
    txt_a2560k_a_fill(' ');
}

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
short txt_a2560k_a_install() {
    t_txt_device device;

    device.number = TXT_SCREEN_A2560K_A;
    device.name = "SCREEN A";

    device.init = txt_a2560k_a_init;
    device.get_capabilities = txt_a2560k_a_get_capabilities;
    device.set_mode = txt_a2560k_a_set_mode;
    device.set_sizes = txt_a2560k_a_set_sizes;
    device.set_resolution = txt_a2560k_a_set_resolution;
    device.set_border = txt_a2560k_a_set_border;
    device.set_border_color = txt_a2560k_a_set_border_color;
    device.set_font = txt_a2560k_a_set_font;
    device.set_cursor = txt_a2560k_a_set_cursor;
    device.set_cursor_visible = txt_a2560k_a_set_cursor_visible;
    device.get_region = txt_a2560k_a_get_region;
    device.set_region = txt_a2560k_a_set_region;
    device.get_color = txt_a2560k_a_get_color;
    device.set_color = txt_a2560k_a_set_color;
    device.set_xy = txt_a2560k_a_set_xy;
    device.get_xy = txt_a2560k_a_get_xy;
    device.put = txt_a2560k_a_put;
    device.scroll = txt_a2560k_a_scroll;
    device.fill = txt_a2560k_a_fill;
    device.get_sizes = txt_a2560k_a_get_sizes;

    return txt_register(&device);
}
