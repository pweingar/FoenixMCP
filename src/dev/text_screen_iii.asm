	idnt	"text_screen_iii.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_text_init
	cnop	0,4
_text_init
	movem.l	l4,-(a7)
	lea	l1,a2
	lea	46+l1,a3
	move.l	#12845056,(a2)
	move.l	#12976128,(4,a2)
	move.l	#13008896,(8,a2)
	move.l	#12845072,(12,a2)
	move.l	#12845076,(16,a2)
	move.l	#12845060,(20,a2)
	move.l	(a2),a0
	moveq	#1,d0
	move.l	d0,(a0)
	move.l	#0,-(a7)
	jsr	_text_setsizes
	move.l	#0,-(a7)
	move.l	#15,-(a7)
	move.l	#0,-(a7)
	jsr	_text_set_color
	move.l	#0,-(a7)
	jsr	_text_clear
	move.l	#0,-(a7)
	move.l	#0,-(a7)
	move.l	#0,-(a7)
	jsr	_text_set_xy
	move.l	#13107200,(a3)
	move.l	#13238272,(4,a3)
	move.l	#13271040,(8,a3)
	move.l	#13107216,(12,a3)
	move.l	#13107220,(16,a3)
	move.l	#13107204,(20,a3)
	move.l	(a3),a0
	moveq	#1,d0
	move.l	d0,(a0)
	move.l	#1,-(a7)
	jsr	_text_setsizes
	move.l	#0,-(a7)
	move.l	#15,-(a7)
	move.l	#1,-(a7)
	jsr	_text_set_color
	move.l	#1,-(a7)
	jsr	_text_clear
	move.l	#0,-(a7)
	move.l	#0,-(a7)
	move.l	#1,-(a7)
	jsr	_text_set_xy
	moveq	#0,d0
	add.w	#64,a7
l2
l4	reg	a2/a3
	movem.l	(a7)+,a2/a3
l6	equ	8
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_set_cursor
	cnop	0,4
_text_set_cursor
	movem.l	l11,-(a7)
	move.w	(22+l13,a7),d6
	move.w	(18+l13,a7),d5
	move.b	(15+l13,a7),d4
	move.w	(10+l13,a7),d3
	move.w	(6+l13,a7),d2
	cmp.w	#2,d2
	bge	l10
l9
	move.w	d2,d0
	ext.l	d0
	moveq	#46,d1
	move.l	d2,-(a7)
	move.l	d0,d7
	move.l	d1,d2
	swap	d7
	swap	d2
	mulu.w	d1,d7
	mulu.w	d0,d2
	mulu.w	d1,d0
	add.w	d2,d7
	swap	d7
	clr.w	d7
	add.l	d7,d0
	move.l	(a7)+,d2
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a1
	move.b	d3,d0
	and.b	#255,d0
	and.w	#255,d0
	and.l	#65535,d0
	moveq	#24,d1
	lsl.l	d1,d0
	move.b	d4,d1
	ext.w	d1
	ext.l	d1
	moveq	#16,d7
	lsl.l	d7,d1
	or.l	d1,d0
	move.b	d5,d1
	and.b	#2,d1
	and.w	#255,d1
	and.l	#65535,d1
	lsl.l	#1,d1
	or.l	d1,d0
	move.b	d6,d1
	and.b	#1,d1
	and.w	#255,d1
	and.l	#65535,d1
	or.l	d1,d0
	move.l	(12,a1),a0
	move.l	d0,(a0)
l10
l7
l11	reg	d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,d2/d3/d4/d5/d6/d7
l13	equ	24
	rts
; stacksize=28
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_set_xy
	cnop	0,4
_text_set_xy
	movem.l	l22,-(a7)
	move.w	(6+l24,a7),d5
	move.w	(10+l24,a7),d3
	move.w	(14+l24,a7),d2
	cmp.w	#2,d5
	bge	l17
