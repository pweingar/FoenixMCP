	idnt	"log.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_DEBUG
	cnop	0,4
_DEBUG
	movem.l	l7,-(a7)
	move.l	(4+l9,a7),a2
	moveq	#0,d2
	bra	l4
l3
	lea	(a2,d2.l),a0
	move.b	(a0),d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	#0,-(a7)
	jsr	_text_put_raw
	addq.w	#8,a7
l6
	addq.l	#1,d2
l4
	move.l	a2,a0
	inline
	move.l	a0,d0
.l1
	tst.b	(a0)+
	bne	.l1
	sub.l	a0,d0
	not.l	d0
	einline
	cmp.l	d2,d0
	bhi	l3
l5
	move.l	#10,-(a7)
	move.l	#0,-(a7)
	jsr	_text_put_raw
	addq.w	#8,a7
l1
l7	reg	a2/d2
	movem.l	(a7)+,a2/d2
l9	equ	8
	rts
	public	_text_put_raw
