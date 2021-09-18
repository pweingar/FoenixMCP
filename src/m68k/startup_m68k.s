            xref ___main
            xdef _syscall
            xdef ___exit
            xdef _int_enable_all
            xdef _int_disable_all

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
            dc.l h_trap_13          ; 45 - TRAP #13
            dc.l not_impl           ; 46 - TRAP #14
            dc.l not_impl           ; 47 - TRAP #15

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
            lea h_trap_13,a0        ; Address of the handler
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
            move.l (56,sp),d7           ; Parameter 5 to D7
            move.l (52,sp),d6           ; Parameter 4 to D6
            move.l (48,sp),d5           ; Parameter 3 to D5
            move.l (44,sp),d4           ; Parameter 2 to D4
            move.l (40,sp),d3           ; Parameter 1 to D3
            move.l (36,sp),d2           ; Parameter 0 to D2
            move.l (32,sp),d1           ; Function number to D1

            TRAP #13                    ; Call into the kernel

            movem.l (sp)+,d1-d7         ; Restore caller's registers
            rts


;
; TRAP#13 handler... transfer control to the C dispatcher
;
h_trap_13:
            move.l d7,-(sp)             ; Push the parameters to the stack for the C call
            move.l d6,-(sp)
            move.l d5,-(sp)
            move.l d4,-(sp)
            move.l d3,-(sp)
            move.l d2,-(sp)
            move.l d1,-(sp)

            jsr _syscall_dispatch       ; Call the C routine to do the dispatch
                                        ; Note: the C routine depends upon the register push order

            add.l #28,sp                ; Remove parameters from the stack

            rte                         ; Return to the caller
