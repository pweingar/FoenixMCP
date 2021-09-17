	idnt	"sid.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_sid_get_base
	cnop	0,4
_sid_get_base
	movem.l	l10,-(a7)
	move.w	(6+l12,a7),d1
	move.w	d1,d0
	sub.w	#0,d0
	beq	l4
	subq.w	#1,d0
	beq	l5
	subq.w	#1,d0
	beq	l6
	subq.w	#1,d0
	beq	l7
	subq.w	#1,d0
	beq	l8
	bra	l9
l4
	move.l	#12849664,d0
	bra	l1
l5
	move.l	#12718080,d0
	bra	l1
l6
	move.l	#12718592,d0
	bra	l1
l7
	move.l	#12716032,d0
	bra	l1
l8
	move.l	#12716288,d0
	bra	l1
l9
	moveq	#0,d0
l3
l1
l10	reg
l12	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_init
	cnop	0,4
_sid_init
	movem.l	l21,-(a7)
	move.w	(6+l23,a7),d3
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_sid_get_base
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l16
	moveq	#0,d2
	bra	l18
l17
	move.b	#0,(0,a2,d2.l)
l20
	addq.l	#1,d2
l18
	moveq	#25,d0
	cmp.l	d2,d0
	bgt	l17
l19
l16
l13
l21	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l23	equ	12
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_init_all
	cnop	0,4
_sid_init_all
	movem.l	l30,-(a7)
	moveq	#0,d2
	bra	l27
l26
	move.l	d2,-(a7)
	jsr	_sid_init
	addq.w	#4,a7
l29
	addq.l	#1,d2
l27
	moveq	#5,d0
	cmp.l	d2,d0
	bgt	l26
l28
l24
l30	reg	d2
	movem.l	(a7)+,d2
l32	equ	4
	rts
; stacksize=32
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_test_internal
	cnop	0,4
_sid_test_internal
	movem.l	l67,-(a7)
	move.b	#41,12718085
	move.b	#41,12718092
	move.b	#41,12718099
	move.b	#41,12718597
	move.b	#41,12718604
	move.b	#41,12718611
	move.b	#31,12718086
	move.b	#31,12718093
	move.b	#31,12718100
	move.b	#31,12718598
	move.b	#31,12718605
	move.b	#31,12718612
	move.b	#15,12718104
	move.b	#15,12718616
	move.b	#96,12718080
	move.b	#22,12718081
	move.b	#96,12718592
	move.b	#22,12718593
	move.b	#17,12718084
	move.b	#17,12718596
	moveq	#0,d1
	bra	l36
l35
l38
	addq.l	#1,d1
l36
	cmp.l	#65536,d1
	bcs	l35
l37
	move.b	#49,12718087
	move.b	#8,12718088
	move.b	#49,12718599
	move.b	#8,12718600
	move.b	#17,12718091
	move.b	#17,12718603
	moveq	#0,d1
	bra	l40
l39
l42
	addq.l	#1,d1
l40
	cmp.l	#65536,d1
	bcs	l39
l41
	move.b	#135,12718094
	move.b	#33,12718095
	move.b	#135,12718606
	move.b	#33,12718607
	move.b	#17,12718098
	move.b	#17,12718610
	moveq	#0,d1
	bra	l44
l43
l46
	addq.l	#1,d1
l44
	cmp.l	#262144,d1
	bcs	l43
l45
	move.b	#16,12718084
	move.b	#16,12718596
	moveq	#0,d1
	bra	l48
l47
l50
	addq.l	#1,d1
l48
	cmp.l	#8192,d1
	bcs	l47
l49
	move.b	#16,12718091
	move.b	#16,12718603
	moveq	#0,d1
	bra	l52
l51
l54
	addq.l	#1,d1
l52
	cmp.l	#8192,d1
	bcs	l51
l53
	move.b	#16,12718091
	move.b	#16,12718603
	moveq	#0,d1
	bra	l56
l55
l58
	addq.l	#1,d1
l56
	cmp.l	#32768,d1
	bcs	l55
l57
	moveq	#0,d2
	bra	l60
l59
	moveq	#0,d1
	bra	l64
l63
l66
	addq.l	#1,d1
l64
	cmp.l	#1024,d1
	bcs	l63
l65
	moveq	#15,d0
	sub.b	d2,d0
	move.b	d0,12718104
	moveq	#15,d0
	sub.b	d2,d0
	move.b	d0,12718616
l62
	addq.b	#1,d2
l60
	cmp.b	#16,d2
	bcs	l59
l61
l33
l67	reg	d2
	movem.l	(a7)+,d2
l69	equ	4
	rts
; stacksize=4
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_text_external
	cnop	0,4
_sid_text_external
	movem.l	l104,-(a7)
	move.b	#41,12716037
	move.b	#41,12716044
	move.b	#41,12716051
	move.b	#41,12716293
	move.b	#41,12716300
	move.b	#41,12716307
	move.b	#31,12716038
	move.b	#31,12716045
	move.b	#31,12716052
	move.b	#31,12716294
	move.b	#31,12716301
	move.b	#31,12716308
	move.b	#15,12716056
	move.b	#15,12716312
	move.b	#96,12716032
	move.b	#22,12716033
	move.b	#96,12716288
	move.b	#22,12716289
	move.b	#17,12716036
	move.b	#17,12716292
	moveq	#0,d1
	bra	l73
l72
l75
	addq.l	#1,d1
l73
	cmp.l	#65536,d1
	bcs	l72
l74
	move.b	#49,12716039
	move.b	#8,12716040
	move.b	#49,12716295
	move.b	#8,12716296
	move.b	#17,12716043
	move.b	#17,12716299
	moveq	#0,d1
	bra	l77
l76
l79
	addq.l	#1,d1
l77
	cmp.l	#65536,d1
	bcs	l76
l78
	move.b	#135,12716046
	move.b	#33,12716047
	move.b	#135,12716302
	move.b	#33,12716303
	move.b	#17,12716050
	move.b	#17,12716306
	moveq	#0,d1
	bra	l81
l80
l83
	addq.l	#1,d1
l81
	cmp.l	#262144,d1
	bcs	l80
l82
	move.b	#16,12716036
	move.b	#16,12716292
	moveq	#0,d1
	bra	l85
l84
l87
	addq.l	#1,d1
l85
	cmp.l	#8192,d1
	bcs	l84
l86
	move.b	#16,12716043
	move.b	#16,12716299
	moveq	#0,d1
	bra	l89
l88
l91
	addq.l	#1,d1
l89
	cmp.l	#8192,d1
	bcs	l88
l90
	move.b	#16,12716043
	move.b	#16,12716299
	moveq	#0,d1
	bra	l93
l92
l95
	addq.l	#1,d1
l93
	cmp.l	#32768,d1
	bcs	l92
l94
	moveq	#0,d2
	bra	l97
l96
	moveq	#0,d1
	bra	l101
l100
l103
	addq.l	#1,d1
l101
	cmp.l	#1024,d1
	bcs	l100
l102
	moveq	#15,d0
	sub.b	d2,d0
	move.b	d0,12716056
	moveq	#15,d0
	sub.b	d2,d0
	move.b	d0,12716312
l99
	addq.b	#1,d2
l97
	cmp.b	#16,d2
	bcs	l96
l98
l70
l104	reg	d2
	movem.l	(a7)+,d2
l106	equ	4
	rts
; stacksize=4
