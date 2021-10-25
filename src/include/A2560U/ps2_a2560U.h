#ifndef __PS2_A2560U_H
#define __PS2_A2560U_H

/*
 * Ports for the PS/2 keyboard and mouse on the A2560U and A2560U+
 */

#define PS2_STATUS      ((unsigned char *)0x00B02804)
#define PS2_CMD_BUF     ((unsigned char *)0x00B02804)
#define PS2_OUT_BUF     ((unsigned char *)0x00B02800)
#define PS2_INPT_BUF    ((unsigned char *)0x00B02800)
#define PS2_DATA_BUF    ((unsigned char *)0x00B02800)

#endif
