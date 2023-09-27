#ifndef __PS2_C256_H
#define __PS2_C256_H

/*
 * Ports for the PS/2 keyboard and mouse on the C256
 */

#define PS2_STATUS      ((unsigned char *)0xaf1064)
#define PS2_CMD_BUF     ((unsigned char *)0xaf1064)
#define PS2_OUT_BUF     ((unsigned char *)0xaf1060)
#define PS2_INPT_BUF    ((unsigned char *)0xaf1060)
#define PS2_DATA_BUF    ((unsigned char *)0xaf1060)

#endif
