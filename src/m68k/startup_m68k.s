            xref ___main
            xref _cli_rerepl
            xref _panic
            xref _panic_number

            xdef _syscall
            xdef ___exit
            xdef _int_enable_all
            xdef _int_disable_all
            xdef _call_user
            xdef _restart_cli

;
; Interrupt registers for A2560U and U+
;
PENDING_GRP0 = $00B00100
PENDING_GRP1 = $00B00102
PENDING_GRP2 = $00B00104

            section "vectors",code

            dc.l ___STACK           ; 00 - Initial stack pointer
            dc.l coldboot           ; 01 - Initial PC
            dc.l _handle_bus_err    ; 02 - Bus error
            dc.l _handle_addr_err   ; 03 - Address error
            dc.l _handle_inst_err   ; 04 - Illegal instruction
            dc.l _handle_div0_err   ; 05 - Zero divide
            dc.l _handle_chk_err    ; 06 - CHK instruction
            dc.l _handle_trapv_err  ; 07 - TRAPV instruction
            dc.l _handle_priv_err   ; 08 - Privilege error
            dc.l not_impl           ; 09 - Trace
            dc.l not_impl           ; 10 - Line 1010
            dc.l not_impl           ; 11 - Line 1111
            dc.l not_impl           ; 12 - Reserved
            dc.l not_impl           ; 13 - Reserved
            dc.l not_impl           ; 14 - Format error
            dc.l not_impl           ; 15 - Uninitialized Interrupt Error
            dc.l not_impl           ; 16 - Reserved
            dc.l not_impl           ; 17 - Reserved
            dc.l not_impl           ; 18 - Reserved
            dc.l not_impl           ; 19 - Reserved
            dc.l not_impl           ; 20 - Reserved
            dc.l not_impl           ; 21 - Reserved
            dc.l not_impl           ; 22 - Reserved
            dc.l not_impl           ; 23 - Reserved
            dc.l _handle_spurious   ; 24 - Spurious Interrupt
            dc.l not_impl           ; 25 - Level 1 Interrupt Autovector
            dc.l not_impl           ; 26 - Level 2 Interrupt Autovector
            dc.l not_impl           ; 27 - Level 3 Interrupt Autovector
            dc.l not_impl           ; 28 - Level 4 Interrupt Autovector
            dc.l not_impl           ; 29 - Level 5 Interrupt Autovector
            dc.l autovec2           ; 30 - Level 6 Interrupt Autovector
            dc.l not_impl           ; 31 - Level 7 Interrupt Autovector
            dc.l not_impl           ; 32 - TRAP #0
            dc.l not_impl           ; 33 - TRAP #1
            dc.l not_impl           ; 34 - TRAP #2
            dc.l not_impl           ; 35 - TRAP #3
            dc.l not_impl           ; 36 - TRAP #4
            dc.l not_impl           ; 37 - TRAP #5
            dc.l not_impl           ; 38 - TRAP #6
            dc.l not_impl           ; 39 - TRAP #7
            dc.l not_impl           ; 40 - TRAP #8
            dc.l not_impl           ; 41 - TRAP #9
            dc.l not_impl           ; 42 - TRAP #10
            dc.l not_impl           ; 43 - TRAP #11
            dc.l not_impl           ; 44 - TRAP #12
            dc.l not_impl           ; 45 - TRAP #13
            dc.l not_impl           ; 46 - TRAP #14
            dc.l h_trap_15          ; 47 - TRAP #15
            dc.l not_impl           ; 48 - Reserved
            dc.l not_impl           ; 49 - Reserved
            dc.l not_impl           ; 50 - Reserved
            dc.l not_impl           ; 51 - Reserved
            dc.l not_impl           ; 52 - Reserved
            dc.l not_impl           ; 53 - Reserved
            dc.l not_impl           ; 54 - Reserved
            dc.l not_impl           ; 55 - Reserved
            dc.l not_impl           ; 56 - Reserved
            dc.l not_impl           ; 57 - Reserved
            dc.l not_impl           ; 58 - Reserved
            dc.l not_impl           ; 59 - Reserved
            dc.l not_impl           ; 60 - Reserved
            dc.l not_impl           ; 61 - Reserved
            dc.l not_impl           ; 62 - Reserved
            dc.l not_impl           ; 63 - Reserved
            dc.l interrupt_x10      ; 64 - Interrupt 0x10 - SuperIO - PS/2 Keyboard
            dc.l interrupt_x11      ; 65 - Interrupt 0x11 - A2560K Built-in Keyboard (Mo)
            dc.l interrupt_x12      ; 66 - Interrupt 0x12 - SuperIO - PS/2 Mouse
            dc.l not_impl           ; 67 - Interrupt 0x13 - SuperIO - COM1
            dc.l not_impl           ; 68 - Interrupt 0x14 - SuperIO - COM2
            dc.l not_impl           ; 69 - Interrupt 0x15 - SuperIO - LPT1
            dc.l not_impl           ; 70 - Interrupt 0x16 - SuperIO - Floppy Disk Controller
            dc.l not_impl           ; 71 - Interrupt 0x17 - SuperIO - MIDI
            dc.l not_impl           ; 72 - Interrupt 0x18 - Timer 0
            dc.l not_impl           ; 73 - Interrupt 0x19 - Timer 1
            dc.l not_impl           ; 74 - Interrupt 0x1A - Timer 2
            dc.l not_impl           ; 76 - Interrupt 0x1B - Timer 3
            dc.l not_impl           ; 75 - Interrupt 0x1C - Timer 4
            dc.l not_impl           ; 77 - Interrupt 0x1D - Reserved
            dc.l not_impl           ; 78 - Interrupt 0x1E - Reserved
            dc.l interrupt_x1F      ; 79 - Interrupt 0x1F - Real Time Clock

            dc.l not_impl           ; 80 - Interrupt 0x20 - IDE HDD Generated Interrupt
            dc.l not_impl           ; 81 - Interrupt 0x21 - SDCard Insert
            dc.l not_impl           ; 82 - Interrupt 0x22 - SDCard Controller
            dc.l not_impl           ; 83 - Interrupt 0x23 - Internal OPM
            dc.l not_impl           ; 84 - Interrupt 0x24 - External OPN2
            dc.l not_impl           ; 85 - Interrupt 0x25 - External OPL3
            dc.l not_impl           ; 86 - Interrupt 0x26 - Reserved
            dc.l not_impl           ; 87 - Interrupt 0x27 - Reserved
            dc.l not_impl           ; 88 - Interrupt 0x28 - Beatrix Interrupt 0
            dc.l not_impl           ; 89 - Interrupt 0x29 - Beatrix Interrupt 1
            dc.l not_impl           ; 90 - Interrupt 0x2A - Beatrix Interrupt 2
            dc.l not_impl           ; 91 - Interrupt 0x2B - Beatrix Interrupt 3
            dc.l not_impl           ; 92 - Interrupt 0x2C - Reserved
            dc.l not_impl           ; 93 - Interrupt 0x2D - DAC1 Playback Done Interrupt (48K)
            dc.l not_impl           ; 94 - Interrupt 0x2E - Reserved
            dc.l not_impl           ; 95 - Interrupt 0x2F - DAC0 Playback Done Interrupt (44.1K)

            code

