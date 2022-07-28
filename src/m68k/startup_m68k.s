            .extern main
            .extern cli_rerepl
            .extern panic
            .extern panic_number
            .extern panic_pc
            .extern panic_address	
            .extern syscall_dispatch
	        .extern g_int_handler
            .extern int_vicky_channel_a

            .public syscall
            .public int_enable_all
            .public int_disable_all
            .public call_user
            .public restart_cli
            .public h_trap_15

;
; Interrupt registers for A2560U and U+
;
PENDING_GRP0 .equ 0x00B00100
PENDING_GRP1 .equ 0x00B00102
PENDING_GRP2 .equ 0x00B00104

            .section vectors
            .long _handle_bus_err    ; 02 - Bus error
            .long _handle_addr_err   ; 03 - Address error
            .long _handle_inst_err   ; 04 - Illegal instruction
            .long _handle_div0_err   ; 05 - Zero divide
            .long _handle_chk_err    ; 06 - CHK instruction
            .long _handle_trapv_err  ; 07 - TRAPV instruction
            .long _handle_priv_err   ; 08 - Privilege error
            .long not_impl           ; 09 - Trace
            .long not_impl           ; 10 - Line 1010
            .long not_impl           ; 11 - Line 1111
            .long not_impl           ; 12 - Reserved
            .long not_impl           ; 13 - Reserved
            .long not_impl           ; 14 - Format error
            .long not_impl           ; 15 - Uninitialized Interrupt Error
            .long not_impl           ; 16 - Reserved
            .long not_impl           ; 17 - Reserved
            .long not_impl           ; 18 - Reserved
            .long not_impl           ; 19 - Reserved
            .long not_impl           ; 20 - Reserved
            .long not_impl           ; 21 - Reserved
            .long not_impl           ; 22 - Reserved
            .long not_impl           ; 23 - Reserved
            .long _handle_spurious   ; 24 - Spurious Interrupt
            .long not_impl           ; 25 - Level 1 Interrupt Autovector
            .long not_impl           ; 26 - Level 2 Interrupt Autovector
            .long not_impl           ; 27 - Level 3 Interrupt Autovector
            .long not_impl           ; 28 - Level 4 Interrupt Autovector
            .long not_impl           ; 29 - Level 5 Interrupt Autovector
            .long not_impl           ; 30 - Level 6 Interrupt Autovector
            .long not_impl           ; 31 - Level 7 Interrupt Autovector
            .long not_impl           ; 32 - TRAP #0
            .long not_impl           ; 33 - TRAP #1
            .long not_impl           ; 34 - TRAP #2
            .long not_impl           ; 35 - TRAP #3
            .long not_impl           ; 36 - TRAP #4
            .long not_impl           ; 37 - TRAP #5
            .long not_impl           ; 38 - TRAP #6
            .long not_impl           ; 39 - TRAP #7
            .long not_impl           ; 40 - TRAP #8
            .long not_impl           ; 41 - TRAP #9
            .long not_impl           ; 42 - TRAP #10
            .long not_impl           ; 43 - TRAP #11
            .long not_impl           ; 44 - TRAP #12
            .long not_impl           ; 45 - TRAP #13
            .long not_impl           ; 46 - TRAP #14
            .long h_trap_15          ; 47 - TRAP #15
            .long not_impl           ; 48 - Reserved
            .long not_impl           ; 49 - Reserved
            .long not_impl           ; 50 - Reserved
            .long not_impl           ; 51 - Reserved
            .long not_impl           ; 52 - Reserved
            .long not_impl           ; 53 - Reserved
            .long not_impl           ; 54 - Reserved
            .long not_impl           ; 55 - Reserved
            .long not_impl           ; 56 - Reserved
            .long not_impl           ; 57 - Reserved
            .long not_impl           ; 58 - Reserved
            .long not_impl           ; 59 - Reserved
            .long not_impl           ; 60 - Reserved
            .long not_impl           ; 61 - Reserved
            .long not_impl           ; 62 - Reserved
            .long not_impl           ; 63 - Reserved
            .long interrupt_x10      ; 64 - Interrupt 0x10 - SuperIO - PS/2 Keyboard
            .long interrupt_x11      ; 65 - Interrupt 0x11 - A2560K Built-in Keyboard (Mo)
            .long interrupt_x12      ; 66 - Interrupt 0x12 - SuperIO - PS/2 Mouse
            .long interrupt_x13      ; 67 - Interrupt 0x13 - SuperIO - COM1
            .long interrupt_x14      ; 68 - Interrupt 0x14 - SuperIO - COM2
            .long interrupt_x15      ; 69 - Interrupt 0x15 - SuperIO - LPT1
            .long interrupt_x16      ; 70 - Interrupt 0x16 - SuperIO - Floppy Disk Controller
            .long interrupt_x17      ; 71 - Interrupt 0x17 - SuperIO - MIDI
            .long interrupt_x18      ; 72 - Interrupt 0x18 - Timer 0
            .long interrupt_x19      ; 73 - Interrupt 0x19 - Timer 1
            .long interrupt_x1A      ; 74 - Interrupt 0x1A - Timer 2
            .long interrupt_x1B      ; 76 - Interrupt 0x1B - Timer 3
            .long interrupt_x1C      ; 75 - Interrupt 0x1C - Timer 4
            .long interrupt_x1D      ; 77 - Interrupt 0x1D - Reserved
            .long interrupt_x1E      ; 78 - Interrupt 0x1E - Reserved
            .long interrupt_x1F      ; 79 - Interrupt 0x1F - Real Time Clock

            .long interrupt_x20      ; 80 - Interrupt 0x20 - IDE HDD Generated Interrupt
            .long interrupt_x21      ; 81 - Interrupt 0x21 - SDCard Insert
            .long interrupt_x22      ; 82 - Interrupt 0x22 - SDCard Controller
            .long interrupt_x23      ; 83 - Interrupt 0x23 - Internal OPM
            .long interrupt_x24      ; 84 - Interrupt 0x24 - External OPN2
            .long interrupt_x25      ; 85 - Interrupt 0x25 - External OPL3
            .long interrupt_x26      ; 86 - Interrupt 0x26 - Reserved
            .long interrupt_x27      ; 87 - Interrupt 0x27 - Reserved
            .long interrupt_x28      ; 88 - Interrupt 0x28 - Beatrix Interrupt 0
            .long interrupt_x29      ; 89 - Interrupt 0x29 - Beatrix Interrupt 1
            .long interrupt_x2A      ; 90 - Interrupt 0x2A - Beatrix Interrupt 2
            .long interrupt_x2B      ; 91 - Interrupt 0x2B - Beatrix Interrupt 3
            .long interrupt_x2C      ; 92 - Interrupt 0x2C - Reserved
            .long interrupt_x2D      ; 93 - Interrupt 0x2D - DAC1 Playback Done Interrupt (48K)
            .long interrupt_x2E      ; 94 - Interrupt 0x2E - Reserved
            .long interrupt_x2F      ; 95 - Interrupt 0x2F - DAC0 Playback Done Interrupt (44.1K)

            .section stack
            .section code

