/**
 * @file c256_vicky_ii.h
 * @author Peter Weingartner (pjw@tailrecursive.org)
 * @brief Define the registers for Vicky II on the C256 computers.
 * @version 0.1
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __c256_vicky_ii__
#define __c256_vicky_ii__

#include "types.h"
#include <stdint.h>

/**
 * @brief Vicky II Master Control Register
 * 
 */
typedef struct tvky_mstr_ctrl_s {
    union {   
        struct {
            int text_enable:1;
            int text_overlay:1;
            int graphics_enable:1;
            int bitmap_enable:1;
            int tile_enable:1;
            int sprite_enable:1;
            int gamma_enable:1;
            int disable:1;
            int mode:2;
        };
        uint16_t raw; 
    };
} *tvky_mstr_ctrl_p;

#define VKY_MCR_TEXT            0x0001
#define VKY_MCR_TEXT_OVERLAY    0x0002
#define VKY_MCR_GRAPHICS        0x0004
#define VKY_MCR_BITMAP          0x0008
#define VKY_MCR_TILE            0x0010
#define VKY_MCR_SPRITE          0x0020
#define VKY_MCR_GAMMA           0x0040
#define VKY_MCR_DISABLE         0x0080
#define VKY_MCR_RES_MASK        0x0300
#define VKY_MCR_RES_320x240     0x0200
#define VKY_MCR_RES_640x480     0x0000
#define VKY_MCR_RES_400x300     0x0300
#define VKY_MCR_RES_800x600     0x0100

/**
 * @brief Vicky II Border Control Registers
 * 
 */
typedef struct tvky_border_ctrl_s {
    uint8_t control;
    t_color3 color;
    uint8_t size_x;
    uint8_t sizy_y;
} *tvky_border_ctrl_p;

/**
 * @brief Vicky II Cursor Control Registers
 * 
 */
typedef struct tvky_crsr_ctrl_s {
    uint8_t control;
	uint8_t start_address;
    char character;
    uint8_t color;
    uint16_t column;
    uint16_t row;
} *tvky_crsr_ctrl_p;

//
// Define the locations of the registers
//

#define tvky_mstr_ctrl ((volatile __attribute__((far)) tvky_mstr_ctrl_p)0xaf0000)
#define tvky_brdr_ctrl ((volatile __attribute__((far)) tvky_border_ctrl_p)0xaf0004)
#define tvky_bg_color ((volatile __attribute__((far)) t_color3 *)0xaf000d)
#define tvky_crsr_ctrl ((volatile __attribute__((far)) tvky_crsr_ctrl_p)0xaf0010)

//
// Text Color Lookup Tables
//
#define tvky_text_fg_color ((volatile __attribute__((far)) t_color4 *)0xaf1f40)
#define tvky_text_bg_color ((volatile __attribute__((far)) t_color4 *)0xaf1f80)

//
// Text Fonts Sets 0 and 1
//
#define tvky_font_set_0 	((volatile __attribute__((far)) uint8_t *)0xaf8000)

//
// Graphics Color Lookup Tables
//
#define VKY_GR_CLUT_0		((volatile __attribute__((far)) uint8_t *)0xaf2000)
#define VKY_GR_CLUT_1		((volatile __attribute__((far)) uint8_t *)0xaf2400)
#define VKY_GR_CLUT_2		((volatile __attribute__((far)) uint8_t *)0xaf2800)
#define VKY_GR_CLUT_3		((volatile __attribute__((far)) uint8_t *)0xaf2c00)

//
// Text mode text and color matrixes
//
#define tvky_text_matrix    ((volatile __attribute__((far)) char *)0xafa000)
#define tvky_color_matrix   ((volatile __attribute__((far)) char *)0xafc000)

//
// Bitmap graphics registers
//

typedef volatile __attribute__((far24)) uint8_t *p_far24;

#define bm0_control         ((volatile __attribute__((far)) uint8_t *)0xaf0100)
#define bm0_address         ((volatile __attribute__((far)) uint8_t *)0xaf0101)
#define bm0_offset_x		((volatile __attribute__((far)) uint16_t *)0xaf0104)
#define bm0_offset_y		((volatile __attribute__((far)) uint16_t *)0xaf0106)

#define bm1_control         ((volatile __attribute__((far)) uint8_t *)0xaf0108)
#define bm1_address         ((volatile __attribute__((far)) uint8_t *)0xaf0109)
#define bm1_offset_x		((volatile __attribute__((far)) uint16_t *)0xaf010c)
#define bm1_offset_y		((volatile __attribute__((far)) uint16_t *)0xaf010d)

#define MousePointer_Mem_A	((volatile uint8_t *)0xaf0500)
#define MousePtr_A_CTRL_Reg	((volatile uint16_t *)0xaf0700)
#define MousePtr_En         0x0001

#define MousePtr_A_X_Pos	((volatile uint16_t *)0xaf0702)
#define MousePtr_A_Y_Pos	((volatile uint16_t *)0xaf0704)
#define MousePtr_A_Mouse0	((volatile uint8_t *)0xaf0706)
#define MousePtr_A_Mouse1	((volatile uint8_t *)0xaf0707)
#define MousePtr_A_Mouse2	((volatile uint8_t *)0xaf0708)

//
// Video RAM
//

#define vram_base           ((volatile p_far24)0xb00000)

#endif
