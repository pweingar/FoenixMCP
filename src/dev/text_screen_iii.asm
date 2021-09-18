	idnt	"text_screen_iii.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_text_init
	cnop	0,4
_text_init
	movem.l	l8,-(a7)
	lea	l1,a2
	lea	46+l1,a3
	moveq	#0,d2
	bra	l5
l4
	move.l	d2,d0
	lsl.l	#1,d0
	lea	_fg_color_lut,a0
	add.l	d0,a0
	move.l	d2,d0
	lsl.l	#1,d0
	move.l	#13026304,a1
	move.w	(a0),(0,a1,d0.l)
	move.l	d2,d0
	lsl.l	#1,d0
	lea	_fg_color_lut,a0
	add.l	d0,a0
	move.l	d2,d0
	lsl.l	#1,d0
	move.l	#13288448,a1
	move.w	(a0),(0,a1,d0.l)
	move.l	d2,d0
	lsl.l	#1,d0
	lea	_bg_color_lut,a0
	add.l	d0,a0
	move.l	d2,d0
	lsl.l	#1,d0
	move.l	#13026368,a1
	move.w	(a0),(0,a1,d0.l)
	move.l	d2,d0
	lsl.l	#1,d0
	lea	_bg_color_lut,a0
	add.l	d0,a0
	move.l	d2,d0
	lsl.l	#1,d0
	move.l	#13288512,a1
	move.w	(a0),(0,a1,d0.l)
l7
	addq.l	#1,d2
l5
	moveq	#32,d0
	cmp.l	d2,d0
	bgt	l4
l6
	move.l	#12845056,(a2)
	move.l	#12976128,(4,a2)
	move.l	#13008896,(8,a2)
	move.l	#12845072,(12,a2)
	move.l	#12845076,(16,a2)
	move.l	#12845060,(20,a2)
	move.l	(a2),a0
	moveq	#1,d0
	move.l	d0,(a0)
	move.l	(20,a2),a0
	move.l	#1056769,(a0)
	move.l	(20,a2),a0
	moveq	#64,d0
	move.l	d0,(4,a0)
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
	move.l	(20,a3),a0
	move.l	#1056768,(a0)
	move.l	(20,a3),a0
	move.l	#4194304,(4,a0)
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
l8	reg	a2/a3/d2
	movem.l	(a7)+,a2/a3/d2
l10	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_set_cursor
	cnop	0,4
_text_set_cursor
	movem.l	l15,-(a7)
	move.w	(22+l17,a7),d6
	move.w	(18+l17,a7),d5
	move.b	(15+l17,a7),d4
	move.w	(10+l17,a7),d3
	move.w	(6+l17,a7),d2
	cmp.w	#2,d2
	bge	l14
l13
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
l14
l11
l15	reg	d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,d2/d3/d4/d5/d6/d7
l17	equ	24
	rts
; stacksize=28
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_set_xy
	cnop	0,4
_text_set_xy
	movem.l	l26,-(a7)
	move.w	(6+l28,a7),d5
	move.w	(10+l28,a7),d3
	move.w	(14+l28,a7),d2
	cmp.w	#2,d5
	bge	l21
l20
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
	bgt	l23
l22
	moveq	#0,d3
	addq.w	#1,d2
l23
	moveq	#0,d0
	move.w	d2,d0
	move.w	(30,a2),d1
	ext.l	d1
	cmp.l	d0,d1
	bgt	l25
l24
	move.w	(30,a2),d0
	subq.w	#1,d0
	move.w	d0,d2
	move.w	d5,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_scroll
	addq.w	#4,a7
l25
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
l21
l18
l26	reg	a2/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/d2/d3/d4/d5/d6/d7
l28	equ	28
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_setsizes
	cnop	0,4
_text_setsizes
	sub.w	#20,a7
	movem.l	l45,-(a7)
	cmp.w	#2,(26+l47,a7)
	bge	l32