coldboot:   lea ___STACK,sp
            bsr _int_disable_all

            lea	___BSSSTART,a0
            move.l #___BSSSIZE,d0
            beq	callmain

            ; clrloop:    clr.l (a0)+
            ; subq.l #4,d0
            ; bne	clrloop

            ; Set TRAP #15 vector handler
            lea h_trap_15,a0        ; Address of the handler
            move.l #(13+32)<<2,a1   ; TRAP#15 vector address
            move.l a0,(a1)          ; Set the vector

callmain:   jsr ___main             ; call __main to transfer to the C code

;	endless loop; can be changed accordingly
___exit:
            bra	___exit

;
; Autovector #1: Used by VICKY III Channel B interrupts
;
autovec1:   movem.l d0-d7/a0-a6,-(a7)
            jsr _int_vicky_channel_b        ; Call the dispatcher for Channel B interrupts
            movem.l (a7)+,d0-d7/a0-a6
            rte

;
; Autovector #1: Used by VICKY III Channel A interrupts
;
autovec2:   movem.l d0-d7/a0-a6,-(a7)
            jsr _int_vicky_channel_a        ; Call the dispatcher for Channel A interrupts
            movem.l (a7)+,d0-d7/a0-a6
            rte

;
; Given the interrupt's offset in the interrupt handler table, fetch the pointer
; to the interrupt handler and call it... if there is one
;
; Assumes registers D0-D7, A0-A6 have been saved to the stack with MOVEM
;
int_dispatch:
            lea _g_int_handler,a0           ; Look in the interrupt handler table
            move.l (0,a0,d0),d0             ; Get the address of the handler
            beq intdis_end                  ; If there isn't one, just return
            movea.l d0,a0
            jsr (a0)                        ; If there is, call it.

intdis_end: movem.l (a7)+,d0-d7/a0-a6       ; Restore affected registers
            rte

;
; Interrupt Vector 0x10 -- SuperIO Keyboard
;
interrupt_x10:
            move.w #$0001,(PENDING_GRP1)    ; Clear the flag for INT 10
            movem.l d0-d7/a0-a6,-(a7)       ; Save affected registers
            move.w #($10<<2),d0             ; Get the offset to interrupt 0x11
            bra int_dispatch

;
; Interrupt Vector 0x11 -- A2560K "Mo" keyboard
;
interrupt_x11:
            move.w #$0002,(PENDING_GRP1)    ; Clear the flag for INT 11
            movem.l d0-d7/a0-a6,-(a7)       ; Save affected registers
            move.w #($11<<2),d0             ; Get the offset to interrupt 0x11
            bra int_dispatch                ; And process the interrupt

