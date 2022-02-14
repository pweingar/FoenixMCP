/*
 * Text screen driver framework
 */

#ifndef __TEXT_DEVICE_H
#define __TEXT_DEVICE_H

#include "types.h"

/**
 * Flags to indicate which display capabilities are available and active
 */
const short MODE_TEXT = 0x01;       // Display text
const short MODE_BITMAP = 0x02;     // Display a bitmap
const short MODE_SPRITE = 0x04;     // Display sprites
const short MODE_TILE = 0x08;       // Display tilesets
const short MODE_BLANK = 0x10;      // Display a blank screen
const short MODE_DISABLE = 0x20;    // Disable graphics engine

/**
 * Structure representing the capabilities of the display
 */
typedef struct s_disp_caps {
    short modes;                    // Flags indicating which display modes are supported
    const p_rect resolutions;       // Array of rectangles listing supported screen resolutions (in pixels)
    const p_rect fonts;             // Array of rectangles listing supported font sizes (in pixels)
} t_disp_caps, *p_disp_caps;

/**
 * Driver function types
 */
typedef short (*p_txt_init)();
typedef p_disp_caps (*p_txt_get_caps)();
typedef void (*p_txt_set_mode)(short);
typedef void (*p_txt_set_res)(short, short);
typedef short (*p_txt_set_font)(p_rect, p_rect, char *);
typedef void (*p_txt_set_border)(short, short, short, p_color3);
typedef void (*p_txt_set_cursor)(short, char, short, short, short);
typedef void (*p_txt_set_xy)(short, short);
typedef p_rect (*p_txt_set_sizes)();
typedef void (*p_txt_put)(char, short, short);
typedef void (*p_txt_move)(p_rect, p_rect);
typedef void (*p_txt_fill)(char, short, short, p_rect);

/**
 * Structure representing a low level text mode screen. This is essentially a text mode driver for a screen
 *
 * I anticipate that there will be several such drivers:
 * C256 -- A driver for the main screen, and a driver for the EVID
 * A2560U -- A single driver for its main screen. If an EVID ever comes out for it, there will be another for that
 * A2560K -- A driver for channel A, and a driver for channel B
 * A2560 GenX -- Probably similar to A2560K... might use the same drivers, if possible
 *
 * All coordinates and sizes used in the driver will be in native rows and columns of character cells
 * with the exception of the resolution and font size information, which will be in pixels.
 * Region support in the general text layer will be provided by the general code, not the driver
 */
typedef struct s_txt_device {
    short number;                   // Number of the text device
    const char * name;              // Human readable name for the text device

    short foreground;               // The current foreground color
    short background;               // The current background color

    t_rect clip_region;             // The current clipping region

    p_txt_init init;                // short init() -- Initialize the device
    p_txt_get_caps get_caps         // p_disp_caps get_caps() -- Gets the capabilities of the display
    p_txt_set_mode set_mode;        // void set_mode(short flags) -- Set the graphics modes
    p_txt_set_res set_resolution;   // void set_resolution(short width, short height) -- Set the display resolution
    p_txt_set_font set_font;        // short set_font(p_rect char_size, p_rect container_size, unsigned char * data) -- Set the font
    p_txt_set_border set_border;    // void set_border(short visible, short width, short height, p_color3 color) -- Set the border
    p_txt_set_cursor set_cursor;    // void set_cursor(short visible, char c, short foreground, short background, short rate) -- Set the cursor
    p_txt_set_xy set_xy;            // void set_xy(short column, short row) -- Set the cursor's position
    p_txt_set_sizes set_sizes;      // const p_rect set_sizes() -- Compute and return the text screen size
    p_txt_put put;                  // void put(char c, short foreground, short background) -- Put character and color at position
    p_txt_move move;                // void move(p_rect from, p_rect to) -- Move character and color data
    p_txt_fill fill;                // void fill(char c, short foreground, short background, p_rect to) -- Fill a rectangle with a character and color data
} t_txt_device, *p_txt_device;

/*
 * Initialize the display system
 */
extern void txt_init();

/*
 * Install the device driver for a text display
 *
 * Inputs:
 * driver = pointer to the t_txt_device record for the text display
 *
 * Returns:
 * 0 on success, any other number is an error code
 */
extern short txt_install(p_txt_device driver);

/*
 * Get the list of supported display resolutions
 *
 * Inputs:
 * display = the number of the display
 *
 * Returns:
 * a poshorter to a table describing the supported resolutions (w, h) in pixels of the display
 * A sentinel entry with w = h = 0 marks the end of the list.
 */
