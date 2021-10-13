            xref ___main
            xref _cli_rerepl

            xdef _syscall
            xdef ___exit
            xdef _int_enable_all
            xdef _int_disable_all
            xdef _call_user
            xdef _restart_cli

            section "vectors",code

            dc.l ___STACK           ; 00 - Initial stack pointer
            dc.l coldboot           ; 01 - Initial PC
            dc.l not_impl           ; 02 - Bus error
            dc.l not_impl           ; 03 - Address error
            dc.l not_impl           ; 04 - Illegal instruction
            dc.l not_impl           ; 05 - Zero divide
            dc.l not_impl           ; 06 - CHK instruction
            dc.l not_impl           ; 07 - TRAPV instruction
            dc.l not_impl           ; 08 - Priviledge error
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
            dc.l not_impl           ; 24 - Spurious Interrupt
            dc.l autovec1           ; 25 - Level 1 Interrupt Autovector
            dc.l autovec2           ; 26 - Level 2 Interrupt Autovector
            dc.l not_impl           ; 27 - Level 3 Interrupt Autovector
            dc.l not_impl           ; 28 - Level 4 Interrupt Autovector
            dc.l not_impl           ; 29 - Level 5 Interrupt Autovector
            dc.l not_impl           ; 30 - Level 6 Interrupt Autovector
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
            ; dc.l interrupt_x13      ; 67 - Interrupt 0x13 - SuperIO - COM1
            ; dc.l interrupt_x14      ; 68 - Interrupt 0x14 - SuperIO - COM2
            ; dc.l interrupt_x15      ; 69 - Interrupt 0x15 - SuperIO - LPT1
            ; dc.l interrupt_x16      ; 70 - Interrupt 0x16 - SuperIO - Floppy Disk Controller
            ; dc.l interrupt_x17      ; 71 - Interrupt 0x17 - SuperIO - MIDI
            ; dc.l interrupt_x18      ; 72 - Interrupt 0x18 - Timer 0
            ; dc.l interrupt_x19      ; 73 - Interrupt 0x19 - Timer 1
            ; dc.l interrupt_x1A      ; 74 - Interrupt 0x1A - Timer 2
            ; dc.l interrupt_x1B      ; 76 - Interrupt 0x1B - Timer 3
            ; dc.l interrupt_x1C      ; 75 - Interrupt 0x1C - Timer 4
            ; dc.l not_impl           ; 77 - Interrupt 0x1D - Reserved
            ; dc.l not_impl           ; 78 - Interrupt 0x1E - Reserved
            dc.l interrupt_x1F      ; 79 - Interrupt 0x1F - Real Time Clock

            ; dc.l interrupt_x20      ; 80 - Interrupt 0x20 - IDE HDD Generated Interrupt
            ; dc.l interrupt_x21      ; 81 - Interrupt 0x21 - SDCard Insert
            ; dc.l interrupt_x22      ; 82 - Interrupt 0x22 - SDCard Controller
            ; dc.l interrupt_x23      ; 83 - Interrupt 0x23 - Internal OPM
            ; dc.l interrupt_x24      ; 84 - Interrupt 0x24 - External OPN2
            ; dc.l interrupt_x25      ; 85 - Interrupt 0x25 - External OPL3
            ; dc.l interrupt_x26      ; 86 - Interrupt 0x26 - Reserved
            ; dc.l interrupt_x27      ; 87 - Interrupt 0x27 - Reserved
            ; dc.l interrupt_x28      ; 88 - Interrupt 0x28 - Beatrix Interrupt 0
            ; dc.l interrupt_x29      ; 89 - Interrupt 0x29 - Beatrix Interrupt 1
            ; dc.l interrupt_x2A      ; 90 - Interrupt 0x2A - Beatrix Interrupt 2
            ; dc.l interrupt_x2B      ; 91 - Interrupt 0x2B - Beatrix Interrupt 3
            ; dc.l interrupt_x2C      ; 92 - Interrupt 0x2C - Reserved
            ; dc.l interrupt_x2D      ; 93 - Interrupt 0x2D - DAC1 Playback Done Interrupt (48K)
            ; dc.l interrupt_x2E      ; 94 - Interrupt 0x2E - Reserved
            ; dc.l interrupt_x2F      ; 95 - Interrupt 0x2F - DAC0 Playback Done Interrupt (44.1K)

            ; TODO: make room for reserved and User Interrupt Vectors

            code

coldboot:   lea ___STACK,sp
            bsr _int_disable_all

            lea	___BSSSTART,a0
            move.l #___BSSSIZE,d0
            beq	callmain

clrloop:    clr.l (a0)+
            subq.l #4,d0
            bne	clrloop

            ; Set TRAP #13 vector handler
            lea h_trap_15,a0        ; Address of the handler
            move.l #(13+32)<<2,a1   ; TRAP#13 vector address
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
            movea.l (0,a0,d0),a1            ; Get the address of the handler
            beq intdis_end                  ; If there isn't one, just return

            jsr (a1)                        ; If there is, call it.

intdis_end: movem.l (a7)+,d0-d7/a0-a6       ; Restore the registers
            rte

;
; Interrupt Vector 0x10 -- SuperIO Keyboard
;
interrupt_x10:
            movem.l d0-d7/a0-a6,-(a7)       ; Save all the registers
            move.w #($10<<2),d0             ; Get the offset to interrupt 0x11
            bra int_dispatch                ; And process the interrupt

;
; Interrupt Vector 0x11 -- A2560K "Mo" keyboard
;
interrupt_x11:
            movem.l d0-d7/a0-a6,-(a7)       ; Save all the registers
            move.w #($11<<2),d0             ; Get the offset to interrupt 0x11
            bra int_dispatch                ; And process the interrupt

;
; Interrupt Vector 0x12 -- SuperIO Mouse
;
interrupt_x12:
            movem.l d0-d7/a0-a6,-(a7)       ; Save all the registers
            move.w #($12<<2),d0             ; Get the offset to interrupt 0x11
            bra int_dispatch                ; And process the interrupt
            ; jsr _mouse_handle_irq
            ; movem.l (a7)+,d0-d7/a0-a6       ; Restore the registers
            ; rte
            
;
; Interrupt Vector 0x1F -- RTC
;
interrupt_x1F:
            movem.l d0-d7/a0-a6,-(a7)       ; Save all the registers
            move.w #($1f<<2),d0             ; Get the offset to interrupt 0x1f
            bra int_dispatch                ; And process the interrupt

;
; Unimplemented Exception Handler -- just return
;
not_impl:   rte

;
; Enable all interrupts
;
_int_enable_all:    andi.w #$F8FF,SR
                    rts

;
; Disable all interrupts
;
_int_disable_all:   ori.w #$0700,SR
                    rts

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
            move.l (4,a7),a0
            move.l (8,a7),a1
            andi #$dfff,sr              ; Drop into user mode
            movea.l a1,a7               ; Set the stack
            jmp (a0)

_restart_cli:
            lea ___STACK,sp
            jsr _cli_rerepl
            bra _restart_cli
