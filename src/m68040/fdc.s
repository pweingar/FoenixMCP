;;;
;;; Assembly code for direct floppy drive support for the A2560K and Gen X
;;;

                xdef _fdc_cmd_asm

code

FDC_BASE = $FEC023F0
FDC_DOR = $FEC023F2     ; Read/Write - Digital Output Register
FDC_DOR_DSEL0 = $01     ; Drive 0 Select
FDC_DOR_DSEL1 = $02     ; Drive 1 Select
FDC_DOR_NRESET = $04    ; Reset the FDC
FDC_DOR_DMAEN = $08     ; Enable DMA
FDC_DOR_MOT0  = $10     ; Turn on motor 0
FDC_DOR_MOT1  = $20     ; Turn on motor 1
FDC_DOR_MOT2  = $40     ; Turn on motor 2
FDC_DOR_MOT3  = $80     ; Turn on motor 3

FDC_MSR = $FEC023F4     ; Read - Main Status Register
FDC_MSR_DRV0BSY = $01   ; Indicates if drive 0 is busy
FDC_MSR_DRV1BSY = $02   ; Indicates if drive 1 is busy
FDC_MSR_CMDBSY = $10    ; Indicates if a command is in progress
FDC_MSR_NONDMA = $20    ;
FDC_MSR_DIO = $40       ; Data direction: 1 = read, 0 = write
FDC_MSR_RQM = $80       ; 1 = host can transfer data, 0 = host must wait

FDC_DSR = $FEC023F4     ; Write - Data Select Register
; Bit[0..1] = data rate
; Bit[2..4] = precompensation select
FDC_DSR_LOPWR = $40     ; Turn on low power mode
FDC_DSR_RESET = $80     ; Software reset of the FDC

FDC_DATA = $FEC023F5    ; Read/Write - Data - FIFO

FDC_RSV = $FEC023F6     ; Reserved

FDC_DIR = $FEC023F7     ; Read - Digital Input Register
FDC_DIR_DSKCHG = $80    ; Indicates if the disk has changed

FDC_CCR = $FEC023F7     ; Write - Configuration Control Register
; Bit[0..1] = Data rate

;
; Issue a command to the floppy drive controller
;
; @param command (byte) the command to execute
; @param argc the number of bytes in the arguments
; @param argv (byte array) the bytes to send as arguments to the command
; @param buffer the buffer to fill with any read data
; @param results the buffer to fill with result bytes
_fdc_cmd_asm    link a6,#0                      ; Set up the stack frame
                setfo 20
results         fo.l 1
buffer          fo.l 1
argv            fo.l 1
argc            fo.w 1
command         fo.w 1

                ; Save our registers
                ; TODO: save only those affected
                movem d1-d7/a1-a5,-(a7)

                move.l (buffer,a6),a2           ; Get the pointer to the data buffer

                ; Make sure the FDC will accept data
                move.b FDC_MSR,d0
                and.b #FDC_MSR_RQM|FDC_MSR_DIO,d0
                cmp.b #FDC_MSR_RQM,d0
                bne fdc_restart

                move.b (command,a6),FDC_DATA    ; Send the command

                ; Send the arguments

                move.w (argc,a6),d1             ; Get the argument count into d1
                move.l (argv,a6),a0             ; Get the pointer to the arguments
wait_snd_args   move.b FDC_MSR,d0               ; Get the MSR
                btst.b #7,d0                    ; Is RQM set?
                bne wait_snd_args
                btst.b #6,d0                    ; Verify that DIO = 0
                beq fdc_restart                 ; If not... restart the FDC

                move.b (a0)+,FDC_DATA           ; Send the next argument byte
                subq.w #1,d1                    ; Decrement the count
                bne wait_snd_args               ; If not done, wait for the FDC to be ready and send the next byte

                ; TODO: Receive data bytes
get_data        move.b FDC_MSR,d0               ; Get the MSR
                btst.b #5,d0                    ; Check to see if NONDMA = 1
                bne result_phase                ; No? Skip to the result phase

wait_data       move.b FDC_MSR,d0
                btst.b #7,d0                    ; Is RQM = 1
                bne wait_data                   ; No: wait until it is

                move.b FDC_DATA,(a2)+           ; Move the data byte to the data buffer
                bra get_data                    ; And check to see if there is more data

                ; TODO: Receive result bytes
result_phase    move.l (results,a6),a2          ; Get the pointer to the result buffer

wait_result     move.b FDC_MSR,d0               ; Get the MSR
                btst.b #7,d0                    ; Is RQM set?
                bne wait_result                 ; No: wait until it is
                and.b #FDC_MSR_DIO|FDC_MSR_CMDBSY,d0
                cmp.b #FDC_MSR_DIO|FDC_MSR_CMDBSY,d0
                bne fdc_cmd_exit                ; If not BUSY and ready for READ, we're done

                move.b FDC_DATA,(a2)+           ; Move the data byte to the result buffer
                bra wait_result                 ; And try to fetch another byte

fdc_cmd_exit    ; Restore our registers
                ; TODO: restore what we saved
                movem (a7)+,d1-d7/a1-a5
                unlk a6                         ; Restore our stack frame
                rts
