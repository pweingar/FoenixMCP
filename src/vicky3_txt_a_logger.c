// This is standalone and raw logger for the A2560X/K/GenX channel A
// It is meant to be standalone and pretty raw so to minimize chances of failing, as it is used for debugging.

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_A2560X || MODEL == MODEL_FOENIX_GENX

#include <stdint.h>
#include <string.h>

#define VICKY3_A_BASE ((uint8_t*)0xfec40000L)

#include "rsrc/font/BM437_IBM_Model3_Alt4.h"


struct vicky3_t {
    uint32_t control;
    uint32_t border_control;
    uint32_t border_color;
    uint32_t background_color;
    uint32_t cursor_control;
    uint32_t cursor_position;
    uint32_t line_interrupt_01;
    uint32_t line_interrupt_23;
    uint32_t font_mgr_0;
    uint32_t font_mgr_1;
    uint32_t reserved_0028[6];
    uint32_t mouse_graphics[16*16*2];
    uint16_t mouse_control; // Must write as 32bits !
    uint16_t reserved_c02;
    uint32_t mouse_position; // Read only
    uint16_t reserved_c06;
    uint32_t ps2_byte[3]; // Must write as 32 bits. See doc.
};

struct vicky3_text_memory_t {
    char     display[0x4000];
    uint8_t  notmapped_4000[0x4000];
    uint8_t  color[0x4000];
    uint8_t  nomapped_c000[0x400];
    uint32_t palette_fg[16];
    uint32_t palette_bg[16];
};

static volatile struct vicky3_t * const vicky3_a_ctrl = (struct vicky3_t*)VICKY3_A_BASE;
static uint8_t * const font_memory = VICKY3_A_BASE + 0x8000;
static struct vicky3_text_memory_t * const text_memory = (struct vicky3_text_memory_t*)(VICKY3_A_BASE + 0x20000);

static void scroll();
void channel_A_cls(void);

void channel_A_logger_init(void)
{
    vicky3_a_ctrl->control = 1; // Text enable, enable video, 800x600, no gamma
    vicky3_a_ctrl->border_color = 0; // No border
    vicky3_a_ctrl->background_color = 0x00110033UL; // xxRGB
    vicky3_a_ctrl->cursor_control =  0x027F0007UL;

    // Load font
    for (int i=0; i<0x1000; i++)
        font_memory[i] = BM437_IBM_Model3_Alt4[i];

    // Setup colors
    text_memory->palette_fg[0] = vicky3_a_ctrl->background_color;
    text_memory->palette_fg[1] = 0x00ffffff; // White
    text_memory->palette_fg[2] = 0x00ff0000; // Used for cursor color

    channel_A_cls();
}

#define COL_COUNT (800/8)
#define LINE_COUNT (600/8)


void channel_A_cls(void)
{
    // Clear screen
    for (int i=0; i<0x4000; i++)
    {
        text_memory->display[i] = 0;
        text_memory->color[i] = 0x10; // Foreground: color 1, background: color 0 (ie transparent color);
    }
}

void channel_A_write(const char *c)
{
    uint32_t x,y;

    x = y = vicky3_a_ctrl->cursor_position;
    x &= 0xffff;
    y = (y >> 16);
    
    while (c && *c)
    {
        if (*c == '\r')
        {
            x = 0;
        }
        else if (*c == '\n')
        {
            if (y == LINE_COUNT-1)
                scroll();
            else
                y++;
            x = 0; // Assume we intend that \n = \r\n
        }
        else
        {
            text_memory->display[COL_COUNT*y + x] = *c;

            if (++x == COL_COUNT)
                channel_A_write("\n");
        }

        vicky3_a_ctrl->cursor_position = (y << 16) | x;

        c++;
    }
}


static void scroll()
{
#define COPY_BYTE_BY_BYTE
#ifdef COPY_BYTE_BY_BYTE
    char *src = &text_memory->display[1*COL_COUNT];
    char *dst = &text_memory->display[0];
    for (int i=0 ; i<((LINE_COUNT-1)*COL_COUNT)/4; i++)
    {
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
    }
    for (int i=0; i < COL_COUNT/4; i++)
    {
        *dst++ = ' ';
        *dst++ = ' ';
        *dst++ = ' ';
        *dst++ = ' ';
    }
#else
    // This doesn't work because it uses 16/32 byte writes
    memcpy(&text_memory->display[COL_COUNT], &text_memory->display[0], COL_COUNT*LINE_COUNT-1);
    memset(&text_memory->display[COL_COUNT*LINE_COUNT-1], COL_COUNT, 0); // VBCC doesn't know bzero
#endif    
    // If we used background colours we would have to scroll them as well, but we don't.
}

#endif