l31
	moveq	#0,d2
	moveq	#0,d5
	moveq	#0,d6
	move.w	(26+l47,a7),d0
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
	beq	l34
	subq.w	#1,d0
	beq	l35
	subq.w	#1,d0
	beq	l36
	subq.w	#1,d0
	beq	l37
	bra	l38
l34
	move.w	#80,(24,a2)
	move.w	#60,(26,a2)
	bra	l33
l35
	move.w	#100,(24,a2)
	move.w	#75,(26,a2)
	bra	l33
l36
	move.w	#128,(24,a2)
	move.w	#96,(26,a2)
	bra	l33
l37
	move.w	#80,(24,a2)
	move.w	#50,(26,a2)
	bra	l33
l38
l33
	tst.w	d5
	beq	l40
	lea	(24,a2),a0
	move.w	(a0),d0
	ext.l	d0
	move.l	d0,d1
	bge	l48
	addq.l	#1,d1
l48:
	asr.l	#1,d1
	move.w	d1,(a0)
	lea	(26,a2),a0
	move.w	(a0),d0
	ext.l	d0
	move.l	d0,d1
	bge	l49
	addq.l	#1,d1
l49:
	asr.l	#1,d1
	move.w	d1,(a0)
l40
	move.w	(26,a2),(30,a2)
	move.w	(24,a2),(28,a2)
	move.w	d2,d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	beq	l42
	move.w	d2,d0
	and.w	#16128,d0
	and.l	#65535,d0
	lsr.l	#8,d0
	move.w	d0,d7
	move.l	#4128768,d0
	and.l	d2,d0
	moveq	#16,d1
	lsr.l	d1,d0
	move.w	d0,(14+l47,a7)
	move.w	d7,d0
	ext.l	d0
	tst.l	d0
	bge	l50
	addq.l	#3,d0
l50:
	asr.l	#2,d0
	move.w	d0,d3
	move.w	(14+l47,a7),d0
	ext.l	d0
	tst.l	d0
	bge	l51
	addq.l	#3,d0
l51:
	asr.l	#2,d0
	move.w	d0,d4
	tst.w	d5
	beq	l44
	move.w	d3,d0
	ext.l	d0
	move.l	d0,d1
	bge	l52
	addq.l	#1,d1
l52:
	asr.l	#1,d1
	move.w	d1,d3
	move.w	d4,d0
	ext.l	d0
	move.l	d0,d1
	bge	l53
	addq.l	#1,d1
l53:
	asr.l	#1,d1
	move.w	d1,d4
l44
	lea	(28,a2),a0
	sub.w	d3,(a0)
	lea	(30,a2),a0
	sub.w	d4,(a0)
l42
l32
l29
l45	reg	a2/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/d2/d3/d4/d5/d6/d7
l47	equ	28
	add.w	#20,a7
	rts
; stacksize=56
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_set_color
	cnop	0,4
_text_set_color
	movem.l	l58,-(a7)
	move.w	(14+l60,a7),d4
	move.w	(10+l60,a7),d3
	move.w	(6+l60,a7),d2
	cmp.w	#2,d2
	bge	l57
l56
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
l57
l54
l58	reg	d2/d3/d4/d5/d6
	movem.l	(a7)+,d2/d3/d4/d5/d6
l60	equ	20
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_clear
	cnop	0,4
_text_clear
	movem.l	l69,-(a7)
	move.w	(6+l71,a7),d3
	cmp.w	#2,d3
	bge	l64
l63
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
	bra	l66
l65
	move.l	(4,a2),a0
	move.b	#32,(0,a0,d2.l)
	move.l	(8,a2),a1
	move.b	(44,a2),(0,a1,d2.l)
l68
	addq.l	#1,d2
l66
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
	bgt	l65
l67
l64
l61
l69	reg	a2/d2/d3/d4/d5
	movem.l	(a7)+,a2/d2/d3/d4/d5
l71	equ	20
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_scroll
	cnop	0,4
_text_scroll
	sub.w	#28,a7
	movem.l	l88,-(a7)
	cmp.w	#2,(34+l90,a7)
	bge	l75
