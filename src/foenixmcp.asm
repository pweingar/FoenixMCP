	idnt	"foenixmcp.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_main
	cnop	0,4
_main
	movem.l	l6,-(a7)
l3
	bra	l3
l5
	moveq	#0,d0
l1
l6	reg
l8	equ	0
	rts
; stacksize=0
