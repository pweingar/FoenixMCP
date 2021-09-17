	idnt	"block.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_bdev_init_system
	cnop	0,4
_bdev_init_system
	movem.l	l7,-(a7)
	moveq	#0,d1
	bra	l4
l3
	moveq	#30,d0
	move.l	d0,d2
	move.l	d1,d3
	swap	d2
	swap	d3
	mulu.w	d1,d2
	mulu.w	d0,d3
	mulu.w	d1,d0
	add.w	d3,d2
	swap	d2
	clr.w	d2
	add.l	d2,d0
	lea	_g_block_devs,a0
	move.w	#0,(0,a0,d0.l)
	moveq	#30,d0
	move.l	d0,d2
	move.l	d1,d3
	swap	d2
	swap	d3
	mulu.w	d1,d2
	mulu.w	d0,d3
	mulu.w	d1,d0
	add.w	d3,d2
	swap	d2
	clr.w	d2
	add.l	d2,d0
	lea	_g_block_devs,a0
	move.l	#0,(2,a0,d0.l)
l6
	addq.l	#1,d1
l4
	moveq	#8,d0
	cmp.l	d1,d0
	bgt	l3
l5
l1
l7	reg	d2/d3
	movem.l	(a7)+,d2/d3
l9	equ	8
	rts
; stacksize=8
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_register
	cnop	0,4
_bdev_register
	movem.l	l15,-(a7)
	move.l	(4+l17,a7),a3
	move.w	(a3),d1
	cmp.w	#8,d1
	bge	l13
l12
	move.w	d1,d0
	ext.l	d0
	moveq	#30,d2
	move.l	d0,d3
	move.l	d2,d4
	swap	d3
	swap	d4
	mulu.w	d2,d3
	mulu.w	d0,d4
	mulu.w	d2,d0
	add.w	d4,d3
	swap	d3
	clr.w	d3
	add.l	d3,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	move.w	(a3),(a2)
	move.l	(2,a3),(2,a2)
	move.l	(6,a3),(6,a2)
	move.l	(10,a3),(10,a2)
	move.l	(14,a3),(14,a2)
	move.l	(18,a3),(18,a2)
	move.l	(22,a3),(22,a2)
	move.l	(26,a3),(26,a2)
	moveq	#0,d0
	bra	l10
l13
	moveq	#-2,d0
l14
l10
l15	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l17	equ	20
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_init
	cnop	0,4
_bdev_init
	subq.w	#8,a7
	movem.l	l25,-(a7)
	move.w	(14+l27,a7),d2
	cmp.w	#8,d2
	bge	l21
l20
	move.w	d2,d0
	ext.l	d0
	moveq	#30,d1
	move.l	d0,d3
	move.l	d1,d4
	swap	d3
	swap	d4
	mulu.w	d1,d3
	mulu.w	d0,d4
	mulu.w	d1,d0
	add.w	d4,d3
	swap	d3
	clr.w	d3
	add.l	d3,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l23
l22
	move.l	(6,a2),a1
	move.l	a1,(4+l27,a7)
	jsr	(a1)
	bra	l18
l23
	moveq	#-2,d0
l24
l21
l18
l25	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l27	equ	16
	addq.w	#8,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_read
	cnop	0,4
_bdev_read
	subq.w	#8,a7
	movem.l	l35,-(a7)
	move.w	(26+l37,a7),d4
	move.l	(16+l37,a7),d3
	move.w	(14+l37,a7),d2
	move.l	(20+l37,a7),a3
	cmp.w	#8,d2
	bge	l31
l30
	move.w	d2,d0
	ext.l	d0
	moveq	#30,d1
	move.l	d0,d5
	move.l	d1,d6
	swap	d5
	swap	d6
	mulu.w	d1,d5
	mulu.w	d0,d6
	mulu.w	d1,d0
	add.w	d6,d5
	swap	d5
	clr.w	d5
	add.l	d5,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l33
