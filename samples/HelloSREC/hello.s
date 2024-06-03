;;;
;;; Sample MCP Program in SREC Format
;;;

				org $4

				dc.l start

                org $010000

start:          move.w #$05,d0						; Function: sys_int_enable_all
				trap #15

				move.l #$13,d0                      ; sys_chan_write
                clr.l d1                            ; Channel #0
                move.l #greet,d2                    ; Pointer to message
                move.l #greet_end-greet+1,d3        ; Length of message
                trap #15

loop:			move.l #$17,d0						; sys_chan_status
				move.l #0,d1						; Console channel
				trap #15
				andi.w #$04,d0						; Channel has data to read?
				beq loop							; No: keep waiting

				move.l #$14,d0						; sys_chan_write_b
				move.l #0,d1						; Console channel
				move.b #'>',d2						; Write the character back
				trap #15

				move.l #$11,d0						; sys_chan_read_b
				move.l #0,d1						; Console channel
				trap #15

				move.l d0,d2						; Write the character back
				move.l #$14,d0						; sys_chan_write_b
				move.l #0,d1						; Console channel
				trap #15

done:           clr.l d0                            ; sys_exit
                clr.l d1                            ; Return value = 0
                trap #15

greet:          dc.b "Hello, world!",10,13,"Press a key:"
greet_end:      dc.b 0


