/*
 * Definitions of types used by the kernel
 */

#ifndef __TYPES_H
#define __TYPES_H

/*
 * Integer types in their standard sizes, signed and unsigned.
 */

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

typedef unsigned char bool;

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


#endif
