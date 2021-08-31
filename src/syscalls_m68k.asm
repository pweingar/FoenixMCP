	idnt	"syscalls_m68k.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_bconout
	cnop	0,4
_bconout
	movem.l	l3,-(a7)
	move.b	(7+l5,a7),d2
	move.b	d2,d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	#1,-(a7)
	jsr	_bios
	addq.w	#8,a7
l1
l3	reg	d2
	movem.l	(a7)+,d2
l5	equ	4
	rts
	public	_bios
