	idnt	"bios_m68k.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_impl_bconout
	cnop	0,4
_impl_bconout
	movem.l	l3,-(a7)
	move.b	(7+l5,a7),d1
	move.l	_text_cursor_0,d0
	addq.l	#1,_text_cursor_0
	move.l	#16384,a0
	move.b	d1,(0,a0,d0.l)
	moveq	#0,d0
l1
l3	reg
l5	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bios_dispatch
	cnop	0,4
_bios_dispatch
	movem.l	l11,-(a7)
	move.l	(8+l13,a7),d3
	move.l	(4+l13,a7),d2
	move.l	d2,d0
	subq.l	#1,d0
	beq	l9
	bra	l10
l9
	move.l	d3,-(a7)
	jsr	_impl_bconout
	addq.w	#4,a7
	bra	l6
l10
	moveq	#-1,d0
l8
l6
l11	reg	d2/d3
	movem.l	(a7)+,d2/d3
l13	equ	8
	rts
; stacksize=16
	public	_text_cursor_0
	section	"DATA",data
	cnop	0,4
_text_cursor_0
	dc.l	0
