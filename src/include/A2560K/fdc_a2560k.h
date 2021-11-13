/*
 * Define the locations of all the FDC registers
 */

#ifndef __FDC_A2560K_H
#define __FDC_A2560K_H

#define FDC_BASE    ((volatile unsigned char *)0x00C023F0)
#define FDC_DOR     ((volatile unsigned char *)0x00C023F2) /* Read/Write - Digital Output Register */
// FDC_DOR_DSEL0 = $01     ; Drive 0 Select
// FDC_DOR_DSEL1 = $02     ; Drive 1 Select
// FDC_DOR_NRESET = $04    ; Reset the FDC
// FDC_DOR_DMAEN = $08     ; Enable DMA
// FDC_DOR_MOT0  = $10     ; Turn on motor 0
// FDC_DOR_MOT1  = $20     ; Turn on motor 1
// FDC_DOR_MOT2  = $40     ; Turn on motor 2
// FDC_DOR_MOT3  = $80     ; Turn on motor 3

#define FDC_MSR     ((volatile unsigned char *)0x00C023F4) /* Read - Main Status Register */
// FDC_MSR_DRV0BSY = $01   ; Indicates if drive 0 is busy
// FDC_MSR_DRV1BSY = $02   ; Indicates if drive 1 is busy
// FDC_MSR_CMDBSY = $10    ; Indicates if a command is in progress
// FDC_MSR_NONDMA = $20    ;
// FDC_MSR_DIO = $40       ; Data direction: 1 = read, 0 = write
// FDC_MSR_RQM = $80       ; 1 = host can transfer data, 0 = host must wait

#define FDC_DSR     ((volatile unsigned char *)0x00C023F4) /* Write - Data Select Register */
// ; Bit[0..1] = data rate
// ; Bit[2..4] = precompensation select
// FDC_DSR_LOPWR = $40     ; Turn on low power mode
// FDC_DSR_RESET = $80     ; Software reset of the FDC

#define FDC_DATA    ((volatile unsigned char *)0x00C023F5) /* Read/Write - Data - FIFO */

#define FDC_RSV     ((volatile unsigned char *)0x00C023F6) /* Reserved */

#define FDC_DIR     ((volatile unsigned char *)0x00C023F7) /* Read - Digital Input Register */
// FDC_DIR_DSKCHG = $80    ; Indicates if the disk has changed

#define FDC_CCR     ((volatile unsigned char *)0x00C023F7) /* Write - Configuration Control Register */
// ; Bit[0..1] = Data rate

#endif
