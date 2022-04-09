/*
 * Definitions of types used by the kernel
 */

#ifndef __TYPES_H
#define __TYPES_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @struct s_extent
 *
 * An extent or size of a rectangular area
 */
typedef struct s_extent {
    short width;        /**< The width of the region */
    short height;       /**< The height of the region */
} t_extent, *p_extent;

/**
 * @struct s_point
 *
 * A point on a plane
 */
typedef struct s_point {
    short x;                /**< The column of the point */
    short y;                /**< The row of the point */
} t_point, *p_point;

/**
 * @struct s_rect
 *
 * A rectangle on the screen
 */
typedef struct s_rect {
    t_point origin;         /**< The upper-left corner of the rectangle */
    t_extent size;          /**< The size of the rectangle */
} t_rect, *p_rect;

//
// A color (BGR)
//
typedef struct s_color3 {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} t_color3;

//
// A color entry for a color lookup table (BGRA)
//
typedef struct s_color4 {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} t_color4;

/*
 * Function types
 */

typedef short (*FUNC_V_2_V)();
typedef short (*FUNC_V_2_S)();
typedef short (*FUNC_S_2_S)(char *);
typedef short (*FUNC_BS_2_S)(unsigned char *, short);
typedef short (*FUNC_cBS_2_S)(const unsigned char *, short);
typedef short (*FUNC_B_2_S)(const unsigned short);
typedef short (*FUNC_LBS_2_S)(long, unsigned char *, short);
typedef short (*FUNC_LcBS_2_S)(long, const unsigned char *, short);
typedef short (*FUNC_SBS_2_S)(short, unsigned char *, short);
typedef short (*FUNC_LB_2_S)(long, short);

#endif
