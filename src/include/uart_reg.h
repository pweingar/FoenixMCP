/*
 * Definitions of the locations of the UART registers
 */

#ifndef __UART_REG_H
#define __UART_REG_H

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#define UART1_BASE              0x00C023F8  /* Base address for UART 1 (COM1) */
#define UART2_BASE              0x00C022F8  /* Base address for UART 2 (COM2) */

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#define UART1_BASE              0x00B028F8  /* Base address for UART 1 (COM1) */
#define UART2_BASE              0x00B028F9  /* Base address for UART 2 (COM2) */

#endif

/*
 * Register Offsets
 */

#define UART_TRHB               0x00        /* Transmit/Receive Hold Buffer */
#define UART_DLL                UART_TRHB   /* Divisor Latch Low Byte */
#define UART_DLH                0x01        /* Divisor Latch High Byte */
#define UART_IER                UART_DLH    /* Interupt Enable Register */
#define UART_FCR                0x02        /* FIFO Control Register */
#define UART_IIR                UART_FCR    /* Interupt Indentification Register */
#define UART_LCR                0x03        /* Line Control Register */
#define UART_MCR                0x04        /* Modem Control REgister */
#define UART_LSR                0x05        /* Line Status Register */
#define UART_MSR                0x06        /* Modem Status Register */
#define UART_SR                 0x07        /* Scratch Register */

/* Interupt Enable Flags */
#define UINT_LOW_POWER          0x20        /* Enable Low Power Mode (16750) */
#define UINT_SLEEP_MODE         0x10        /* Enable Sleep Mode (16750) */
#define UINT_MODEM_STATUS       0x08        /* Enable Modem Status Interrupt */
#define UINT_LINE_STATUS        0x04        /* Enable Receiver Line Status Interupt */
#define UINT_THR_EMPTY          0x02        /* Enable Transmit Holding Register Empty interrupt */
#define UINT_DATA_AVAIL         0x01        /* Enable Recieve Data Available interupt */

/* Interrupt Identification Register Codes */
#define IIR_FIFO_ENABLED        0x80        /* FIFO is enabled */
#define IIR_FIFO_NONFUNC        0x40        /* FIFO is not functioning */
#define IIR_FIFO_64BYTE         0x20        /* 64 byte FIFO enabled (16750) */
#define IIR_MODEM_STATUS        0x00        /* Modem Status Interrupt */
#define IIR_THR_EMPTY           0x02        /* Transmit Holding Register Empty Interrupt */
#define IIR_DATA_AVAIL          0x04        /* Data Available Interrupt */
#define IIR_LINE_STATUS         0x06        /* Line Status Interrupt */
#define IIR_TIMEOUT             0x0C        /* Time-out Interrupt (16550 and later) */
#define IIR_INTERRUPT_PENDING   0x01        /* Interrupt Pending Flag */

/* Line Control Register Codes */
#define LCR_DLB                 0x80        /* Divisor Latch Access Bit */
#define LCR_SBE                 0x60        /* Set Break Enable */

#define LCR_PARITY_NONE         0x00        /* Parity: None */
#define LCR_PARITY_ODD          0x08        /* Parity: Odd */
#define LCR_PARITY_EVEN         0x18        /* Parity: Even */
#define LCR_PARITY_MARK         0x28        /* Parity: Mark */
#define LCR_PARITY_SPACE        0x38        /* Parity: Space */

#define LCR_STOPBIT_1           0x00        /* One Stop Bit */
#define LCR_STOPBIT_2           0x04        /* 1.5 or 2 Stop Bits */

#define LCR_DATABITS_5          0x00        /* Data Bits: 5 */
#define LCR_DATABITS_6          0x01        /* Data Bits: 6 */
#define LCR_DATABITS_7          0x02        /* Data Bits: 7 */
#define LCR_DATABITS_8          0x03        /* Data Bits: 8 */

#define LSR_ERR_RECIEVE         0x80        /* Error in Received FIFO */
#define LSR_XMIT_DONE           0x40        /* All data has been transmitted */
#define LSR_XMIT_EMPTY          0x20        /* Empty transmit holding register */
#define LSR_BREAK_INT           0x10        /* Break interrupt */
#define LSR_ERR_FRAME           0x08        /* Framing error */
#define LSR_ERR_PARITY          0x04        /* Parity error */
#define LSR_ERR_OVERRUN         0x02        /* Overrun error */
#define LSR_DATA_AVAIL          0x01        /* Data is ready in the receive buffer */

#if MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS

#define UART_300                4167        /* Code for 300 bps */
#define UART_1200               1042        /* Code for 1200 bps */
#define UART_2400               521         /* Code for 2400 bps */
#define UART_4800               260         /* Code for 4800 bps */
#define UART_9600               130         /* Code for 9600 bps */
#define UART_19200              65          /* Code for 19200 bps */
#define UART_38400              33          /* Code for 28400 bps */
#define UART_57600              22          /* Code for 57600 bps */
#define UART_115200             11          /* Code for 115200 bps */

#else

#define UART_300                384         /* Code for 300 bps */
#define UART_1200               96          /* Code for 1200 bps */
#define UART_2400               48          /* Code for 2400 bps */
#define UART_4800               24          /* Code for 4800 bps */
#define UART_9600               12          /* Code for 9600 bps */
#define UART_19200              6           /* Code for 19200 bps */
#define UART_38400              3           /* Code for 28400 bps */
#define UART_57600              2           /* Code for 57600 bps */
#define UART_115200             1           /* Code for 115200 bps */

#endif



#endif
