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
    int16_t width;        /**< The width of the region */
    int16_t height;       /**< The height of the region */
} t_extent, *p_extent;

/**
 * @struct s_point
 *
 * A point on a plane
 */
typedef struct s_point {
    int16_t x;                /**< The column of the point */
    int16_t y;                /**< The row of the point */
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

typedef void  (*FUNC_V_2_V)();
typedef int16_t (*FUNC_V_2_S)();
typedef int16_t (*FUNC_S_2_S)(int8_t *);
typedef int16_t (*FUNC_BS_2_S)(uint8_t *, int16_t);
typedef int16_t (*FUNC_cBS_2_S)(const uint8_t *, int16_t);
typedef int16_t (*FUNC_B_2_S)(const uint16_t);
typedef int16_t (*FUNC_LBS_2_S)(int32_t, uint8_t *, int16_t);
typedef int16_t (*FUNC_LcBS_2_S)(int32_t, const uint8_t *, int16_t);
typedef int16_t (*FUNC_SBS_2_S)(int16_t, uint8_t *, int16_t);
typedef int16_t (*FUNC_LB_2_S)(int32_t, int16_t);

#endif
