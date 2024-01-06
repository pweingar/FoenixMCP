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

            xdef _m68k_int_superio_ps2
            xdef _m68k_int_maurice
            xdef _m68k_int_superio_mouse
            xdef _m68k_int_superio_com1
            xdef _m68k_int_superio_com2
            xdef _m68k_int_superio_lpt1
            xdef _m68k_int_superio_fdc
            xdef _m68k_int_superio_midi
            xdef _m68k_int_timer0
            xdef _m68k_int_timer1
            xdef _m68k_int_timer2
            xdef _m68k_int_timer3
            xdef _m68k_int_timer4
            xdef _m68k_int_x1D
            xdef _m68k_int_x1E
            xdef _m68k_int_rtc
            xdef _m68k_int_hdc
            xdef _m68k_int_sdc_insert
            xdef _m68k_int_sdc
            xdef _m68k_int_opm
            xdef _m68k_int_opn2
            xdef _m68k_int_opl3
            xdef _m68k_int_x26
            xdef _m68k_int_x27
            xdef _m68k_int_beatrix0
            xdef _m68k_int_beatrix1
            xdef _m68k_int_beatrix2
            xdef _m68k_int_beatrix3
            xdef _m68k_int_x2C
            xdef _m68k_int_dac1
            xdef _m68k_int_x2E
            xdef _m68k_int_dac0

  IF MODEL==6 || MODEL==9 ; MODEL_FOENIX_A2560U or MODEL_FOENIX_A2560U_PLUS
MACHINE_IS_A2560U EQU 1
  ELSE
MACHINE_IS_A2560U EQU 0
  ENDC

;
; Interrupt registers for A2560X/K and GenX
;
PENDING_GRP0 = $FEC00100
PENDING_GRP1 = $FEC00102
PENDING_GRP2 = $FEC00104

            section "VECTORS",code

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
  IF MACHINE_IS_A2560U ; A2560U/+ have only one screen
            dc.l not_impl           ; 29 - Level 5 Interrupt Autovector
            dc.l int_vicky_a        ; 30 - Level 6 Interrupt Autovector
  ELSE
            dc.l int_vicky_a        ; 29 - Level 5 Interrupt Autovector
            dc.l int_vicky_b        ; 30 - Level 6 Interrupt Autovector
  ENDC
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

            code

