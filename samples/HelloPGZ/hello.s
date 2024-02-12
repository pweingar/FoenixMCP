;;;
;;; Sample PGX Program
;;;

                org $010000

				; dc.b "z"

				; dc.l (greet_end-start+1)
				; dc.l start

				; align 2

start:          move.l #$13,d0                      ; sys_chan_write
                clr.l d1                            ; Channel #0
                move.l #greet,d2                    ; Pointer to message
                move.l #greet_end-greet+1,d3        ; Length of message
                trap #15

done:           clr.l d0                            ; sys_exit
                clr.l d1                            ; Return value = 0
                trap #15

greet:          dc.b "Hello, world!"
greet_end:      dc.b 0

				; dc.l 0, 0