l32
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a3,-(a7)
	move.l	d3,-(a7)
	move.l	(10,a2),a1
	move.l	a1,(16+l37,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l28
l33
	moveq	#-2,d0
l34
l31
l28
l35	reg	a2/a3/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5/d6
l37	equ	28
	addq.w	#8,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_write
	cnop	0,4
_bdev_write
	subq.w	#8,a7
	movem.l	l45,-(a7)
	move.w	(26+l47,a7),d4
	move.l	(16+l47,a7),d3
	move.w	(14+l47,a7),d2
	move.l	(20+l47,a7),a3
	cmp.w	#8,d2
	bge	l41
l40
	move.w	d2,d0
	ext.l	d0
	moveq	#30,d1
	move.l	d0,d5
	move.l	d1,d6
	swap	d5
	swap	d6
	mulu.w	d1,d5
	mulu.w	d0,d6
	mulu.w	d1,d0
	add.w	d6,d5
	swap	d5
	clr.w	d5
	add.l	d5,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l43
l42
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a3,-(a7)
	move.l	d3,-(a7)
	move.l	(14,a2),a1
	move.l	a1,(16+l47,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l38
l43
	moveq	#-2,d0
l44
l41
l38
l45	reg	a2/a3/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5/d6
l47	equ	28
	addq.w	#8,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_status
	cnop	0,4
_bdev_status
	subq.w	#8,a7
	movem.l	l55,-(a7)
	move.w	(14+l57,a7),d2
	cmp.w	#8,d2
	bge	l51
l50
	move.w	d2,d0
	ext.l	d0
	moveq	#30,d1
	move.l	d0,d3
	move.l	d1,d4
	swap	d3
	swap	d4
	mulu.w	d1,d3
	mulu.w	d0,d4
	mulu.w	d1,d0
	add.w	d4,d3
	swap	d3
	clr.w	d3
	add.l	d3,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l53
l52
	move.l	(18,a2),a1
	move.l	a1,(4+l57,a7)
	jsr	(a1)
	bra	l48
l53
	moveq	#-2,d0
l54
l51
l48
l55	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l57	equ	16
	addq.w	#8,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_flush
	cnop	0,4
_bdev_flush
	subq.w	#8,a7
	movem.l	l65,-(a7)
	move.w	(14+l67,a7),d2
	cmp.w	#8,d2
	bge	l61
l60
	move.w	d2,d0
	ext.l	d0
	moveq	#30,d1
	move.l	d0,d3
	move.l	d1,d4
	swap	d3
	swap	d4
	mulu.w	d1,d3
	mulu.w	d0,d4
	mulu.w	d1,d0
	add.w	d4,d3
	swap	d3
	clr.w	d3
	add.l	d3,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l63
l62
	move.l	(22,a2),a1
	move.l	a1,(4+l67,a7)
	jsr	(a1)
	bra	l58
l63
	moveq	#-2,d0
l64
l61
l58
l65	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l67	equ	16
	addq.w	#8,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_bdev_ioctrl
	cnop	0,4
_bdev_ioctrl
	subq.w	#8,a7
	movem.l	l75,-(a7)
	move.w	(26+l77,a7),d4
	move.w	(18+l77,a7),d3
	move.w	(14+l77,a7),d2
	move.l	(20+l77,a7),a3
	cmp.w	#8,d2
	bge	l71
l70
	move.w	d2,d0
	ext.l	d0
	moveq	#30,d1
	move.l	d0,d5
	move.l	d1,d6
	swap	d5
	swap	d6
	mulu.w	d1,d5
	mulu.w	d0,d6
	mulu.w	d1,d0
	add.w	d6,d5
	swap	d5
	clr.w	d5
	add.l	d5,d0
	lea	_g_block_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l73
l72
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a3,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	(26,a2),a1
	move.l	a1,(16+l77,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l68
l73
	moveq	#-2,d0
l74
l71
l68
l75	reg	a2/a3/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5/d6
l77	equ	28
	addq.w	#8,a7
	rts
	public	_g_block_devs
	section	"BSS",bss
	cnop	0,4
_g_block_devs
	ds.b	240
