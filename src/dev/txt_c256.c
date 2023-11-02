/** @file txt_c256.c
 *
 * Text screen driver for c256
 */

#include <string.h>
#include <stdio.h>

#include "log.h"
#include "C256/vicky_ii.h"
#include "dev/txt_c256.h"
#include "dev/txt_screen.h"

extern const unsigned char MSX_CP437_8x8_bin[];

const t_color4 c256_clut[] = {
    {0, 0, 0},          // 0: Black
    {0, 0, 128},        // 1: Red
    {0, 128, 0},        // 2: Green
    {0, 128, 128},      // 3: Yellow
    {128, 0, 0},        // 4: Blue
    {128, 0, 128},      // 5: Magenta
    {128, 128, 0},      // 6: Cyan
    {192, 192, 192},    // 7: White
    {128, 128, 128},    // 8: Bright Gray
    {0, 0, 255},        // 9: Bright Red
    {0, 255, 0},        // A: Bright Green
    {0, 255, 255},      // B: Bright Yellow
    {255, 0, 0},        // C: Bright Blue
    {255, 0, 255},      // D: Bright Magenta
    {255, 255, 0},      // E: Bright Cyan
    {255, 255, 255}     // F: Bright White
};

/*
 * Driver level variables for the screen
 */

unsigned char c256_enable_set_sizes;    /* Flag to enable set_sizes to actually do its computation */

t_txt_capabilities c256_caps;           /* The capabilities of Channel A */

t_extent c256_resolutions[] = {   /* The list of display resolutions */
    { 800, 600 },
    { 400, 300 },
    { 640, 480 },
    { 320, 240 }
};

t_extent c256_fonts[] = {         /* The list of supported font resolutions */
    { 8, 8 }
};

t_rect c256_region;                     /* The current region */
t_point c256_cursor;                    /* The current cursor position */
t_extent c256_resolution;               /* The current display resolution */
t_extent c256_font_size;                /* The current font size */
t_extent c256_max_size;                 /* The size of the screen in characters (without border removed) */
t_extent c256_visible_size;             /* The size of the visible screen in characters (with border removed) */
short c256_border_width;                /* Width of the border on one side */
short c256_border_height;               /* Height of the border on one side */
unsigned char c256_color;               /* The current color */
unsigned long msr_shadow;               /* A shadow register for the Master Control Register */

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
 * NOTE: this should be called whenever the VKY3 registers are changed
 */
static void txt_c256_set_sizes() {
	TRACE("txt_c256_set_sizes");
	
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
        if ((c256_border_width != 0) && (c256_border_height != 0)) {
            short border_width = (2 * c256_border_width) / c256_font_size.width;
            short border_height = (2 * c256_border_height) / c256_font_size.height;

            c256_visible_size.width = c256_max_size.width - border_width;
            c256_visible_size.height = c256_max_size.height - border_height;
        } else {
            c256_visible_size.width = c256_max_size.width;            
            c256_visible_size.height = c256_max_size.height;
        }
	
        DEBUG4("txt_c256_set_sizes max:%d,%d, visible:%d,%d", c256_max_size.width, c256_max_size.height, c256_visible_size.width, c256_visible_size.height);
    }
}

/**
 * Get the display resolutions
 *
 * @param text_size the size of the screen in visible characters (may be null)
 * @param pixel_size the size of the screen in pixels (may be null)
 */
