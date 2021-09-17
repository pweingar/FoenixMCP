	idnt	"ring_buffer.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_rb_word_init
	cnop	0,4
_rb_word_init
	movem.l	l3,-(a7)
	move.l	(4+l5,a7),a1
	move.w	#0,(256,a1)
	move.w	#0,(258,a1)
l1
l3	reg
l5	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_rb_word_full
	cnop	0,4
_rb_word_full
	movem.l	l11,-(a7)
	move.l	(4+l13,a7),a1
	moveq	#0,d0
	move.w	(256,a1),d0
	addq.l	#1,d0
	moveq	#0,d1
	move.w	(258,a1),d1
	cmp.l	d0,d1
	beq	l8
l10
	moveq	#0,d0
	bra	l9
l8
	moveq	#1,d0
l9
l6
l11	reg
l13	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_rb_word_empty
	cnop	0,4
_rb_word_empty
	movem.l	l19,-(a7)
	move.l	(4+l21,a7),a2
	move.w	(256,a2),d0
	cmp.w	(258,a2),d0
	beq	l16
l18
	moveq	#0,d0
	bra	l17
l16
	moveq	#1,d0
l17
l14
l19	reg	a2
	movem.l	(a7)+,a2
l21	equ	4
	rts
; stacksize=4
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_rb_word_put
	cnop	0,4
_rb_word_put
	movem.l	l28,-(a7)
	move.w	(10+l30,a7),d2
	move.l	(4+l30,a7),a2
	move.l	a2,-(a7)
	jsr	_rb_word_full
	addq.w	#4,a7
	tst.w	d0
	bne	l25
l24
	lea	(256,a2),a0
	move.w	(a0),d0
	addq.w	#1,(a0)
	and.l	#65535,d0
	lsl.l	#1,d0
	move.w	d2,(0,a2,d0.l)
	cmp.w	#128,(256,a2)
	bcs	l27
l26
	move.w	#0,(256,a2)
l27
l25
l22
l28	reg	a2/d2
	movem.l	(a7)+,a2/d2
l30	equ	8
	rts
; stacksize=16
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_rb_word_get
	cnop	0,4
_rb_word_get
	movem.l	l38,-(a7)
	move.l	(4+l40,a7),a2
	move.l	a2,-(a7)
	jsr	_rb_word_empty
	addq.w	#4,a7
	tst.w	d0
	bne	l34
l33
	lea	(258,a2),a0
	move.w	(a0),d0
	addq.w	#1,(a0)
	and.l	#65535,d0
	lsl.l	#1,d0
	move.w	(0,a2,d0.l),d2
	cmp.w	#128,(258,a2)
	bcs	l36
l35
	move.w	#0,(258,a2)
l36
	move.w	d2,d0
	bra	l31
l34
	moveq	#0,d0
l37
l31
l38	reg	a2/d2
	movem.l	(a7)+,a2/d2
l40	equ	8
	rts
; stacksize=20