l74
	move.w	(34+l90,a7),d0
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
	bra	l77
l76
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
	bra	l81
l80
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
l83
	addq.w	#2,d2
l81
	cmp.w	(24,a6),d2
	blt	l80
l82
l79
	addq.w	#1,d3
l77
	move.w	d3,d0
	ext.l	d0
	move.w	(30,a6),d1
	ext.l	d1
	subq.l	#1,d1
	cmp.l	d0,d1
	bgt	l76
l78
	move.w	(30,a6),d0
	subq.w	#1,d0
	muls.w	(24,a6),d0
	move.w	d0,d7
	move.l	(4,a6),a1
	add.w	d7,a1
	move.l	a1,(10+l90,a7)
	move.l	(8,a6),a1
	add.w	d7,a1
	move.l	a1,(14+l90,a7)
	move.b	(44,a6),d6
	moveq	#0,d2
	bra	l85
l84
	move.l	(10+l90,a7),a0
	addq.l	#2,(10+l90,a7)
	move.w	#32,(a0)
	move.l	(14+l90,a7),a0
	addq.l	#2,(14+l90,a7)
	moveq	#0,d0
	move.b	d6,d0
	move.w	d0,(a0)
l87
	addq.w	#2,d2
l85
	cmp.w	(24,a6),d2
	blt	l84
l86
l75
l72
l88	reg	a2/a3/a4/a5/a6/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/a3/a4/a5/a6/d2/d3/d4/d5/d6/d7
l90	equ	44
	add.w	#28,a7
	rts
; stacksize=80
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_put_raw
	cnop	0,4
_text_put_raw
	movem.l	l99,-(a7)
	move.b	(11+l101,a7),d3
	move.w	(6+l101,a7),d2
	cmp.w	#2,d2
	bge	l94
l93
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
	beq	l96
	subq.b	#3,d0
	beq	l97
	bra	l98
l96
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
	bra	l95
l97
	bra	l95
l98
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
l95
l94
l91
l99	reg	a2/a3/d2/d3/d4/d5
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5
l101	equ	24
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_text_put_ansi
	cnop	0,4
_text_put_ansi
	movem.l	l106,-(a7)
	move.w	(6+l108,a7),d0
l104
l105
l102
l106	reg
l108	equ	0
	rts
; stacksize=0
	public	_fg_color_lut
	cnop	0,4
_fg_color_lut
	dc.w	0
	dc.w	65280
	dc.w	0
	dc.w	65408
	dc.w	32768
	dc.w	65280
	dc.w	128
	dc.w	65280
	dc.w	32768
	dc.w	65408
	dc.w	32896
	dc.w	65280
	dc.w	128
	dc.w	65408
	dc.w	32896
	dc.w	65408
	dc.w	17664
	dc.w	65535
	dc.w	17683
	dc.w	65419
	dc.w	0
	dc.w	65312
	dc.w	8192
	dc.w	65280
	dc.w	32
	dc.w	65280
	dc.w	8224
	dc.w	65312
	dc.w	16448
	dc.w	65344
	dc.w	65535
	dc.w	65535
	public	_bg_color_lut
	cnop	0,4
_bg_color_lut
	dc.w	0
	dc.w	65280
	dc.w	0
	dc.w	65408
	dc.w	32768
	dc.w	65280
	dc.w	128
	dc.w	65280
	dc.w	8192
	dc.w	65312
	dc.w	8224
	dc.w	65280
	dc.w	32
	dc.w	65312
	dc.w	8224
	dc.w	65312
	dc.w	26910
	dc.w	65490
	dc.w	17683
	dc.w	65419
	dc.w	0
	dc.w	65312
	dc.w	8192
	dc.w	65280
	dc.w	32
	dc.w	65280
	dc.w	4112
	dc.w	65296
	dc.w	16448
	dc.w	65344
	dc.w	65535
	dc.w	65535
	section	"BSS",bss
	cnop	0,4
l1
	ds.b	92