coldboot:   move.w #$2700,SR        ; Supervisor mode, Interrupt mode (68040), disable all interrupts

  IF CPU == 6
            moveq #0,d0   ; Disable 040's MMU
            movec d0,TC
  ENDC

  move.l #$ff00ff00,$fec80008 ; border color for debug
            lea ___STACK,sp
            bsr _int_disable_all

            ; Clear BSS segment (we assumed it's 32bits aligned)
            lea	___BSSSTART,a0
            move.l #___BSSSIZE,d0
            beq	callmain
            moveq #0,d1
clrloop:    move.l d1,(a0)+
            subq.l #4,d0
            bne	clrloop

  move.l #$ffffff00,$fec80008 ; change border color

  xref _int_init
  ;jsr _int_init
  ; move.l #$ffff0000,$fec80008 ; change border color
callmain:   jsr ___main             ; call __main to transfer to the C code

;	endless loop; can be changed accordingly
___exit:
            bra	___exit

;
; Autovector handler: Used by VICKY III Channel B interrupts
;
int_vicky_b:
            movem.l d0-d7/a0-a6,-(a7)
            move.w #8,-(sp)           ; Use MSB of the interrupt registers
            jsr _int_vicky            ; Call the dispatcher for Channel B interrupts
            addq.l  #2,sp
            movem.l (a7)+,d0-d7/a0-a6
            rte

;
; Autovector handler: Used by VICKY III Channel A interrupts
;
int_vicky_a:
            movem.l d0-d7/a0-a6,-(a7)
            clr.w -(sp)               ; Use LSB of the interrupt registers
            jsr _int_vicky            ; Call the dispatcher for Channel A interrupts
            addq.l  #2,sp            
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
            beq.s intdis_end                ; If there isn't one, just return
            movea.l d0,a0
            jsr (a0)                        ; If there is, call it.

intdis_end: movem.l (a7)+,d0-d7/a0-a6       ; Restore affected registers
            rte

            ;
            ; Macro to implement an interrupt handler for the user interrupt vectors
            ;
            ; Standard handlers will:
            ; 1. Save all registers
            ; 2. Clear the pending flag for this interrupt
            ; 3. Look for a handler registered through the system calls
            ; 4. Call the handler (if found)
            ; 5. Restore all registers
            ; 6. Return to the interrupted code
            ;
            macro inthandler                ; Individual interrupt handler. Parameters: interrupt number, interrupt mask, pending register
            movem.l d0-d7/a0-a6,-(a7)       ; Save affected registers
            move.w #\2,(\3)                 ; Clear the flag for the interrupt
            move.w #(\1<<2),d0              ; Get the offset to interrupt 0x11
            bra int_dispatch
            endm

;
; Group 1 Interrupt Handlers
;

_m68k_int_superio_ps2:  inthandler $10, $0001, PENDING_GRP1     ; Interrupt Vector 0x10 -- SuperIO Keyboard
_m68k_int_maurice:  inthandler $11, $0002, PENDING_GRP1         ; Interrupt Vector 0x11 -- A2560K "Mo" keyboard
_m68k_int_superio_mouse:  inthandler $12, $0004, PENDING_GRP1     ; Interrupt Vector 0x12 -- SuperIO Mouse
_m68k_int_superio_com1:  inthandler $13, $0008, PENDING_GRP1     ; Interrupt Vector 0x13 -- COM1
_m68k_int_superio_com2:  inthandler $14, $0010, PENDING_GRP1     ; Interrupt Vector 0x14 -- COM2
_m68k_int_superio_lpt1:  inthandler $15, $0020, PENDING_GRP1     ; Interrupt Vector 0x15 -- LPT1
_m68k_int_superio_fdc:  inthandler $16, $0040, PENDING_GRP1     ; Interrupt Vector 0x16 -- Floppy drive controller
_m68k_int_superio_midi:  inthandler $17, $0080, PENDING_GRP1     ; Interrupt Vector 0x17 -- MIDI
_m68k_int_timer0:  inthandler $18, $0100, PENDING_GRP1        ; Interrupt Vector 0x18 -- Timer 0
_m68k_int_timer1:  inthandler $19, $0200, PENDING_GRP1        ; Interrupt Vector 0x19 -- Timer 1
_m68k_int_timer2:  inthandler $1A, $0400, PENDING_GRP1        ; Interrupt Vector 0x1A -- Timer 2
_m68k_int_timer3:  inthandler $1B, $0800, PENDING_GRP1        ; Interrupt Vector 0x1B -- Timer 3
_m68k_int_timer4:  inthandler $1C, $1000, PENDING_GRP1        ; Interrupt Vector 0x1C -- Timer 4
_m68k_int_x1D:  inthandler $1D, $2000, PENDING_GRP1           ; Interrupt Vector 0x1D -- Reserved
_m68k_int_x1E:  inthandler $1E, $4000, PENDING_GRP1     ; Interrupt Vector 0x1E -- Reserved
_m68k_int_rtc:  inthandler $1F, $8000, PENDING_GRP1     ; Interrupt Vector 0x1F -- Real Time Clock

;
; Group 2 Interrupt Handlers
;

_m68k_int_hdc:  inthandler $20, $0001, PENDING_GRP2     ; Interrupt Vector 0x20 -- IDE HDD Generated Interrupt
_m68k_int_sdc_insert:  inthandler $21, $0002, PENDING_GRP2     ; Interrupt Vector 0x21 -- SDCard Insert
_m68k_int_sdc:  inthandler $22, $0004, PENDING_GRP2     ; Interrupt Vector 0x22 -- SDCard Controller
_m68k_int_opm:  inthandler $23, $0008, PENDING_GRP2     ; Interrupt Vector 0x23 -- Internal OPM
_m68k_int_opn2:  inthandler $24, $0010, PENDING_GRP2     ; Interrupt Vector 0x24 -- External OPN2
_m68k_int_opl3:  inthandler $25, $0020, PENDING_GRP2     ; Interrupt Vector 0x25 -- External OPL3
_m68k_int_x26:  inthandler $26, $0040, PENDING_GRP2     ; Interrupt Vector 0x26 -- Reserved
_m68k_int_x27:  inthandler $27, $0080, PENDING_GRP2     ; Interrupt Vector 0x27 -- Reserved
_m68k_int_beatrix0:  inthandler $28, $0100, PENDING_GRP2     ; Interrupt Vector 0x28 -- Beatrix Interrupt 0
_m68k_int_beatrix1:  inthandler $29, $0200, PENDING_GRP2     ; Interrupt Vector 0x29 -- Beatrix Interrupt 1
_m68k_int_beatrix2:  inthandler $2A, $0400, PENDING_GRP2     ; Interrupt Vector 0x2A -- Beatrix Interrupt 2
_m68k_int_beatrix3:  inthandler $2B, $0800, PENDING_GRP2     ; Interrupt Vector 0x2B -- Beatrix Interrupt 3
_m68k_int_x2C:  inthandler $2C, $1000, PENDING_GRP2     ; Interrupt Vector 0x2C -- Reserved
_m68k_int_dac1:  inthandler $2D, $2000, PENDING_GRP2     ; Interrupt Vector 0x2D -- DAC1 Playback Done Interrupt (48K)
_m68k_int_x2E:  inthandler $2E, $4000, PENDING_GRP2     ; Interrupt Vector 0x2E -- Reserved
_m68k_int_dac0:  inthandler $2F, $8000, PENDING_GRP2     ; Interrupt Vector 0x2F -- DAC0 Playback Done Interrupt (44.1K)

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
                    move.l (2,a7),(a1)
                    move.l (8,a7),(a2)
                    bra call_panic

_handle_addr_err:
                    lea _panic_number,a0
                    lea _panic_pc,a1
                    lea _panic_address,a2
                    move.w #3,(a0)
                    move.l (2,a7),(a1)
                    move.l (8,a7),(a2)
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
            cmp.w #$43,d1               ; Is this a sys_proc_elevate call?
            beq h_trap_elev             ; Yes, just handle it here

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

h_trap_elev ori #$2000,(a7)             ; Change the caller's privilege to supervisor
            rte                         ; And return to it

;
; Jump into a user mode code
;
_call_user:
 IF MACHINE_IS_A2560U
  ; Not sure that works (was taken from startup_m68k.s)
  ; a0 = pointer to code to execute
  ; a1 = location to set user stack pointer
            move.l (4,a7),a0            ; Get the pointer to the code to start
            move.l (8,a7),a1            ; Get the pointer to the process's stack
            move.l (12,a7),d0           ; Get the number of parameters passed
            move.l (16,a7),a2           ; Get the pointer to the parameters
            ; andi #$dfff,sr              ; Drop into user mode
            movea.l a1,a7               ; Set the stack

            move.l a2,-(a7)             ; Push the parameters list
            move.l d0,-(a7)             ; Push the parameter count
            jsr (a0)
 ELSE
            ; Set up the user stack
            move.l #$00010000,a0        ; Get the pointer to the process's stack
            move.l (12,a7),d1           ; Get the number of parameters passed
            move.l (16,a7),a1           ; Get the pointer to the parameters
            move.l a1,-(a0)             ; Push the parameters list
            move.w d1,-(a0)             ; Push the parameter count
            move.l a0,usp               ; Set the User Stack Pointer


            ; Set up the system stack
            move.l (4,a7),a0            ; Get the pointer to the code to start
            move.w #0,-(a7)             ; Push the fake vector offset
            move.l a0,-(a7)             ; Push it as the starting address
            move.w #$0000,-(a7)         ; Push the user's initial SR (to switch to user mode)
            rte                         ; Start the user process
 ENDC
_restart_cli:
            lea ___STACK,sp
            jsr _cli_rerepl
            bra _restart_cli