static void txt_c256_get_sizes(p_extent text_size, p_extent pixel_size) {
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
static short txt_c256_set_mode(short mode) {
    /* Turn off anything not set */
    msr_shadow &= ~(TXT_MODE_SLEEP | TXT_MODE_TEXT);

    if (mode & TXT_MODE_SLEEP) {
        /* Put the monitor to sleep */
        msr_shadow |= VKY_MCR_DISABLE;
        tvky_mstr_ctrl->raw = msr_shadow;
        return 0;

    } else if (mode & TXT_MODE_TEXT) {
        /* Put on text mode */
        msr_shadow |= VKY_MCR_TEXT;
        tvky_mstr_ctrl->raw = msr_shadow;
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
static short txt_c256_set_resolution(short width, short height) {
    // TODO: If no size specified, set it based on the DIP switch


    /* Turn off resolution bits */
    /* TODO: there gotta be a better way to do that */
    msr_shadow &= ~(VKY_MCR_RES_MASK);

    if ((width == 800) && (height == 600)) {
        msr_shadow |= VKY_MCR_RES_800x600;
        c256_resolution.width = width;
        c256_resolution.height = height;

        // Recalculate the size of the screen
        txt_c256_set_sizes();

        tvky_mstr_ctrl->raw = msr_shadow;
        return 0;

    } else if ((width == 640) && (height == 480)) {
        msr_shadow |= VKY_MCR_RES_640x480;
        c256_resolution.width = width;
        c256_resolution.height = height;

        // Recalculate the size of the screen
        txt_c256_set_sizes();

        tvky_mstr_ctrl->raw = msr_shadow;
        return 0;
    }
    else if ((width == 320) && (height == 240)) {
        msr_shadow |= VKY_MCR_RES_320x240;
        c256_resolution.width = width;
        c256_resolution.height = height;

        // Recalculate the size of the screen
        txt_c256_set_sizes();

        tvky_mstr_ctrl->raw = msr_shadow;
        return 0;
    }
    else if ((width == 400) && (height == 300)) {
        msr_shadow |= VKY_MCR_RES_400x300;
        c256_resolution.width = width;
        c256_resolution.height = height;

        // Recalculate the size of the screen
        txt_c256_set_sizes();

        tvky_mstr_ctrl->raw = msr_shadow;
        return 0;
    }
    else {
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
static void txt_c256_set_border(short width, short height) {
    if ((width > 0) || (height > 0)) {
        c256_border_width = width;
        c256_border_height = height;
        tvky_brdr_ctrl->control = 0x01;
        tvky_brdr_ctrl->size_x = width;
        tvky_brdr_ctrl->sizy_y = height;
        
    } else {
        tvky_brdr_ctrl->control = 0;
        tvky_brdr_ctrl->size_x = 0;
        tvky_brdr_ctrl->sizy_y = 0;
    }

    // Recalculate the size of the screen
    txt_c256_set_sizes();    
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param red the red component of the color (0 - 255)
 * @param green the green component of the color (0 - 255)
 * @param blue the blue component of the color (0 - 255)
 */
static void txt_c256_set_border_color(unsigned char red, unsigned char green, unsigned char blue) {
    tvky_brdr_ctrl->color.red = red;
    tvky_brdr_ctrl->color.green = green;
    tvky_brdr_ctrl->color.blue = blue;
}

/**
 * Load a font as the current font for the screen
 *
 * @param width width of a character in pixels
 * @param height of a character in pixels
 * @param data pointer to the raw font data to be loaded
 */
static short txt_c256_set_font(short width, short height, const unsigned char * data) {
    if (width == 8 && height == 8) {
        int i;

        /* The size is valid... set the font */
        c256_font_size.width = width;
        c256_font_size.height = height;

        /* Copy the font data... this assumes a width of one byte! */
        /* TODO: generalize this for all possible font sizes */
        for (i = 0; i < 256 * height; i++) {
            tvky_font_set_0[i] = data[i];
        }

        // Recalculate the size of the screen
        txt_c256_set_sizes();

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
static void txt_c256_set_cursor(short enable, short rate, char c) {
    tvky_crsr_ctrl->control = ((rate & 0x03) << 1) | ((enable) ? 1 : 0);
    tvky_crsr_ctrl->character = c;
}

/**
 * Set if the cursor is visible or not
 *
 * @param enable 0 to hide, any other number to make visible
 */
static void txt_c256_set_cursor_visible(short enable) {
    if (enable) {
        tvky_crsr_ctrl->control |= 0x01;
    } else {
        tvky_crsr_ctrl->control &= ~0x01;
    }
}

/**
 * get the current region
 *
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
static short txt_c256_get_region(p_rect region) {
    region->origin.x = c256_region.origin.x;
    region->origin.y = c256_region.origin.y;
    region->size.width = c256_region.size.width;
    region->size.height = c256_region.size.height;

    {
        char msg[80];
        sprintf(msg,"txt_c256_get_region %p: x:%d, y:%d, w:%d, h:%d", region, region->origin.x, region->origin.y, region->size.width, region->size.height);
        DEBUG(msg);
    }  

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
static short txt_c256_set_region(const p_rect region) {    
    // char msg[80];
    // sprintf(msg,"SET REGION %p x:%d, y:%d, w:%d, h:%d (visible:%d,%d)",
    // region, region->origin.x, region->origin.y, region->size.width, region->size.height, c256_visible_size.width, c256_visible_size.height);
    // DEBUG(msg);

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

        //sprintf(msg,"specific region %d %d %d %d", region->origin.x, region->origin.y, region->size.width, region->size.height);
        //DEBUG(msg);
    }

    //sprintf(msg,"txt_c256_set_region: NEW REGION %p x:%d, y:%d, w:%d, h:%d (visible:%d,%d)",
    //region, region->origin.x, region->origin.y, region->size.width, region->size.height, c256_visible_size.width, c256_visible_size.height);
    //DEBUG(msg);

    return 0;
}

/**
 * Get the default foreground and background colors for printing
 *
 * @param pointer to the foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param pointer to the background the Text LUT index of the new current background color (0 - 15)
 */
static short txt_c256_get_color(unsigned char * foreground, unsigned char * background) {
    *foreground = (c256_color & 0xf0) >> 4;
    *background = c256_color & 0x0f;
    return 0;
}

/**
 * Set the default foreground and background colors for printing
 *
 * @param foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param background the Text LUT index of the new current background color (0 - 15)
 */
static short txt_c256_set_color(unsigned char foreground, unsigned char background) {
    c256_color = ((foreground & 0x0f) << 4) + (background & 0x0f);
    return 0;
}

/**
 * Scroll the text in the current region
 *
 * @param screen the number of the text device
 * @param horizontal the number of columns to scroll (negative is left, positive is right)
 * @param vertical the number of rows to scroll (negative is down, positive is up)
 */
static void txt_c256_scroll(short horizontal, short vertical) {
    short x, x0, x1, x2, x3, dx;
    short y, y0, y1, y2, y3, dy;

    /*
     * Determine limits of rectangles to move and fill and directions of loops
     * x0 and y0 are the positions of the first cell to be over-written
     * x1 and y1 are the positions of the first cell to be copied... TEXT[x0,y0] := TEXT[x1,y1]
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
   
    /* Copy the rectangle. */
    int delta_y = dy * c256_max_size.width;
    int row_dst = y0 * c256_max_size.width - delta_y;
    int row_src = y0 * c256_max_size.width + vertical * c256_max_size.width - delta_y;
    for (y = y0; y != y2; y += dy) {
        row_dst += delta_y;
        row_src += delta_y;
        int offset_dst = row_dst + x0 - dx;
        int offset_src = row_src + horizontal + x0 - dx;
        for (x = x0; x != x2; x += dx) {
            offset_dst += dx;
            offset_src += dx;
            tvky_text_matrix[offset_dst] = tvky_text_matrix[offset_src];
            tvky_color_matrix[offset_dst] = tvky_color_matrix[offset_src];
        }
    }

    /* Clear the rectangles */
    if (horizontal != 0) {
        row_dst = y0 * c256_max_size.width - delta_y;
        for (y = y0; y != y3; y += dy) {
            row_dst += delta_y;
            for (x = x2; x != x3; x += dx) {
                tvky_text_matrix[row_dst + x] = ' ';
                tvky_color_matrix[row_dst + x] = c256_color;
            }
        }
    }

    if (vertical != 0) {
        row_dst = y2 * c256_max_size.width - delta_y;
        for (y = y2; y != y3; y += dy) {
            row_dst += delta_y;
            for (x = x0; x != x3; x += dx) {
                tvky_text_matrix[row_dst + x] = ' ';
                tvky_color_matrix[row_dst + x] = c256_color;
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
static void txt_c256_fill(char c) {
    int x;
    int y;

    for (y = 0; y < c256_region.size.height; y++) {
        int offset_row = (c256_region.origin.y + y) * c256_max_size.width;
        for (x = 0; x < c256_region.size.width; x++) {
            int offset = offset_row + c256_region.origin.x + x;
            tvky_text_matrix[offset] = c;
            tvky_color_matrix[offset] = c256_color;
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
static void txt_c256_set_xy(short x, short y) {
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
    tvky_crsr_ctrl->column = c256_region.origin.x + x;
    tvky_crsr_ctrl->row = c256_region.origin.y + y;
}

/**
 * Get the position of the cursor (x, y) relative to the current region
 *
 * @param screen the number of the text device
 * @param position pointer to a t_point record to fill out
 */
static void txt_c256_get_xy(p_point position) {
    position->x = c256_cursor.x;
    position->y = c256_cursor.y;
}

/**
 * Print a character to the current cursor position in the current color
 *
 * @param c the character to print
 */
static void txt_c256_put(char c) {
    short x;
    short y;
    unsigned int offset;

    x = c256_region.origin.x + c256_cursor.x;
    y = c256_region.origin.y + c256_cursor.y;
    offset = y * c256_max_size.width + x;
    tvky_text_matrix[offset] = c;
    tvky_color_matrix[offset] = c256_color;

    txt_c256_set_xy(c256_cursor.x + 1, c256_cursor.y);
}

/**
 * Initialize the screen
 */
static void txt_c256_init() {
    char buffer[255];
    t_rect region;
    int i;
    DEBUG("txt_c256_init");

    c256_resolution.width = 0;
    c256_resolution.height = 0;
    c256_visible_size.width = 0;
    c256_visible_size.height = 0;
    c256_font_size.width = 0;
    c256_font_size.height = 0;

    /* Disable the set_sizes call for now, to avoid computing transcient unnecessary values */
    c256_enable_set_sizes = 0;

    /* Start with nothing on */
    msr_shadow = 0;

    /* Define the capabilities */

    /* Specify the screen number. We have only one so... */
    c256_caps.number = TXT_SCREEN_C256;

    /* This screen can be text, bitmap or can be put to sleep */
    c256_caps.supported_modes = TXT_MODE_TEXT | TXT_MODE_SPRITE | TXT_MODE_BITMAP | TXT_MODE_TILE | TXT_MODE_SLEEP;

    /* Supported resolutions */
    c256_caps.resolution_count = sizeof(c256_resolutions) / sizeof(t_extent);
    c256_caps.resolutions = c256_resolutions;

    /* Only 8x8 on the U */
    c256_caps.font_size_count = sizeof(c256_fonts) / sizeof(t_extent);
    c256_caps.font_sizes = c256_fonts;

    /* Initialize the color lookup tables */
    for (i = 0; i < sizeof(c256_clut)/sizeof(t_color4); i++) {
		tvky_text_fg_color[i] = c256_clut[i];
		tvky_text_bg_color[i] = c256_clut[i];        
    }

    /* Set the mode to text */
    txt_c256_set_mode(TXT_MODE_TEXT);

    /* Set the resolution */
    txt_c256_set_resolution(640, 480);      

    /* Set the default color: light grey on blue */
    txt_c256_set_color(0x07, 0x04);

    /* Set the font */
    txt_c256_set_font(8, 8, MSX_CP437_8x8_bin);             /* Use 8x8 font */

    /* Set the cursor */
    txt_c256_set_cursor(1, 0, 0xB1);

    /* Set the border */
    txt_c256_set_border(0, 0);                              /* Set up the border */
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
    device.name = "SCREEN";

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
    device.get_region = txt_c256_get_region;
    device.set_region = txt_c256_set_region;
    device.get_color = txt_c256_get_color;
    device.set_color = txt_c256_set_color;
    device.set_xy = txt_c256_set_xy;
    device.get_xy = txt_c256_get_xy;
    device.put = txt_c256_put;
    device.scroll = txt_c256_scroll;
    device.fill = txt_c256_fill;
    device.get_sizes = txt_c256_get_sizes;

    return txt_register(&device);
}