;
; Given the interrupt's offset in the interrupt handler table, fetch the pointer
; to the interrupt handler and call it... if there is one
;
; Assumes registers D0-D7, A0-A6 have been saved to the stack with MOVEM
;
int_dispatch:
            lea g_int_handler,a0           ; Look in the interrupt handler table
            move.l (0,a0,d0.w),d0               ; Get the address of the handler
            beq intdis_end                  ; If there isn't one, just return
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
inthandler  .macro irq_no,irq_mask,pending_reg
; Individual interrupt handler. Parameters: interrupt number, interrupt mask, pending register
            movem.l d0-d7/a0-a6,-(a7)        ; Save affected registers
            move.w #\irq_mask,(\pending_reg) ; Clear the flag for the interrupt
            move.w #(\irq_no<<2),d0          ; Get the offset to interrupt 0x11
            bra int_dispatch
            .endm

;
; Group 1 Interrupt Handlers
;

interrupt_x10:  inthandler 0x10,0x0001,PENDING_GRP1
				; Interrupt Vector 0x10 -- SuperIO Keyboard
interrupt_x11:  inthandler 0x11, 0x0002, PENDING_GRP1     ; Interrupt Vector 0x11 -- A2560K "Mo" keyboard
interrupt_x12:  inthandler 0x12, 0x0004, PENDING_GRP1     ; Interrupt Vector 0x12 -- SuperIO Mouse
interrupt_x13:  inthandler 0x13, 0x0008, PENDING_GRP1     ; Interrupt Vector 0x13 -- COM1
interrupt_x14:  inthandler 0x14, 0x0010, PENDING_GRP1     ; Interrupt Vector 0x14 -- COM2
interrupt_x15:  inthandler 0x15, 0x0020, PENDING_GRP1     ; Interrupt Vector 0x15 -- LPT1
interrupt_x16:  inthandler 0x16, 0x0040, PENDING_GRP1     ; Interrupt Vector 0x16 -- Floppy drive controller
interrupt_x17:  inthandler 0x17, 0x0080, PENDING_GRP1     ; Interrupt Vector 0x17 -- MIDI
interrupt_x18:  inthandler 0x18, 0x0100, PENDING_GRP1     ; Interrupt Vector 0x18 -- Timer 0
interrupt_x19:  inthandler 0x19, 0x0200, PENDING_GRP1     ; Interrupt Vector 0x19 -- Timer 1
interrupt_x1A:  inthandler 0x1A, 0x0400, PENDING_GRP1     ; Interrupt Vector 0x1A -- Timer 2
interrupt_x1B:  inthandler 0x1B, 0x0800, PENDING_GRP1     ; Interrupt Vector 0x1B -- Timer 3
interrupt_x1C:  inthandler 0x1C, 0x1000, PENDING_GRP1     ; Interrupt Vector 0x1C -- Timer 4
interrupt_x1D:  inthandler 0x1D, 0x2000, PENDING_GRP1     ; Interrupt Vector 0x1D -- Reserved
interrupt_x1E:  inthandler 0x1E, 0x4000, PENDING_GRP1     ; Interrupt Vector 0x1E -- Reserved
interrupt_x1F:  inthandler 0x1F, 0x8000, PENDING_GRP1     ; Interrupt Vector 0x1F -- Real Time Clock

