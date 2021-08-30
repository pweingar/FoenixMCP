            xref ___exit
            xref ___main

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
; Dummy interrupt handler for when we have no vector installed
;
no_handler: rte

;
; Dummy routine for when we have no function defined
;
no_proc:    moveq.l #0,d0           ; Return 0
            rts

;
; Function to make a BIOS system call based on the number of the BIOS function:
; int bios(int number, ...)
;
_bios::     trap #13
            rts

;
; TRAP#13 handler... play stack games and transfer control to the C dispatcher
;
; Calling conventions:
; Programs will call the kernel using TRAP, first pushing arguments to the user stack
; the only mandatory argument is the function number, which will be the last argument
; pushed to the stack.
;
; Values returned will be in d0
;
; The C routine to process a TRAP will have an additional context variable which
; will be unused by the kernel except by the assembly routine as a means of restoring
; the caller's stack context:
;
; uint32_t handle_TRAP_13(uint32_t context, uint16_t function, ...)
;
h_trap_13:  movem d2-d7/a2-a6,-(sp)     ; Save the caller's registers to the system stack
            movea sp,a6                 ; Hold onto the SSP
            move usp,a7                 ; Get back the usp
            move a6,-(sp)               ; Push the SSP as the context argument

            move.w (4,sp),d0            ; Get the function #
            cmpi #BIOS_FUNCS,d0         ; Is it within range?
            bge bad_func                ; No: return an error number

            lsl.w #2,d0                 ; offset := number * 4
            jsr (bios_jump,pc,d0)       ; Call the function
            bra htrap_done              ; And return results

bad_func:   moveq.l #1,d0               ; Return -1 for an error
            neg.l d0

htrap_done: move (sp)+,a6               ; Get the SSP off the user stack
            move  a7,usp                ; Put the USP back
            movea a6,a7                 ; Restore the SSP
            movem (sp)+,d2-d7/a2-a6     ; Restore the caller's registers from the system stack
            rte                         ; Return to the caller

BIOS_FUNCS=6                            ; The number of BIOS functions that are defined

;
; Jump table for the BIOS dispatch
;
bios_jump:  dc.l no_proc                ; Function #0 = NULL
            dc.l _impl_bconout          ; Function #1 = bconout
            dc.l no_proc                ; Function #2 = NULL
            dc.l no_proc                ; Function #3 = NULL
            dc.l no_proc                ; Function #4 = NULL         
            dc.l _impl_setexc           ; Function #5 = SETEXCW
