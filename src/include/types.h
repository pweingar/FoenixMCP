/*
 * Definitions of types used by the kernel
 */

#ifndef __TYPES_H
#define __TYPES_H

#include <stdint.h>

/*
 * Function types
 */

/*
 * Integer types in their standard sizes, signed and unsigned.
 */

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

/*
 * Function types
 */

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
