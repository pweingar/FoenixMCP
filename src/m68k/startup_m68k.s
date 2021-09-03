            xref ___main
            xdef _bios
            xdef ___exit

            code

coldboot:   lea ___STACK,sp
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
; Function to make a BIOS system call based on the number of the BIOS function:
; int32_t bios(int32_t number, int32_t p0, int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5)
;
_bios:
            movem.l d1-d7,-(sp)
            move.l (18,sp),d7           ; Parameter 5 to D7
            move.l (22,sp),d6           ; Parameter 4 to D6
            move.l (26,sp),d5           ; Parameter 3 to D5
            move.l (30,sp),d4           ; Parameter 2 to D4
            move.l (44,sp),d3           ; Parameter 1 to D3
            move.l (48,sp),d2           ; Parameter 0 to D2
            move.l (42,sp),d1           ; Function number to D1

            trap #13

            movem.l (sp)+,d1-d7
            rts

;
; TRAP#13 handler... transfer control to the C dispatcher
;
h_trap_13:  
            move.l d2,-(sp)
            move.l d1,-(sp)

            jsr _bios_dispatch          ; Call the C routine to do the dispatch
                                        ; Note: the C routine depends upon the register push order

            addq.l #8,a7

            rte                         ; Return to the caller