;
; Interrupt Vector 0x12 -- SuperIO Mouse
;
interrupt_x12:
            move.w #$0004,(PENDING_GRP1)    ; Clear the flag for INT 12
            movem.l d0-d7/a0-a6,-(a7)       ; Save affected registers
            move.w #($12<<2),d0             ; Get the offset to interrupt 0x11
            bra int_dispatch                ; And process the interrupt

;
; Interrupt Vector 0x1F -- RTC
;
interrupt_x1F:
            move.w #$8000,(PENDING_GRP1)    ; Clear the flag for INT 1F
            movem.l d0-d7/a0-a6,-(a7)       ; Save affected registers
            move.w #($1f<<2),d0             ; Get the offset to interrupt 0x1f
            bra int_dispatch                ; And process the interrupt

;
; Interrupt Vector 0x21 -- SDCard Insert
;
interrupt_x21:
            movem.l d0-d7/a0-a6,-(a7)       ; Save affected registers
            move.w #($21<<2),d0             ; Get the offset to interrupt 0x1f
            bra int_dispatch                ; And process the interrupt

;
; Unimplemented Exception Handler -- just return
;
not_impl:   rte

;
; Enable all interrupts
;
_int_enable_all:    move.w SR,d0        ; Save the old level for return
                    andi.w #$0700,d0
                    lsr.w #8,d0
                    ext.l d0

                    andi.w #$F8FF,SR    ; Clear the level
                    rts

;
; Disable all interrupts
;
_int_disable_all:   move.w SR,d0        ; Save the old level for return
                    andi.w #$0700,d0
                    lsr.w #8,d0
                    ext.l d0

                    ori.w #$0700,SR     ; Set the level to 7
                    rts

;
; Restore interrupt priority
;
_int_restore:       move.w (4,sp),d0    ; Get the priority into d0
                    andi.w #3,d0
                    lsl.w #8,d0

                    move.w sr,d1        ; Get the current SR into d1
                    andi.w #$F8FF,d1    ; Clear the old priority
                    or.w d0,d1          ; And set it to the current
                    move.w d1,sr        ; And set the level

                    rts

;
; Handlers for the various exceptions...
;
; These all go to the kernel panic screen after placing the
; triggering PC address in "panic_pc" and the number of the exception
; in "panic_number". Bus and address errors will also put the
; address involved in "panic_address".
;

_handle_bus_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    lea _panic_address,a2
                    move.w #2,(a0)
                    move.l (10,a7),(a1)
                    move.l (2,a7),(a2)
                    bra call_panic

_handle_addr_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    lea _panic_address,a2
                    move.w #3,(a0)
                    move.l (10,a7),(a1)
                    move.l (2,a7),(a2)
                    bra call_panic

_handle_inst_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    move.w #4,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_div0_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    move.w #5,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_chk_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    move.w #6,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_trapv_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    move.w #7,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_priv_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    move.w #8,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_spurious:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    move.w #24,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

call_panic:         jsr _panic
panic_lock:         bra panic_lock

;
; Function to make a system call based on the number of the system function:
; int32_t syscall(int32_t number, int32_t p0, int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5)
;
_syscall:
            movem.l d1-d7,-(sp)         ; Save caller's registers
            move.l (56,sp),d6           ; Parameter 5 to D6
            move.l (52,sp),d5           ; Parameter 4 to D5
            move.l (48,sp),d4           ; Parameter 3 to D4
            move.l (44,sp),d3           ; Parameter 2 to D3
            move.l (40,sp),d2           ; Parameter 1 to D2
            move.l (36,sp),d1           ; Parameter 0 to D1
            move.l (32,sp),d0           ; Function number to D0

            TRAP #15                    ; Call into the kernel

            movem.l (sp)+,d1-d7         ; Restore caller's registers
            rts

;
; TRAP#15 handler... transfer control to the C dispatcher
;
h_trap_15:
            move.l d6,-(sp)             ; Push the parameters to the stack for the C call
            move.l d5,-(sp)
            move.l d4,-(sp)
            move.l d3,-(sp)
            move.l d2,-(sp)
            move.l d1,-(sp)
            move.l d0,-(sp)

            jsr _syscall_dispatch       ; Call the C routine to do the dispatch
                                        ; Note: the C routine depends upon the register push order

            add.l #28,sp                ; Remove parameters from the stack

            rte                         ; Return to the caller

;
; Jump into a user mode code
;
; Inputs:
; a0 = pointer to code to execute
; a1 = location to set user stack pointer
;
_call_user:
            move.l (4,a7),a0            ; Get the pointer to the code to start
            move.l (8,a7),a1            ; Get the pointer to the process's stack
            move.l (12,a7),d0           ; Get the number of parameters passed
            move.l (16,a7),a2           ; Get the pointer to the parameters
            andi #$dfff,sr              ; Drop into user mode
            movea.l a1,a7               ; Set the stack

            move.l a2,-(a7)             ; Push the parameters list
            move.l d0,-(a7)             ; Push the parameter count
            jsr (a0)

_restart_cli:
            lea ___STACK,sp
            jsr _cli_rerepl
            bra _restart_cli
