/**
 * @file txt_evid.c
 * @brief Low level FoenixMCP driver for the C200 EVID expansion card for the C256 computers
 * @version 0.1
 * @date 2023-08-09
 * 
 */

#include <stdlib.h>
#include <stdint.h>

#include "log.h"
#include "C256/vicky_ii.h"
#include "dev/txt_evid.h"
#include "dev/txt_screen.h"


#define gabe_sys_stat           ((volatile __attribute__((far)) uint8_t *)0xafe887)
#define GABE_EXPANSION_FLAG     0x10

#define expn_id_vendor          ((volatile __attribute__((far)) uint16_t *)0xae0010)
#define EXPN_VENDOR_FOENIX      0xf0e1
#define expn_id_card            ((volatile __attribute__((far)) uint16_t *)0xae0012)
#define EXPN_CARD_C200          0x9236

#define evid_font_set           ((volatile __attribute__((far)) uint8_t *)0xae1000)
#define evid_fg_clut            ((volatile __attribute__((far)) t_color4 *)0xae1b00)
#define evid_bg_clut            ((volatile __attribute__((far)) t_color4 *)0xae1b40)

#define evid_mstr_ctrl          ((volatile __attribute__((far)) uint16_t *)0xae1e00)
#define EVID_MCR_TEXT           0x0001
#define EVID_MCR_RES_800x600    0x0100

#define evid_brdr_ctrl          ((volatile __attribute__((far)) uint8_t *)0xae1e04)
#define evid_brdr_color         ((volatile __attribute__((far)) t_color4 *)0xae1e05)
#define evid_brdr_size_x        ((volatile __attribute__((far)) uint16_t *)0xae1e08)
#define evid_brdr_size_y        ((volatile __attribute__((far)) uint16_t *)0xae1e09)

#define evid_crsr_ctrl          ((volatile __attribute__((far)) uint8_t *)0xae1e10)
#define evid_crsr_char          ((volatile __attribute__((far)) uint8_t *)0xae1e12)
#define evid_crsr_color         ((volatile __attribute__((far)) uint8_t *)0xae1e13)
#define evid_crsr_column        ((volatile __attribute__((far)) uint16_t *)0xae1e14)
#define evid_crsr_row           ((volatile __attribute__((far)) uint16_t *)0xae1e16)

#define evid_text_matrix        ((volatile __attribute__((far)) uint8_t *)0xae2000)
#define evid_color_matrix       ((volatile __attribute__((far)) uint8_t *)0xae4000)

extern const unsigned char MSX_CP437_8x8_bin[];