l16
	move.w	d5,d0
	ext.l	d0
	moveq	#46,d1
	move.l	d0,d6
	move.l	d1,d7
	swap	d6
	swap	d7
	mulu.w	d1,d6
	mulu.w	d0,d7
	mulu.w	d1,d0
	add.w	d7,d6
	swap	d6
	clr.w	d6
	add.l	d6,d0
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a2
	moveq	#0,d0
	move.w	d3,d0
	move.w	(28,a2),d1
	ext.l	d1
	cmp.l	d0,d1
	bgt	l19
l18
	moveq	#0,d3
	addq.w	#1,d2
l19
	moveq	#0,d0
	move.w	d2,d0
	move.w	(30,a2),d1
	ext.l	d1
	cmp.l	d0,d1
	bgt	l21
l20
	move.w	(30,a2),d0
	subq.w	#1,d0
	move.w	d0,d2
	move.w	d5,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_scroll
	addq.w	#4,a7
l21
	move.w	d3,(32,a2)
	move.w	d2,(34,a2)
	moveq	#0,d0
	move.w	d2,d0
	moveq	#16,d1
	lsl.l	d1,d0
	moveq	#0,d1
	move.w	d3,d1
	or.l	d1,d0
	move.l	(16,a2),a0
	move.l	d0,(a0)
	move.w	d2,d0
	muls.w	(24,a2),d0
	move.w	d0,d4
	add.w	d3,d4
	move.l	(4,a2),a0
	add.w	d4,a0
	move.l	a0,(36,a2)
	move.l	(8,a2),a0
	add.w	d4,a0
	move.l	a0,(40,a2)
l17
l14
l22	reg	a2/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/d2/d3/d4/d5/d6/d7
l24	equ	28
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_setsizes
	cnop	0,4
_text_setsizes
	sub.w	#20,a7
	movem.l	l41,-(a7)
	cmp.w	#2,(26+l43,a7)
	bge	l28
l27
	moveq	#0,d2
	moveq	#0,d5
	moveq	#0,d6
	move.w	(26+l43,a7),d0
	ext.l	d0
	moveq	#46,d1
	move.l	d2,-(a7)
	move.l	d3,-(a7)
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
	move.l	(a7)+,d3
	move.l	(a7)+,d2
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a2
	move.l	(20,a2),a0
	move.l	(a0),d2
	move.l	(a2),a0
	move.l	#1024,d0
	and.l	(a0),d0
	move.w	d0,d5
	move.l	(a2),a0
	move.w	(2,a0),d0
	and.w	#768,d0
	and.l	#65535,d0
	lsr.l	#8,d0
	move.w	d0,d6
	move.w	d6,d0
	sub.w	#0,d0
	beq	l30
	subq.w	#1,d0
	beq	l31
	subq.w	#1,d0
	beq	l32
	subq.w	#1,d0
	beq	l33
	bra	l34
l30
	move.w	#80,(24,a2)
	move.w	#60,(26,a2)
	bra	l29
l31
	move.w	#100,(24,a2)
	move.w	#75,(26,a2)
	bra	l29
l32
	move.w	#128,(24,a2)
	move.w	#96,(26,a2)
	bra	l29
l33
	move.w	#80,(24,a2)
	move.w	#50,(26,a2)
	bra	l29
l34
l29
	tst.w	d5
	beq	l36
	lea	(24,a2),a0
	move.w	(a0),d0
	ext.l	d0
	move.l	d0,d1
	bge	l44
	addq.l	#1,d1
l44:
	asr.l	#1,d1
	move.w	d1,(a0)
	lea	(26,a2),a0
	move.w	(a0),d0
	ext.l	d0
	move.l	d0,d1
	bge	l45
	addq.l	#1,d1
l45:
	asr.l	#1,d1
	move.w	d1,(a0)
l36
	move.w	(26,a2),(30,a2)
	move.w	(24,a2),(28,a2)
	move.w	d2,d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	beq	l38
	move.w	d2,d0
	and.w	#16128,d0
	and.l	#65535,d0
	lsr.l	#8,d0
	move.w	d0,d7
	move.l	#4128768,d0
	and.l	d2,d0
	moveq	#16,d1
	lsr.l	d1,d0
	move.w	d0,(14+l43,a7)
	move.w	d7,d0
	ext.l	d0
	tst.l	d0
	bge	l46
	addq.l	#3,d0
