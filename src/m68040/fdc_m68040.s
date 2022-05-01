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
; short fdc_cmd_asm(short cmd, short argc, uint8_t * argv, uint8_t * buffer, short resultc, uint8_t * results);
;
; @param command (byte) the command to execute (d1)
; @param argc the number of bytes in the arguments (d2)
; @param argv (byte array) the bytes to send as arguments to the command (a1)
; @param buffer the buffer to fill with any read data (a2)
; @param resultc the count of result bytes expected
; @param results the buffer to fill with result bytes (a3)
_fdc_cmd_asm    link a6,#0                      ; Set up the stack frame

                ; Save our registers
                ; TODO: save only those affected
                movem d4-d7/a4-a5,-(a7)

                ; Clear the buffer

                move.w #$0,d0
clr_buffer      move.b #$55,(a2,d0)
                addq.w #1,d0
                cmp.w #512,d0
                bne clr_buffer

                ; Clear the results

                move.w #$0,d0
clr_results     move.b #$AA,(a3,d0)
                addq.w #1,d0
                cmp.w #10,d0
                bne clr_results

                ; Make sure the FDC will accept data
                move.b FDC_MSR,d0
                and.b #FDC_MSR_RQM|FDC_MSR_DIO,d0
                cmp.b #FDC_MSR_RQM,d0
                beq snd_cmd
                move.l #$FFFFFFFF,d0            ; Return -1
                bra fdc_cmd_exit

snd_cmd         move.b d1,FDC_DATA              ; Send the command

                ; Send the arguments

                move.w #0,d4                    ; Counter = 0
wait_snd_args   move.b FDC_MSR,d0               ; Get the MSR
                btst #7,d0                      ; Is RQM set?
                beq wait_snd_args
                btst #6,d0                      ; Verify that DIO = 0
                beq snd_arg
                move.w #$FFFFFFFE,d0            ; Return -2
                bra fdc_cmd_exit

snd_arg         move.b (a1,d4),FDC_DATA         ; Send the next argument byte
                addq.w #1,d4
                cmp.w d2,d4
                bne wait_snd_args               ; If not done, wait for the FDC to be ready and send the next byte

                ; Receive data bytes

                move.w #0,d4                    ; Counter = 0
get_data        ; TODO: should be checking NONDMA here... not sure why that fails here

wait_data       move.b FDC_MSR,d0
                btst #7,d0                      ; Is RQM = 1
                beq wait_data                   ; No: wait until it is

                move.b FDC_DATA,(a2,d4)         ; Move the data byte to the data buffer
                addq.w #1,d4
                cmp.l #512,d4
                bne get_data                    ; And check to see if there is more data

                ; Receive result bytes

result_phase    move.w #0,d4                    ; Counter = 0
wait_result     move.b FDC_MSR,d0               ; Get the MSR
                btst #7,d0                      ; Is RQM set?
                beq wait_result                 ; No: wait until it is
                and.b #FDC_MSR_DIO|FDC_MSR_CMDBSY,d0
                cmp.b #FDC_MSR_DIO|FDC_MSR_CMDBSY,d0
                bne fdc_success                 ; If not BUSY and ready for READ, we're done

                move.b FDC_DATA,(a3)+           ; Move the data byte to the result buffer
                addq.w #1,d4
                cmp.w d4,d3                     ; Have we reached the end?
                beq fdc_success                 ; Yes, quit
                bra wait_result                 ; And try to fetch another byte

fdc_error       move.l #$ffffffff,d0            ; Return -1 for error
                bra fdc_cmd_exit

fdc_success     move.l #0,d0                    ; Return 0 for success

                ; Restore our registers
                ; TODO: restore what we saved
fdc_cmd_exit    movem (a7)+,d4-d7/a4-a5
                unlk a6                         ; Restore our stack frame
                rts
