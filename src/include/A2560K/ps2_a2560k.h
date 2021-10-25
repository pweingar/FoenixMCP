#ifndef __PS2_A2560K_H
#define __PS2_A2560K_H

/*
 * Ports for the PS/2 keyboard and mouse on the A2560K
 */

#define PS2_STATUS      ((unsigned char *)0x00C02064)
#define PS2_CMD_BUF     ((unsigned char *)0x00C02064)
#define PS2_OUT_BUF     ((unsigned char *)0x00C02060)
#define PS2_INPT_BUF    ((unsigned char *)0x00C02060)
#define PS2_DATA_BUF    ((unsigned char *)0x00C02060)

#endif
