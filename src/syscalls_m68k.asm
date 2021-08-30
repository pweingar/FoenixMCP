	idnt	"syscalls_m68k.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_setexc
	cnop	0,4
_setexc
	movem.l	l3,-(a7)
	move.w	(6+l5,a7),d2
	move.l	(8+l5,a7),a2
	move.l	a2,-(a7)
	moveq	#0,d0
	move.w	d2,d0
	move.l	d0,-(a7)
	moveq	#0,d0
	move.w	_BIOS_SETEXC,d0
	move.l	d0,-(a7)
	jsr	_bios
	add.w	#12,a7
l1
l3	reg	a2/d2
	movem.l	(a7)+,a2/d2
l5	equ	8
	rts
	public	_BIOS_SETEXC
	cnop	0,4
_BIOS_SETEXC
	dc.w	5
	public	_bios
