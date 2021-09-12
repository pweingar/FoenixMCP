/**
 * Definitions for the Vicky graphics controller
 */

#ifndef __VICKYII_GENERAL_H
#define __VICKYII_GENERAL_H

#define VKY_MASTER_CONTROL_L 0xAF0000
#define Mstr_Ctrl_Text_Mode_En  0x01
#define Mstr_Ctrl_Text_Overlay  0x02
#define Mstr_Ctrl_Graph_Mode_En 0x04
#define Mstr_Ctrl_Bitmap_En     0x08
#define Mstr_Ctrl_TileMap_En    0x10
#define Mstr_Ctrl_Sprite_En     0x20
#define Mstr_Ctrl_GAMMA_En      0x40
#define Mstr_Ctrl_Disable_Vid   0x80

#define VKY_MASTER_CONTROL_H 0xAF0001
#define VKY_RESOLUTION_640_480 0
#define VKY_RESOLUTION_320_240 2
#define VKY_RESOLUTION_800_600 1
#define VKY_RESOLUTION_400_300 3

#define BORDER_CTRL_REG         0xAF0004
#define Border_Ctrl_Enable      0x01
#define BORDER_COLOR_B          0xAF0005
#define BORDER_COLOR_G          0xAF0006
#define BORDER_COLOR_R          0xAF0007
#define BORDER_X_SIZE           0xAF0008    // X-  Values: 0 - 32 (Default: 32)
#define BORDER_Y_SIZE           0xAF0009    // Y- Values 0 - 32 (Default: 32)

#define BACKGROUND_COLOR_B      0xAF000D    // When in Graphic Mode, if a pixel is "0" then the Background pixel is chosen
#define BACKGROUND_COLOR_G      0xAF000E
#define BACKGROUND_COLOR_R      0xAF000F

#define VKY_TXT_CURSOR_CTRL_REG 0xAF0010    // [0]  Enable Text Mode
#define Vky_Cursor_Enable       0x01
#define Vky_Cursor_Flash_Rate0  0x02        // 00 - 1/Sec, 01 - 2/Sec, 10 - 4/Sec, 11 - 5/Sec
#define Vky_Cursor_Flash_Rate1  0x04
#define Vky_Cursor_FONT_Page0   0x08        // Pick Font Page 0 or Font Page 1
#define Vky_Cursor_FONT_Page1   0x10        // Pick Font Page 0 or Font Page 1
#define VKY_TXT_START_ADD_PTR   0xAF0011    // This is an offset to change the Starting address of the Text Mode Buffer (in x)
#define VKY_TXT_CURSOR_CHAR_REG 0xAF0012

#define VKY_TXT_CURSOR_COLR_REG 0xAF0013
#define VKY_TXT_CURSOR_X_REG_L  0xAF0014
#define VKY_TXT_CURSOR_X_REG_H  0xAF0015
#define VKY_TXT_CURSOR_Y_REG_L  0xAF0016
#define VKY_TXT_CURSOR_Y_REG_H  0xAF0017

#define MAX_TEXT_COLORS         15          // The maximum number of colors in the text LUTs
#define FG_CHAR_LUT_PTR         0xAF1F40    // Text screen foreground color LUT
#define BG_CHAR_LUT_PTR         0xAF1F80    // Text screen background color LUT

#define FONT_MEMORY_BANK0       0xAF8000    // The memory area for the font

#define CS_TEXT_MEM_PTR         0xAFA000    // The text cell matrix
#define CS_COLOR_MEM_PTR        0xAFC000    // The color cell matrix

#endif