;
; Group 2 Interrupt Handlers
;

interrupt_x20:  inthandler 0x20, 0x0001, PENDING_GRP2     ; Interrupt Vector 0x20 -- IDE HDD Generated Interrupt
interrupt_x21:  inthandler 0x21, 0x0002, PENDING_GRP2     ; Interrupt Vector 0x21 -- SDCard Insert
interrupt_x22:  inthandler 0x22, 0x0004, PENDING_GRP2     ; Interrupt Vector 0x22 -- SDCard Controller
interrupt_x23:  inthandler 0x23, 0x0008, PENDING_GRP2     ; Interrupt Vector 0x23 -- Internal OPM
interrupt_x24:  inthandler 0x24, 0x0010, PENDING_GRP2     ; Interrupt Vector 0x24 -- External OPN2
interrupt_x25:  inthandler 0x25, 0x0020, PENDING_GRP2     ; Interrupt Vector 0x25 -- External OPL3
interrupt_x26:  inthandler 0x26, 0x0040, PENDING_GRP2     ; Interrupt Vector 0x26 -- Reserved
interrupt_x27:  inthandler 0x27, 0x0080, PENDING_GRP2     ; Interrupt Vector 0x27 -- Reserved
interrupt_x28:  inthandler 0x28, 0x0100, PENDING_GRP2     ; Interrupt Vector 0x28 -- Beatrix Interrupt 0
interrupt_x29:  inthandler 0x29, 0x0200, PENDING_GRP2     ; Interrupt Vector 0x29 -- Beatrix Interrupt 1
interrupt_x2A:  inthandler 0x2A, 0x0400, PENDING_GRP2     ; Interrupt Vector 0x2A -- Beatrix Interrupt 2
interrupt_x2B:  inthandler 0x2B, 0x0800, PENDING_GRP2     ; Interrupt Vector 0x2B -- Beatrix Interrupt 3
interrupt_x2C:  inthandler 0x2C, 0x1000, PENDING_GRP2     ; Interrupt Vector 0x2C -- Reserved
interrupt_x2D:  inthandler 0x2D, 0x2000, PENDING_GRP2     ; Interrupt Vector 0x2D -- DAC1 Playback Done Interrupt (48K)
interrupt_x2E:  inthandler 0x2E, 0x4000, PENDING_GRP2     ; Interrupt Vector 0x2E -- Reserved
interrupt_x2F:  inthandler 0x2F, 0x8000, PENDING_GRP2     ; Interrupt Vector 0x2F -- DAC0 Playback Done Interrupt (44.1K)

