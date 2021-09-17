	idnt	"console.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_con_init
	cnop	0,4
_con_init
	movem.l	l3,-(a7)
	moveq	#0,d0
l1
l3	reg
l5	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_write_b
	cnop	0,4
_con_write_b
	movem.l	l8,-(a7)
	move.b	(11+l10,a7),d2
	move.l	(4+l10,a7),a2
	move.b	d2,d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.w	(2,a2),d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_put_raw
	moveq	#0,d0
	addq.w	#8,a7
l6
l8	reg	a2/d2
	movem.l	(a7)+,a2/d2
l10	equ	8
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_read_b
	cnop	0,4
_con_read_b
	movem.l	l13,-(a7)
	moveq	#0,d0
l11
l13	reg
l15	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_read
	cnop	0,4
_con_read
	movem.l	l27,-(a7)
	move.w	(14+l29,a7),d4
	move.l	(8+l29,a7),a3
	move.l	(4+l29,a7),a2
	moveq	#0,d3
	bra	l19
l18
	move.l	a2,-(a7)
	jsr	_con_read_b
	move.w	d0,d2
	addq.w	#4,a7
	bge	l23
l22
	move.w	d2,d0
	bra	l16
l23
	tst.w	d2
	ble	l26
l25
	move.w	d2,d0
	ext.l	d0
	and.l	#255,d0
	move.b	d0,(0,a3,d3.l)
l26
l24
l21
	addq.l	#1,d3
l19
	move.w	d4,d0
	ext.l	d0
	cmp.l	d3,d0
	bgt	l18
l20
	move.w	d3,d0
l16
l27	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l29	equ	20
	rts
; stacksize=28
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_readline
	cnop	0,4
_con_readline
	movem.l	l46,-(a7)
	move.w	(14+l48,a7),d4
	move.l	(4+l48,a7),a3
	move.l	(8+l48,a7),a2
	moveq	#0,d2
	bra	l33
l32
	move.l	a3,-(a7)
	jsr	_con_read_b
	move.w	d0,d3
	addq.w	#4,a7
	bge	l36
l35
	move.w	d3,d0
	bra	l30
l36
	tst.w	d3
	ble	l39
l38
	move.b	d3,d0
	and.b	#255,d0
	moveq	#0,d3
	move.b	d0,d3
	move.b	#0,(0,a2,d2.l)
	move.w	d3,d0
	subq.w	#8,d0
	beq	l42
	subq.w	#2,d0
	beq	l41
	bra	l45
l41
	move.b	#0,(0,a2,d2.l)
	move.w	d2,d0
	bra	l30
l42
	tst.l	d2
	ble	l44
l43
	subq.l	#1,d2
	move.b	#0,(0,a2,d2.l)
l44
	bra	l40
l45
	move.b	d3,d0
	move.l	d2,d1
	addq.l	#1,d2
	move.b	d0,(0,a2,d1.l)
	move.b	#0,(0,a2,d2.l)
l40
l39
l37
l33
	move.w	d4,d0
	ext.l	d0
	subq.l	#1,d0
	cmp.l	d2,d0
	bgt	l32
l34
	move.w	d2,d0
l30
l46	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l48	equ	20
	rts
; stacksize=28
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_write
	cnop	0,4
_con_write
	movem.l	l58,-(a7)
	move.w	(14+l60,a7),d4
	move.l	(8+l60,a7),a3
	move.l	(4+l60,a7),a2
	moveq	#0,d2
	bra	l52
l51
	move.b	(0,a3,d2.l),d3
	bne	l56
l55
	bra	l53
l56
	move.b	d3,d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.w	(2,a2),d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_put_raw
	addq.w	#8,a7
l57
l54
	addq.l	#1,d2
l52
	move.w	d4,d0
	ext.l	d0
	cmp.l	d2,d0
	bgt	l51
l53
	move.w	d2,d0
l49
l58	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l60	equ	20
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_status
	cnop	0,4
_con_status
	movem.l	l63,-(a7)
	moveq	#12,d0
l61
l63	reg
l65	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_flush
	cnop	0,4
_con_flush
	movem.l	l68,-(a7)
	moveq	#0,d0
l66
l68	reg
l70	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_seek
	cnop	0,4
_con_seek
	movem.l	l73,-(a7)
	moveq	#0,d0
l71
l73	reg
l75	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_ioctrl
	cnop	0,4
_con_ioctrl
	movem.l	l78,-(a7)
	moveq	#0,d0
l76
l78	reg
l80	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_con_install
	cnop	0,4
_con_install
	sub.w	#48,a7
	movem.l	l85,-(a7)
	move.l	#l83,(2+l87,a7)
	move.w	#0,(0+l87,a7)
	move.l	#_con_init,(6+l87,a7)
	move.l	#_con_read,(10+l87,a7)
	move.l	#_con_readline,(14+l87,a7)
	move.l	#_con_read_b,(18+l87,a7)
	move.l	#_con_write,(22+l87,a7)
	move.l	#_con_write_b,(26+l87,a7)
	move.l	#_con_flush,(34+l87,a7)
	move.l	#_con_seek,(38+l87,a7)
	move.l	#_con_status,(30+l87,a7)
	move.l	#_con_ioctrl,(42+l87,a7)
	lea	(0+l87,a7),a0
	move.l	a0,-(a7)
	jsr	_cdev_register
	move.l	#l84,(6+l87,a7)
	move.w	#1,(4+l87,a7)
	move.l	#_con_init,(10+l87,a7)
	move.l	#_con_read,(14+l87,a7)
	move.l	#_con_readline,(18+l87,a7)
	move.l	#_con_read_b,(22+l87,a7)
	move.l	#_con_write,(26+l87,a7)
	move.l	#_con_write_b,(30+l87,a7)
	move.l	#_con_flush,(38+l87,a7)
	move.l	#_con_seek,(42+l87,a7)
	move.l	#_con_status,(34+l87,a7)
	move.l	#_con_ioctrl,(46+l87,a7)
	lea	(4+l87,a7),a0
	move.l	a0,-(a7)
	jsr	_cdev_register
	addq.w	#8,a7
l81
l85	reg
l87	equ	0
	add.w	#48,a7
	rts
	cnop	0,4
l83
	dc.b	67
	dc.b	79
	dc.b	78
	dc.b	83
	dc.b	79
	dc.b	76
	dc.b	69
	dc.b	0
	cnop	0,4
l84
	dc.b	69
	dc.b	86
	dc.b	73
	dc.b	68
	dc.b	0
	public	_cdev_register
	public	_text_put_raw