l46:
	asr.l	#2,d0
	move.w	d0,d3
	move.w	(14+l43,a7),d0
	ext.l	d0
	tst.l	d0
	bge	l47
	addq.l	#3,d0
l47:
	asr.l	#2,d0
	move.w	d0,d4
	tst.w	d5
	beq	l40
	move.w	d3,d0
	ext.l	d0
	move.l	d0,d1
	bge	l48
	addq.l	#1,d1
l48:
	asr.l	#1,d1
	move.w	d1,d3
	move.w	d4,d0
	ext.l	d0
	move.l	d0,d1
	bge	l49
	addq.l	#1,d1
l49:
	asr.l	#1,d1
	move.w	d1,d4
l40
	lea	(28,a2),a0
	sub.w	d3,(a0)
	lea	(30,a2),a0
	sub.w	d4,(a0)
l38
l28
l25
l41	reg	a2/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/d2/d3/d4/d5/d6/d7
l43	equ	28
	add.w	#20,a7
	rts
; stacksize=56
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_set_color
	cnop	0,4
_text_set_color
	movem.l	l54,-(a7)
	move.w	(14+l56,a7),d4
	move.w	(10+l56,a7),d3
	move.w	(6+l56,a7),d2
	cmp.w	#2,d2
	bge	l53
l52
	move.w	d2,d0
	ext.l	d0
	moveq	#46,d1
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
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a1
	move.b	d3,d0
	and.b	#15,d0
	and.w	#255,d0
	and.l	#65535,d0
	lsl.l	#4,d0
	move.w	d4,d1
	ext.l	d1
	and.l	#15,d1
	or.l	d1,d0
	move.b	d0,(44,a1)
l53
l50
l54	reg	d2/d3/d4/d5/d6
	movem.l	(a7)+,d2/d3/d4/d5/d6
l56	equ	20
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_clear
	cnop	0,4
_text_clear
	movem.l	l65,-(a7)
	move.w	(6+l67,a7),d3
	cmp.w	#2,d3
	bge	l60
l59
	move.w	d3,d0
	ext.l	d0
	moveq	#46,d1
	move.l	d0,d4
	move.l	d1,d5
	swap	d4
	swap	d5
	mulu.w	d1,d4
	mulu.w	d0,d5
	mulu.w	d1,d0
	add.w	d5,d4
	swap	d4
	clr.w	d4
	add.l	d4,d0
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a2
	moveq	#0,d2
	bra	l62
l61
	move.l	(4,a2),a0
	move.b	#32,(0,a0,d2.l)
	move.l	(8,a2),a1
	move.b	(44,a2),(0,a1,d2.l)
l64
	addq.l	#1,d2
l62
	move.w	(24,a2),d0
	ext.l	d0
	move.w	(26,a2),d1
	ext.l	d1
	move.l	d0,d4
	move.l	d1,d5
	swap	d4
	swap	d5
	mulu.w	d1,d4
	mulu.w	d0,d5
	mulu.w	d1,d0
	add.w	d5,d4
	swap	d4
	clr.w	d4
	add.l	d4,d0
	cmp.l	d2,d0
	bgt	l61
l63
l60
l57
l65	reg	a2/d2/d3/d4/d5
	movem.l	(a7)+,a2/d2/d3/d4/d5
l67	equ	20
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_scroll
	cnop	0,4
_text_scroll
	sub.w	#28,a7
	movem.l	l84,-(a7)
	cmp.w	#2,(34+l86,a7)
	bge	l71
l70
	move.w	(34+l86,a7),d0
	ext.l	d0
	moveq	#46,d1
	move.l	d2,-(a7)
	move.l	d3,-(a7)
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
	move.l	(a7)+,d3
	move.l	(a7)+,d2
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a6
	moveq	#0,d3
	bra	l73