;
; Unimplemented Exception Handler -- just return
;
not_impl:   rte

;
; Enable all interrupts
;
int_enable_all:    move.w SR,d0        ; Save the old level for return
                    andi.w #0x0700,d0
                    lsr.w #8,d0
                    ext.l d0

                    andi.w #0xF8FF,SR    ; Clear the level
                    rts

;
; Disable all interrupts
;
int_disable_all:   move.w SR,d0        ; Save the old level for return
                    andi.w #0x0700,d0
                    lsr.w #8,d0
                    ext.l d0

                    ori.w #0x0700,SR     ; Set the level to 7
                    rts

;
; Restore interrupt priority
;
_int_restore:       move.w (4,sp),d0    ; Get the priority into d0
                    andi.w #3,d0
                    lsl.w #8,d0

                    move.w sr,d1        ; Get the current SR into d1
                    andi.w #0xF8FF,d1    ; Clear the old priority
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
                    lea panic_number,a0
                    lea panic_pc,a1
                    lea panic_address,a2
                    move.w #2,(a0)
                    move.l (10,a7),(a1)
                    move.l (2,a7),(a2)
                    bra call_panic

_handle_addr_err:
                    lea panic_number,a0
                    lea panic_pc,a1
                    lea panic_address,a2
                    move.w #3,(a0)
                    move.l (10,a7),(a1)
                    move.l (2,a7),(a2)
                    bra call_panic

_handle_inst_err:
                    lea panic_number,a0
                    lea panic_pc,a1
                    move.w #4,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_div0_err:
                    lea panic_number,a0
                    lea panic_pc,a1
                    move.w #5,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_chk_err:
                    lea panic_number,a0
                    lea panic_pc,a1
                    move.w #6,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_trapv_err:
                    lea panic_number,a0
                    lea panic_pc,a1
                    move.w #7,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_priv_err:
                    lea panic_number,a0
                    lea panic_pc,a1
                    move.w #8,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

_handle_spurious:
                    lea panic_number,a0
                    lea panic_pc,a1
                    move.w #24,(a0)
                    move.l (2,a7),(a1)
                    bra call_panic

call_panic:         jsr panic
panic_lock:         bra panic_lock

;
; Function to make a system call based on the number of the system function:
; int32_t syscall(int32_t number, int32_t p0, int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5)
;
syscall:
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

            jsr syscall_dispatch       ; Call the C routine to do the dispatch
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
call_user:
            move.l (4,a7),a0            ; Get the pointer to the code to start
            move.l (8,a7),a1            ; Get the pointer to the process's stack
            move.l (12,a7),d0           ; Get the number of parameters passed
            move.l (16,a7),a2           ; Get the pointer to the parameters
            ; andi #0xdfff,sr              ; Drop into user mode
            movea.l a1,a7               ; Set the stack

            move.l a2,-(a7)             ; Push the parameters list
            move.l d0,-(a7)             ; Push the parameter count
            jsr (a0)

restart_cli:
            lea .sectionEnd stack,sp
            jsr cli_rerepl
            bra restart_cli
