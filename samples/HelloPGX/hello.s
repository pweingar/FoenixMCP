;;;
;;; Sample PGX Program
;;;

                org $010000

                dc.b "PGX", $02
                dc.l _start

_start:         move.l #_greet_end-_greet+1,d3  ; Length of the message
                move.l #_greet,d2               ; Pointer to the message
                clr.l d1                        ; Write to channel #0 (main screen)
                move.l #$14,d0                  ; sys_chan_write_b
                trap #15                        ; Call it

_lock:          bra _lock

_greet:         dc.b "Hello, world!"
_greet_end:     dc.b 0