l72
	move.w	d3,d0
	muls.w	(24,a6),d0
	move.w	d0,d4
	moveq	#1,d0
	add.w	d3,d0
	muls.w	(24,a6),d0
	move.w	d0,d5
	move.l	(4,a6),a2
	add.w	d4,a2
	move.l	(8,a6),a3
	add.w	d4,a3
	move.l	(4,a6),a4
	add.w	d5,a4
	move.l	(8,a6),a5
	add.w	d5,a5
	moveq	#0,d2
	bra	l77
l76
	move.l	a4,a0
	addq.l	#2,a4
	move.l	a2,a1
	addq.l	#2,a2
	move.w	(a0),(a1)
	move.l	a5,a0
	addq.l	#2,a5
	move.l	a3,a1
	addq.l	#2,a3
	move.w	(a0),(a1)
l79
	addq.w	#2,d2
l77
	cmp.w	(24,a6),d2
	blt	l76
l78
l75
	addq.w	#1,d3
l73
	move.w	d3,d0
	ext.l	d0
	move.w	(30,a6),d1
	ext.l	d1
	subq.l	#1,d1
	cmp.l	d0,d1
	bgt	l72
l74
	move.w	(30,a6),d0
	subq.w	#1,d0
	muls.w	(24,a6),d0
	move.w	d0,d7
	move.l	(4,a6),a1
	add.w	d7,a1
	move.l	a1,(10+l86,a7)
	move.l	(8,a6),a1
	add.w	d7,a1
	move.l	a1,(14+l86,a7)
	move.b	(44,a6),d6
	moveq	#0,d2
	bra	l81
l80
	move.l	(10+l86,a7),a0
	addq.l	#2,(10+l86,a7)
	move.w	#32,(a0)
	move.l	(14+l86,a7),a0
	addq.l	#2,(14+l86,a7)
	moveq	#0,d0
	move.b	d6,d0
	move.w	d0,(a0)
l83
	addq.w	#2,d2
l81
	cmp.w	(24,a6),d2
	blt	l80
l82
l71
l68
l84	reg	a2/a3/a4/a5/a6/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/a3/a4/a5/a6/d2/d3/d4/d5/d6/d7
l86	equ	44
	add.w	#28,a7
	rts
; stacksize=80
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_put_raw
	cnop	0,4
_text_put_raw
	movem.l	l95,-(a7)
	move.b	(11+l97,a7),d3
	move.w	(6+l97,a7),d2
	cmp.w	#2,d2
	bge	l90
l89
	move.w	d2,d0
	ext.l	d0
	moveq	#46,d1
	move.l	d0,d4
	move.l	d1,d5
	swap	d4
	swap	d5
	mulu.w	d1,d4
	mulu.w	d0,d5
	mulu.w	d1,d0
	add.w	d5,d4
	swap	d4
	clr.w	d4
	add.l	d4,d0
	lea	l1,a0
	add.l	d0,a0
	move.l	a0,a3
	move.b	d3,d0
	sub.b	#10,d0
	beq	l92
	subq.b	#3,d0
	beq	l93
	bra	l94
l92
	move.w	(34,a3),d0
	ext.l	d0
	addq.l	#1,d0
	move.l	d0,-(a7)
	move.l	#0,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_set_xy
	add.w	#12,a7
	bra	l91
l93
	bra	l91
l94
	lea	(36,a3),a0
	move.l	(a0),a1
	addq.l	#1,(a0)
	move.b	d3,(a1)
	lea	(40,a3),a1
	move.l	(a1),a2
	addq.l	#1,(a1)
	move.b	(44,a3),(a2)
	move.w	(34,a3),d0
	ext.l	d0
	move.l	d0,-(a7)
	move.w	(32,a3),d0
	ext.l	d0
	addq.l	#1,d0
	move.l	d0,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_set_xy
	add.w	#12,a7
l91
l90
l87
l95	reg	a2/a3/d2/d3/d4/d5
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5
l97	equ	24
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_put_ansi
	cnop	0,4
_text_put_ansi
	movem.l	l102,-(a7)
	move.w	(6+l104,a7),d0
l100
l101
l98
l102	reg
l104	equ	0
	rts
; stacksize=0
	section	"BSS",bss
	cnop	0,4
l1
	ds.b	92
