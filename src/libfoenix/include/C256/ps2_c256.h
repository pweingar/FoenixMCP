#ifndef __PS2_C256_H
#define __PS2_C256_H

/*
 * Ports for the PS/2 keyboard and mouse on the C256
 */

 #if MODEL == MODEL_FOENIX_FMX

#define PS2_STATUS      ((volatile __attribute__((far)) char *)0xaf1064)
#define PS2_CMD_BUF     ((volatile __attribute__((far)) char *)0xaf1064)
#define PS2_OUT_BUF     ((volatile __attribute__((far)) char *)0xaf1060)
#define PS2_INPT_BUF    ((volatile __attribute__((far)) char *)0xaf1060)
#define PS2_DATA_BUF    ((volatile __attribute__((far)) char *)0xaf1060)

#else

#define PS2_STATUS      ((volatile __attribute__((far)) char *)0xaf1807)
#define PS2_CMD_BUF     ((volatile __attribute__((far)) char *)0xaf1807)
#define PS2_OUT_BUF     ((volatile __attribute__((far)) char *)0xaf1803)
#define PS2_INPT_BUF    ((volatile __attribute__((far)) char *)0xaf1803)
#define PS2_DATA_BUF    ((volatile __attribute__((far)) char *)0xaf1803)

#endif

#endif