extern const p_rect txt_get_resolutions(short display);

/*
 * Set the base resolution of the display.
 * It is an error if the requested resolution is not supported.
 *
 * Inputs:
 * display = the number of the display
 * width = the number of pixels across
 * height = the number of pixels down
 *
 * Returns:
 * 0 on success, any other number means the requested resolution is not supported
 */
extern short txt_set_resolution(short display, short width, short height);

/*
 * Get the list of supported font sizes
 *
 * Inputs:
 * display = the number of the display
 *
 * Returns:
 * a poshorter to a table describing the supported font sizes (w, h) in pixels
 * A sentinel entry with w = h = 0 marks the end of the list.
 */
extern const p_rect txt_get_font_sizes(short display);

/*
 * Set the font (size and bitmap) for display
 * It is an error if the font size is not supported.
 *
 * Inputs:
 * display = the number of the display
 * width = the number of pixels across per character
 * height = the number of pixels down per character
 * data = poshorter to the raw pixel data for the font
 *
 * Returns:
 * 0 on success, any other number means the requested resolution is not supported
 */
extern short txt_set_font(short display, short width, short height, unsigned char * data);

/*
 * Set the border of the display
 *
 * Inputs:
 * display = the number of the display
 * visible = 0 for disabled, 1 for enabled (visible)
 * horizontal = the width of the border in pixels
 * vertical = the height of the border in pixels
 * color = pointer to the color structure specifying the color of the border
 */
extern void txt_set_border(short display, short visible, short horizontal, short vertical, p_color3 color);

/*
 * Set the display region to limit all character operations
 * Changes to the characters, scrolling, deleting, cursor positions, etc.
 * will be limited to the rectangular region provided.
 *
 * Setting a width or height of 0 in the region will reset the region to the
 * full screen, which is the default.
 *
 * Inputs:
 * display = the number of the display
 * region = poshorter to rectangle describing the region (in character positions)
 */
extern void txt_set_region(short display, p_rect region);

/*
 * Set the color of the text for future puts
 *
 * Inputs:
 * display = the number of the display
 * foreground = the color of the character
 * background = the color of the background
 */
extern void txt_set_colors(short display, short foreground, short background);

/*
 * Setup the text cursor
 *
 * Inputs:
 * display = the number of the display
 * c = the character to display for the cursor
 * visible = 0 for invisible, any other number for visible
 * foreground = the color of the cursor foreground
 * background = the color of the cursor background
 * rate = the flash rate for the cursor
 */
extern void txt_set_cursor(short display, short visible, char c, short foreground, short background, short rate);

/*
 * Recompute shorternal size information about the display based on the hardware settings.
 *
 * This call allows the user program to alter VICKY registers itself and still allow the kernel text drivers
 * to display data correctly.
 *
 * Inputs:
 * display = the number of the display
 */
extern void txt_set_sizes(short display);

/*
 * Set the position of the cursor within the current region
 *
 * Inputs:
 * display = the number of the display
 * column = the horizontal position of the cursor (within the region)
 * row = the vertical position of the cursor (within the region)
 */
extern void txt_set_position(short display, short column, short row);

/*
 * Put a character on the screen at the cursor position
 *
 * Inputs:
 * display = the number of the display
 * c = the character to put on the display
 */
extern void txt_put(short display, char c);

/*
 * Clear all or part of the line the cursor is on (within the current region)
 *
 * Inputs:
 * display = the number of the display
 * mode = 2 = all, 1 = from cursor to end of line, 0 = from beginning of line to cursor
 */
extern void txt_clear_line(short display, short mode);

/*
 * Clear all or part of the region
 *
 * Inputs:
 * display = the number of the display
 * mode = 2 = all, 1 = from cursor to end of region, 0 = from beginning of region to cursor
 */
extern void txt_clear_region(short display, short mode);

/*
 * Insert a number of blanks at the cursor position
 *
 * Inputs:
 * display = the number of the display
 * n = number of blanks to insert
 */
extern void txt_insert(short display, short n);

/*
 * Delete a number of blanks at the cursor position
 *
 * Inputs:
 * display = the number of the display
 * n = number of blanks to delete
 */
extern void txt_delete(short display, short n);

/*
 * Scroll the current region horizontally or vertically
 *
 * Inputs:
 * display = the number of the display
 * horizontal = columns to scroll left (negative) or right (positive)
 * vertical = rows to scroll up (negative) or right (positive)
 */
extern void txt_scroll(short display, short horizontal, short vertical);

#endif