const t_color4 evid_clut[] = {
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

unsigned char evid_enable_set_sizes;    /* Flag to enable set_sizes to actually do its computation */

t_txt_capabilities evid_caps;           /* The capabilities of the EVID */

t_extent evid_resolutions[] = {         /* The list of display resolutions */
    { 800, 600 },
    { 640, 480 }
};

t_extent evid_fonts[] = {               /* The list of supported font resolutions */
    { 8, 8 }
};

t_rect evid_region;                     /* The current region */
t_point evid_cursor;                    /* The current cursor position */
t_extent evid_resolution;               /* The current display resolution */
t_extent evid_font_size;                /* The current font size */
t_extent evid_max_size;                 /* The size of the screen in characters (without border removed) */
t_extent evid_visible_size;             /* The size of the visible screen in characters (with border removed) */
short evid_border_width;                /* Width of the border on one side */
short evid_border_height;               /* Height of the border on one side */
unsigned char evid_color;               /* The current color */
unsigned long evid_msr_shadow;          /* A shadow register for the Master Control Register */

/**
 * @brief Return TRUE if the C200 EVID card is present
 * 
 * @return short 0 if not present, any other value means the card is present
 */
short txt_evid_present() {
    if ((*gabe_sys_stat & GABE_EXPANSION_FLAG) == 0) {
        // An expansion card is present check to see if it's a C200 EVID
        return ((*expn_id_vendor == EXPN_VENDOR_FOENIX) && (*expn_id_card == EXPN_CARD_C200));

    } else {
        // No card is present...
        return 0;
    }
}

/**
 * Gets the description of a screen's capabilities
 *
 * @return a pointer to the read-only description (0 on error)
 */
const p_txt_capabilities txt_evid_get_capabilities() {
    return &evid_caps;
}

/**
 * Calculate the size of the text screen in rows and columns so that
 * the kernel printing routines can work correctly.
 *
 * NOTE: this should be called whenever the VKY3 registers are changed
 */
static void txt_evid_set_sizes() {
	TRACE("txt_evid_set_sizes");
	
    if (evid_enable_set_sizes) {
        /* Only recalculate after initialization is mostly completed */

        /*
         * Calculate the maximum number of characters visible on the screen
         * This controls text layout in memory
         */
        evid_max_size.width = evid_resolution.width / evid_font_size.width;
        evid_max_size.height = evid_resolution.height / evid_font_size.height;

        /*
         * Calculate the characters that are visible in whole or in part
         */
        if ((evid_border_width != 0) && (evid_border_height != 0)) {
            short border_width = (2 * evid_border_width) / evid_font_size.width;
            short border_height = (2 * evid_border_height) / evid_font_size.height;

            evid_visible_size.width = evid_max_size.width - border_width;
            evid_visible_size.height = evid_max_size.height - border_height;
        } else {
            evid_visible_size.width = evid_max_size.width;            
            evid_visible_size.height = evid_max_size.height;
        }
	
        DEBUG4("txt_evid_set_sizes max:%d,%d, visible:%d,%d", evid_max_size.width, evid_max_size.height, evid_visible_size.width, evid_visible_size.height);
    }
}

/**
 * Get the display resolutions
 *
 * @param text_size the size of the screen in visible characters (may be null)
 * @param pixel_size the size of the screen in pixels (may be null)
 */
static void txt_evid_get_sizes(p_extent text_size, p_extent pixel_size) {
    if (text_size) {
        text_size->width = evid_visible_size.width;
        text_size->height = evid_visible_size.height;
    }

    if (pixel_size) {
        pixel_size->width = evid_resolution.width;
        pixel_size->height = evid_resolution.height;
    }
}

/**
 * Set the display mode for the screen
 *
 * @param mode a bitfield of desired display mode options
 *
 * @return 0 on success, any other number means the mode is invalid for the screen
 */
static short txt_evid_set_mode(short mode) {
    /* Turn off anything not set */
    evid_msr_shadow &= ~(TXT_MODE_TEXT);

    if (mode & TXT_MODE_TEXT) {
        /* Put on text mode */
        evid_msr_shadow |= EVID_MCR_TEXT;
        *evid_mstr_ctrl = evid_msr_shadow;
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
static short txt_evid_set_resolution(short width, short height) {
    // TODO: If no size specified, set it based on the DIP switch

    /* Turn off resolution bits */
    /* TODO: there gotta be a better way to do that */
    evid_msr_shadow &= ~(EVID_MCR_RES_800x600);

    if ((width == 800) && (height == 600)) {
        evid_msr_shadow |= EVID_MCR_RES_800x600;
        evid_resolution.width = width;
        evid_resolution.height = height;

        // Recalculate the size of the screen
        txt_evid_set_sizes();

        *evid_mstr_ctrl = evid_msr_shadow;
        return 0;

    } else if ((width == 640) && (height == 480)) {
        evid_resolution.width = width;
        evid_resolution.height = height;

        // Recalculate the size of the screen
        txt_evid_set_sizes();

        *evid_mstr_ctrl = evid_msr_shadow;
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
static void txt_evid_set_border(short width, short height) {
    if ((width > 0) || (height > 0)) {
        evid_border_width = width;
        evid_border_height = height;
        *evid_brdr_ctrl = 0x01;
        *evid_brdr_size_x = width;
        *evid_brdr_size_y = height;
        
    } else {
        *evid_brdr_ctrl = 0;
        *evid_brdr_size_x = 0;
        *evid_brdr_size_y = 0;
    }

    // Recalculate the size of the screen
    txt_evid_set_sizes();    
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param red the red component of the color (0 - 255)
 * @param green the green component of the color (0 - 255)
 * @param blue the blue component of the color (0 - 255)
 */
static void txt_evid_set_border_color(unsigned char red, unsigned char green, unsigned char blue) {
    evid_brdr_color->red = red;
    evid_brdr_color->green = green;
    evid_brdr_color->blue = blue;
}

/**
 * Load a font as the current font for the screen
 *
 * @param width width of a character in pixels
 * @param height of a character in pixels
 * @param data pointer to the raw font data to be loaded
 */
static short txt_evid_set_font(short width, short height, const unsigned char * data) {
    if (width == 8 && height == 8) {
        int i;

        /* The size is valid... set the font */
        evid_font_size.width = width;
        evid_font_size.height = height;

        /* Copy the font data... this assumes a width of one byte! */
        /* TODO: generalize this for all possible font sizes */
        for (i = 0; i < 256 * height; i++) {
            evid_font_set[i] = data[i];
        }

        // Recalculate the size of the screen
        txt_evid_set_sizes();

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
static void txt_evid_set_cursor(short enable, short rate, char c) {
    *evid_crsr_ctrl = ((rate & 0x03) << 1) | ((enable) ? 1 : 0);
    *evid_crsr_char = c;
}

/**
 * Set if the cursor is visible or not
 *
 * @param enable 0 to hide, any other number to make visible
 */
static void txt_evid_set_cursor_visible(short enable) {
    if (enable) {
        *evid_crsr_ctrl|= 0x01;
    } else {
        *evid_crsr_ctrl &= ~0x01;
    }
}

/**
 * get the current region
 *
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
static short txt_evid_get_region(p_rect region) {
    region->origin.x = evid_region.origin.x;
    region->origin.y = evid_region.origin.y;
    region->size.width = evid_region.size.width;
    region->size.height = evid_region.size.height;

    // {
    //     char msg[80];
    //     sprintf(msg,"txt_evid_get_region %p: x:%d, y:%d, w:%d, h:%d", region, region->origin.x, region->origin.y, region->size.width, region->size.height);
    //     DEBUG(msg);
    // }  

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
static short txt_evid_set_region(const p_rect region) {    
    // char msg[80];
    // sprintf(msg,"SET REGION %p x:%d, y:%d, w:%d, h:%d (visible:%d,%d)",
    // region, region->origin.x, region->origin.y, region->size.width, region->size.height, evid_visible_size.width, evid_visible_size.height);
    // DEBUG(msg);

    if ((region->size.width == 0) || (region->size.height == 0)) {
        /* Set the region to the default (full screen) */
        evid_region.origin.x = 0;
        evid_region.origin.y = 0;
        evid_region.size.width = evid_visible_size.width;
        evid_region.size.height = evid_visible_size.height;        
    } else {
        evid_region.origin.x = region->origin.x;
        evid_region.origin.y = region->origin.y;
        evid_region.size.width = region->size.width;
        evid_region.size.height = region->size.height;

        //sprintf(msg,"specific region %d %d %d %d", region->origin.x, region->origin.y, region->size.width, region->size.height);
        //DEBUG(msg);
    }

    //sprintf(msg,"txt_evid_set_region: NEW REGION %p x:%d, y:%d, w:%d, h:%d (visible:%d,%d)",
    //region, region->origin.x, region->origin.y, region->size.width, region->size.height, evid_visible_size.width, evid_visible_size.height);
    //DEBUG(msg);

    return 0;
}

/**
 * Get the default foreground and background colors for printing
 *
 * @param pointer to the foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param pointer to the background the Text LUT index of the new current background color (0 - 15)
 */
static short txt_evid_get_color(unsigned char * foreground, unsigned char * background) {
    *foreground = (evid_color & 0xf0) >> 4;
    *background = evid_color & 0x0f;
    return 0;
}

/**
 * Set the default foreground and background colors for printing
 *
 * @param foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param background the Text LUT index of the new current background color (0 - 15)
 */
static short txt_evid_set_color(unsigned char foreground, unsigned char background) {
    evid_color = ((foreground & 0x0f) << 4) + (background & 0x0f);
    return 0;
}

/**
 * Scroll the text in the current region
 *
 * @param screen the number of the text device
 * @param horizontal the number of columns to scroll (negative is left, positive is right)
 * @param vertical the number of rows to scroll (negative is down, positive is up)
 */
static void txt_evid_scroll(short horizontal, short vertical) {
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
        y0 = evid_region.origin.y;
        y1 = y0 + vertical;
        y3 = evid_region.origin.y + evid_region.size.height;
        y2 = y3 - vertical;
        dy = 1;
    } else {
        y0 = evid_region.origin.y + evid_region.size.height - 1;
        y1 = y0 + vertical;
        y3 = evid_region.origin.y - 1;
        y2 = y3 - vertical;
        dy = -1;
    }

    // Determine the column limits

    if (horizontal >= 0) {
        x0 = evid_region.origin.x;
        x1 = x0 + horizontal;
        x3 = evid_region.origin.x + evid_region.size.width;
        x2 = x3 - horizontal;
        dx = 1;
    } else {
        x0 = evid_region.origin.x + evid_region.size.width - 1;
        x1 = x0 + horizontal;
        x3 = evid_region.origin.x - 1;
        x2 = x3 - horizontal;
        dx = -1;
    }
   
    /* Copy the rectangle. */
    int delta_y = dy * evid_max_size.width;
    int row_dst = y0 * evid_max_size.width - delta_y;
    int row_src = y0 * evid_max_size.width + vertical * evid_max_size.width - delta_y;
    for (y = y0; y != y2; y += dy) {
        row_dst += delta_y;
        row_src += delta_y;
        int offset_dst = row_dst + x0 - dx;
        int offset_src = row_src + horizontal + x0 - dx;
        for (x = x0; x != x2; x += dx) {
            offset_dst += dx;
            offset_src += dx;
            evid_text_matrix[offset_dst] = evid_text_matrix[offset_src];
            evid_color_matrix[offset_dst] = evid_color_matrix[offset_src];
        }
    }

    /* Clear the rectangles */
    if (horizontal != 0) {
        row_dst = y0 * evid_max_size.width - delta_y;
        for (y = y0; y != y3; y += dy) {
            row_dst += delta_y;
            for (x = x2; x != x3; x += dx) {
                evid_text_matrix[row_dst + x] = ' ';
                evid_color_matrix[row_dst + x] = evid_color;
            }
        }
    }

    if (vertical != 0) {
        row_dst = y2 * evid_max_size.width - delta_y;
        for (y = y2; y != y3; y += dy) {
            row_dst += delta_y;
            for (x = x0; x != x3; x += dx) {
                evid_text_matrix[row_dst + x] = ' ';
                evid_color_matrix[row_dst + x] = evid_color;
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
static void txt_evid_fill(char c) {
    int x;
    int y;

    for (y = 0; y < evid_region.size.height; y++) {
        int offset_row = (evid_region.origin.y + y) * evid_max_size.width;
        for (x = 0; x < evid_region.size.width; x++) {
            int offset = offset_row + evid_region.origin.x + x;
            evid_text_matrix[offset] = c;
            evid_color_matrix[offset] = evid_color;
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
static void txt_evid_set_xy(short x, short y) {
    /* Make sure X is within range for the current region... "print" a newline if not */
    if (x < 0) {
        x = 0;
    } else if (x >= evid_region.size.width) {
        x = 0;
        y++;
    }

    /* Make sure Y is within range for the current region... scroll if not */
    if (y < 0) {
        y = 0;
    } else if (y >= evid_region.size.height) {
        txt_evid_scroll(0, y - evid_region.size.height + 1);
        y = evid_region.size.height - 1;
    }

    evid_cursor.x = x;
    evid_cursor.y = y;

    /* Set register */
    *evid_crsr_column = evid_region.origin.x + x;
    *evid_crsr_row = evid_region.origin.y + y;
}

/**
 * Get the position of the cursor (x, y) relative to the current region
 *
 * @param screen the number of the text device
 * @param position pointer to a t_point record to fill out
 */
static void txt_evid_get_xy(p_point position) {
    position->x = evid_cursor.x;
    position->y = evid_cursor.y;
}

/**
 * Print a character to the current cursor position in the current color
 *
 * @param c the character to print
 */
static void txt_evid_put(char c) {
    short x;
    short y;
    unsigned int offset;

    x = evid_region.origin.x + evid_cursor.x;
    y = evid_region.origin.y + evid_cursor.y;
    offset = y * evid_max_size.width + x;
    evid_text_matrix[offset] = c;
    evid_color_matrix[offset] = evid_color;

    txt_evid_set_xy(evid_cursor.x + 1, evid_cursor.y);
}

/**
 * Initialize the screen
 */
static void txt_evid_init() {
    char buffer[255];
    t_rect region;
    int i;
    DEBUG("txt_evid_init");

    evid_resolution.width = 0;
    evid_resolution.height = 0;
    evid_visible_size.width = 0;
    evid_visible_size.height = 0;
    evid_font_size.width = 0;
    evid_font_size.height = 0;

    /* Disable the set_sizes call for now, to avoid computing transcient unnecessary values */
    evid_enable_set_sizes = 0;

    /* Start with nothing on */
    evid_msr_shadow = 0;

    /* Define the capabilities */

    /* Specify the screen number. We have only one so... */
    evid_caps.number = TXT_SCREEN_EVID;

    /* This screen can be text, bitmap or can be put to sleep */
    evid_caps.supported_modes = TXT_MODE_TEXT;

    /* Supported resolutions */
    evid_caps.resolution_count = sizeof(evid_resolutions) / sizeof(t_extent);
    evid_caps.resolutions = evid_resolutions;

    /* Only 8x8 on the U */
    evid_caps.font_size_count = sizeof(evid_fonts) / sizeof(t_extent);
    evid_caps.font_sizes = evid_fonts;

    /* Initialize the color lookup tables */
    for (i = 0; i < sizeof(evid_clut)/sizeof(t_color4); i++) {
		evid_fg_clut[i] = evid_clut[i];
		evid_bg_clut[i] = evid_clut[i];        
    }

    /* Set the mode to text */
    txt_evid_set_mode(TXT_MODE_TEXT);

    /* Set the resolution */
    txt_evid_set_resolution(640, 480);      

    /* Set the default color: light grey on blue */
    txt_evid_set_color(0x07, 0x04);

    /* Set the font */
    txt_evid_set_font(8, 8, MSX_CP437_8x8_bin);             /* Use 8x8 font */

    /* Set the cursor */
    txt_evid_set_cursor(1, 0, 0xB1);

    /* Set the border */
    txt_evid_set_border(16, 16);                            /* Set up the border */
    txt_evid_set_border_color(0, 0, 0x3f);

    /*
     * Enable set_sizes, now that everything is set up initially
     * And calculate the size of the screen
     */
    evid_enable_set_sizes = 1;
    txt_evid_set_sizes();

    /* Set region to default */
    region.origin.x = 0;
    region.origin.y = 0;
    region.size.width = 0;
    region.size.height = 0;
    txt_evid_set_region(&region);

    /* Home the cursor */
    txt_evid_set_xy(0, 0);

    /* Clear the screen */
    txt_evid_fill(' ');
}

static void txt_evid_test() {
    const char * message = "Hello, EVID!";

    txt_evid_init();
    
    for (const char *x = message; *x; x++) {
        txt_evid_put(*x);
    }
}

// static     t_txt_device device;

/**
 * Initialize and install the driver
 *
 * @return 0 on success, any other number is an error
 */
short txt_evid_install() {
    if (1) { // txt_evid_present()) {
        // t_txt_device device;

        // device.number = TXT_SCREEN_EVID;
        // device.name = "EVID";

        // device.init = txt_evid_init;
        // device.get_capabilities = txt_evid_get_capabilities;
        // device.set_mode = txt_evid_set_mode;
        // device.set_sizes = txt_evid_set_sizes;
        // device.set_resolution = txt_evid_set_resolution;
        // device.set_border = txt_evid_set_border;
        // device.set_border_color = txt_evid_set_border_color;
        // device.set_font = txt_evid_set_font;
        // device.set_cursor = txt_evid_set_cursor;
        // device.set_cursor_visible = txt_evid_set_cursor_visible;
        // device.get_region = txt_evid_get_region;
        // device.set_region = txt_evid_set_region;
        // device.get_color = txt_evid_get_color;
        // device.set_color = txt_evid_set_color;
        // device.set_xy = txt_evid_set_xy;
        // device.get_xy = txt_evid_get_xy;
        // device.put = txt_evid_put;
        // device.scroll = txt_evid_scroll;
        // device.fill = txt_evid_fill;
        // device.get_sizes = txt_evid_get_sizes;

        // Card is present... for the moment, just run the test
        // Normally, we'd install the driver here.
        txt_evid_test();
        return 0;

//     return txt_register(&device);
    } else {
        // Don't install if the card isn't present, but don't treat it as an error
        return 0;
    }
}
