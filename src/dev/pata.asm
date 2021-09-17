	idnt	"pata.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_pata_wait_not_busy
	cnop	0,4
_pata_wait_not_busy
	movem.l	l11,-(a7)
	move.w	#10000,d2
	pea	l3
	jsr	_DEBUG
	addq.w	#4,a7
l4
	move.b	12583950,d3
l6
	move.b	d3,d0
	ext.w	d0
	and.b	#128,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l5
l7
	move.w	d2,d0
	subq.w	#1,d2
	tst.w	d0
	bgt	l4
l5
	tst.w	d2
	bne	l9
l8
	moveq	#-3,d0
	bra	l1
l9
	moveq	#0,d0
l10
l1
l11	reg	d2/d3
	movem.l	(a7)+,d2/d3
l13	equ	8
	rts
	cnop	0,4
l3
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	119
	dc.b	97
	dc.b	105
	dc.b	116
	dc.b	95
	dc.b	110
	dc.b	111
	dc.b	116
	dc.b	95
	dc.b	98
	dc.b	117
	dc.b	115
	dc.b	121
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_wait_ready
	cnop	0,4
_pata_wait_ready
	movem.l	l24,-(a7)
	move.w	#10000,d2
	pea	l16
	jsr	_DEBUG
	addq.w	#4,a7
l17
	move.b	12583950,d3
l19
	move.b	d3,d0
	ext.w	d0
	and.b	#64,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l18
l20
	move.w	d2,d0
	subq.w	#1,d2
	tst.w	d0
	bgt	l17
l18
	tst.w	d2
	bne	l22
l21
	moveq	#-3,d0
	bra	l14
l22
	moveq	#0,d0
l23
l14
l24	reg	d2/d3
	movem.l	(a7)+,d2/d3
l26	equ	8
	rts
	cnop	0,4
l16
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	119
	dc.b	97
	dc.b	105
	dc.b	116
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	121
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_wait_ready_not_busy
	cnop	0,4
_pata_wait_ready_not_busy
	movem.l	l37,-(a7)
	move.w	#10000,d2
	pea	l29
	jsr	_DEBUG
	addq.w	#4,a7
l30
	move.b	12583950,d3
l32
	move.b	d3,d0
	ext.w	d0
	and.b	#192,d0
	and.w	#255,d0
	and.l	#65535,d0
	moveq	#64,d1
	cmp.l	d0,d1
	beq	l31
l33
	move.w	d2,d0
	subq.w	#1,d2
	tst.w	d0
	bgt	l30
l31
	tst.w	d2
	bne	l35
l34
	moveq	#-3,d0
	bra	l27
l35
	moveq	#0,d0
l36
l27
l37	reg	d2/d3
	movem.l	(a7)+,d2/d3
l39	equ	8
	rts
	cnop	0,4
l29
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	119
	dc.b	97
	dc.b	105
	dc.b	116
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	121
	dc.b	95
	dc.b	110
	dc.b	111
	dc.b	116
	dc.b	95
	dc.b	98
	dc.b	117
	dc.b	115
	dc.b	121
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_wait_data_request
	cnop	0,4
_pata_wait_data_request
	movem.l	l50,-(a7)
	move.w	#10000,d2
	pea	l42
	jsr	_DEBUG
	addq.w	#4,a7
l43
	move.b	12583950,d3
l45
	move.b	d3,d0
	ext.w	d0
	and.b	#8,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l44
l46
	move.w	d2,d0
	subq.w	#1,d2
	tst.w	d0
	bgt	l43
l44
	tst.w	d2
	bne	l48
l47
	moveq	#-3,d0
	bra	l40
l48
	moveq	#0,d0
l49
l40
l50	reg	d2/d3
	movem.l	(a7)+,d2/d3
l52	equ	8
	rts
	cnop	0,4
l42
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	119
	dc.b	97
	dc.b	105
	dc.b	116
	dc.b	95
	dc.b	100
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	113
	dc.b	117
	dc.b	101
	dc.b	115
	dc.b	116
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_identity
	cnop	0,4
_pata_identity
	movem.l	l66,-(a7)
	move.l	(4+l68,a7),a2
	pea	l55
	jsr	_DEBUG
	move.b	#224,12583948
	move.b	#1,12583940
	move.b	#0,12583942
	move.b	#0,12583944
	move.b	#0,12583946
	move.b	#236,12583950
	jsr	_pata_wait_not_busy
	addq.w	#4,a7
	tst.w	d0
	beq	l57
	moveq	#-3,d0
	bra	l53
l57
	jsr	_pata_wait_ready_not_busy
	tst.w	d0
	beq	l59
	moveq	#-3,d0
	bra	l53
l59
	pea	l60
	jsr	_DEBUG
	lea	_g_buffer,a3
	moveq	#0,d3
	addq.w	#4,a7
	bra	l62
l61
	move.w	12583936,d4
	moveq	#0,d0
	move.w	d4,d0
	and.l	#255,d0
	move.w	d3,d1
	addq.w	#1,d3
	lea	_g_buffer,a0
	move.b	d0,(0,a0,d1.w)
	moveq	#0,d0
	move.w	d4,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.w	d3,d1
	addq.w	#1,d3
	lea	_g_buffer,a0
	move.b	d0,(0,a0,d1.w)
l64
l62
	cmp.w	#512,d3
	blt	l61
l63
	pea	l65
	jsr	_DEBUG
	move.l	a4,a3
	move.b	1+_g_buffer,d0
	ext.w	d0
	ext.l	d0
	moveq	#16,d1
	lsl.l	d1,d0
	move.b	_g_buffer,d1
	ext.w	d1
	ext.l	d1
	or.l	d1,d0
	move.w	d0,(a2)
	move.b	99+_g_buffer,d0
	ext.w	d0
	ext.l	d0
	moveq	#16,d1
	lsl.l	d1,d0
	move.b	98+_g_buffer,d1
	ext.w	d1
	ext.l	d1
	or.l	d1,d0
	move.w	d0,(64,a2)
	move.b	121+_g_buffer,d0
	ext.w	d0
	ext.l	d0
	lsl.l	#8,d0
	move.b	120+_g_buffer,d1
	ext.w	d1
	ext.l	d1
	or.l	d1,d0
	move.w	d0,(66,a2)
	move.b	123+_g_buffer,d0
	ext.w	d0
	ext.l	d0
	lsl.l	#8,d0
	move.b	122+_g_buffer,d1
	ext.w	d1
	ext.l	d1
	or.l	d1,d0
	lea	(66,a2),a0
	move.w	d0,(2,a0)
	moveq	#18,d2
	lea	22+_g_buffer,a1
	lea	(2,a2),a0
	inline
	move.l	a0,d0
	cmp.l	#16,d2
	blo	.l5
	moveq	#1,d1
	and.b	d0,d1
	beq	.l1
	move.b	(a1)+,(a0)+
	subq.l	#1,d2
.l1
	move.l	a1,d1
	and.b	#1,d1
	beq	.l3
	cmp.l	#$10000,d2
	blo	.l5
.l2
	move.b	(a1)+,(a0)+
	subq.l	#1,d2
	bne	.l2
	bra	.l7
.l3
	moveq	#3,d1
	and.l	d2,d1
	sub.l	d1,d2
.l4
	move.l	(a1)+,(a0)+
	subq.l	#4,d2
	bne	.l4
	move.w	d1,d2
.l5
	subq.w	#1,d2
	blo	.l7
.l6
	move.b	(a1)+,(a0)+
	dbf	d2,.l6
.l7
	einline
	moveq	#6,d2
	lea	46+_g_buffer,a1
	lea	(20,a2),a0
	inline
	move.l	a0,d0
	cmp.l	#16,d2
	blo	.l5
	moveq	#1,d1
	and.b	d0,d1
	beq	.l1
	move.b	(a1)+,(a0)+
	subq.l	#1,d2
.l1
	move.l	a1,d1
	and.b	#1,d1
	beq	.l3
	cmp.l	#$10000,d2
	blo	.l5
.l2
	move.b	(a1)+,(a0)+
	subq.l	#1,d2
	bne	.l2
	bra	.l7
.l3
	moveq	#3,d1
	and.l	d2,d1
	sub.l	d1,d2
.l4
	move.l	(a1)+,(a0)+
	subq.l	#4,d2
	bne	.l4
	move.w	d1,d2
.l5
	subq.w	#1,d2
	blo	.l7
.l6
	move.b	(a1)+,(a0)+
	dbf	d2,.l6
.l7
	einline
	moveq	#38,d2
	lea	54+_g_buffer,a1
	lea	(26,a2),a0
	inline
	move.l	a0,d0
	cmp.l	#16,d2
	blo	.l5
	moveq	#1,d1
	and.b	d0,d1
	beq	.l1
	move.b	(a1)+,(a0)+
	subq.l	#1,d2
.l1
	move.l	a1,d1
	and.b	#1,d1
	beq	.l3
	cmp.l	#$10000,d2
	blo	.l5
.l2
	move.b	(a1)+,(a0)+
	subq.l	#1,d2
	bne	.l2
	bra	.l7
.l3
	moveq	#3,d1
	and.l	d2,d1
	sub.l	d1,d2
.l4
	move.l	(a1)+,(a0)+
	subq.l	#4,d2
	bne	.l4
	move.w	d1,d2
.l5
	subq.w	#1,d2
	blo	.l7
.l6
	move.b	(a1)+,(a0)+
	dbf	d2,.l6
.l7
	einline
	moveq	#0,d0
	addq.w	#4,a7
l53
l66	reg	a2/a3/a4/d2/d3/d4
	movem.l	(a7)+,a2/a3/a4/d2/d3/d4
l68	equ	24
	rts
	cnop	0,4
l55
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	105
	dc.b	100
	dc.b	101
	dc.b	110
	dc.b	116
	dc.b	105
	dc.b	116
	dc.b	121
	dc.b	0
	cnop	0,4
l60
	dc.b	99
	dc.b	111
	dc.b	112
	dc.b	121
	dc.b	105
	dc.b	110
	dc.b	103
	dc.b	32
	dc.b	100
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	0
	cnop	0,4
l65
	dc.b	100
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	32
	dc.b	99
	dc.b	111
	dc.b	112
	dc.b	105
	dc.b	101
	dc.b	100
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_init
	cnop	0,4
_pata_init
	movem.l	l76,-(a7)
	pea	l71
	jsr	_DEBUG
	move.b	#0,12583950
	jsr	_pata_wait_not_busy
	addq.w	#4,a7
	tst.w	d0
	beq	l73
	moveq	#-3,d0
	bra	l69
l73
	move.b	#160,12583948
	move.b	#1,12583940
	move.b	#0,12583942
	move.b	#0,12583944
	move.b	#0,12583946
	jsr	_pata_wait_ready_not_busy
	tst.w	d0
	beq	l75
	moveq	#-3,d0
	bra	l69
l75
	move.w	#2,_g_pata_status
	moveq	#0,d0
l69
l76	reg
l78	equ	0
	rts
	cnop	0,4
l71
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_read
	cnop	0,4
_pata_read
	movem.l	l92,-(a7)
	move.l	(4+l94,a7),d4
	move.w	(14+l94,a7),d3
	move.l	(8+l94,a7),a3
	pea	l81
	jsr	_DEBUG
	jsr	_pata_wait_ready_not_busy
	addq.w	#4,a7
	tst.w	d0
	beq	l83
	moveq	#-3,d0
	bra	l79
l83
	moveq	#24,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#7,d0
	or.l	#224,d0
	move.b	d0,12583948
	jsr	_pata_wait_ready_not_busy
	tst.w	d0
	beq	l85
	moveq	#-3,d0
	bra	l79
l85
	move.b	#1,12583940
	move.l	#255,d0
	and.l	d4,d0
	move.b	d0,12583942
	move.l	d4,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.b	d0,12583944
	moveq	#16,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#255,d0
	move.b	d0,12583944
	move.b	#33,12583950
	jsr	_pata_wait_ready_not_busy
	tst.w	d0
	beq	l87
	moveq	#-3,d0
	bra	l79
l87
	moveq	#0,d2
	move.l	a3,a2
	bra	l89
l88
	move.l	a2,a0
	addq.l	#2,a2
	move.w	12583936,(a0)
l91
	addq.w	#2,d2
l89
	cmp.w	d2,d3
	bgt	l88
l90
	move.w	d2,d0
l79
l92	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l94	equ	20
	rts
	cnop	0,4
l81
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_write
	cnop	0,4
_pata_write
	movem.l	l108,-(a7)
	move.l	(4+l110,a7),d4
	move.w	(14+l110,a7),d3
	move.l	(8+l110,a7),a3
	pea	l97
	jsr	_DEBUG
	jsr	_pata_wait_ready_not_busy
	addq.w	#4,a7
	tst.w	d0
	beq	l99
	moveq	#-3,d0
	bra	l95
l99
	moveq	#24,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#7,d0
	or.l	#224,d0
	move.b	d0,12583948
	jsr	_pata_wait_ready_not_busy
	tst.w	d0
	beq	l101
	moveq	#-3,d0
	bra	l95
l101
	move.b	#1,12583940
	move.l	#255,d0
	and.l	d4,d0
	move.b	d0,12583942
	move.l	d4,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.b	d0,12583944
	moveq	#16,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#255,d0
	move.b	d0,12583944
	move.b	#48,12583950
	jsr	_pata_wait_ready_not_busy
	tst.w	d0
	beq	l103
	moveq	#-3,d0
	bra	l95
l103
	moveq	#0,d2
	move.l	a3,a2
	bra	l105
l104
	move.l	a2,a0
	addq.l	#2,a2
	move.w	(a0),12583936
l107
	addq.w	#2,d2
l105
	cmp.w	d2,d3
	bgt	l104
l106
	moveq	#0,d0
l95
l108	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l110	equ	20
	rts
	cnop	0,4
l97
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	119
	dc.b	114
	dc.b	105
	dc.b	116
	dc.b	101
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_status
	cnop	0,4
_pata_status
	movem.l	l114,-(a7)
	pea	l113
	jsr	_DEBUG
	move.w	_g_pata_status,d0
	addq.w	#4,a7
l111
l114	reg
l116	equ	0
	rts
	cnop	0,4
l113
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	116
	dc.b	117
	dc.b	115
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_error
	cnop	0,4
_pata_error
	movem.l	l120,-(a7)
	pea	l119
	jsr	_DEBUG
	move.w	_g_pata_error,d0
	addq.w	#4,a7
l117
l120	reg
l122	equ	0
	rts
	cnop	0,4
l119
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	101
	dc.b	114
	dc.b	114
	dc.b	111
	dc.b	114
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_flush
	cnop	0,4
_pata_flush
	movem.l	l126,-(a7)
	pea	l125
	jsr	_DEBUG
	moveq	#0,d0
	addq.w	#4,a7
l123
l126	reg
l128	equ	0
	rts
	cnop	0,4
l125
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	102
	dc.b	108
	dc.b	117
	dc.b	115
	dc.b	104
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_ioctrl
	cnop	0,4
_pata_ioctrl
	sub.w	#88,a7
	movem.l	l142,-(a7)
	move.w	(94+l144,a7),d3
	move.l	(96+l144,a7),a2
	pea	l131
	jsr	_DEBUG
	move.w	d3,d0
	subq.w	#1,d0
	addq.w	#4,a7
	beq	l133
	subq.w	#1,d0
	beq	l136
	subq.w	#1,d0
	beq	l137
	subq.w	#1,d0
	beq	l138
	bra	l141
l133
	move.l	a2,a5
	lea	(14+l144,a7),a0
	move.l	a0,-(a7)
	jsr	_pata_identity
	move.w	d0,d2
	addq.w	#4,a7
	beq	l135
l134
	move.w	d2,d0
	bra	l129
l135
	move.l	(80+l144,a7),(a5)
	bra	l132
l136
	move.l	a2,a4
	move.w	#512,(a4)
	bra	l132
l137
	move.l	a2,a3
	moveq	#1,d0
	move.l	d0,(a3)
	bra	l132
l138
	move.l	a2,a6
	move.l	a6,-(a7)
	jsr	_pata_identity
	move.w	d0,d2
	addq.w	#4,a7
	beq	l140
l139
	move.w	d2,d0
	bra	l129
l140
	bra	l132
l141
	moveq	#0,d0
	bra	l129
l132
	moveq	#0,d0
l129
l142	reg	a2/a3/a4/a5/a6/d2/d3
	movem.l	(a7)+,a2/a3/a4/a5/a6/d2/d3
l144	equ	28
	add.w	#88,a7
	rts
	cnop	0,4
l131
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	105
	dc.b	111
	dc.b	99
	dc.b	116
	dc.b	114
	dc.b	108
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_pata_install
	cnop	0,4
_pata_install
	sub.w	#32,a7
	movem.l	l149,-(a7)
	pea	l147
	jsr	_DEBUG
	move.w	#0,_g_pata_error
	move.w	#1,_g_pata_status
	move.w	#2,(4+l151,a7)
	move.l	#l148,(6+l151,a7)
	move.l	#_pata_init,(10+l151,a7)
	move.l	#_pata_read,(14+l151,a7)
	move.l	#_pata_write,(18+l151,a7)
	move.l	#_pata_status,(22+l151,a7)
	move.l	#_pata_flush,(26+l151,a7)
	move.l	#_pata_ioctrl,(30+l151,a7)
	move.w	#0,_g_pata_status
	lea	(4+l151,a7),a0
	move.l	a0,-(a7)
	jsr	_bdev_register
	addq.w	#8,a7
l145
l149	reg
l151	equ	0
	add.w	#32,a7
	rts
	cnop	0,4
l147
	dc.b	112
	dc.b	97
	dc.b	116
	dc.b	97
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	0
	cnop	0,4
l148
	dc.b	72
	dc.b	68
	dc.b	68
	dc.b	0
	public	_g_pata_error
	section	"DATA",data
	cnop	0,4
_g_pata_error
	dc.w	0
	public	_g_pata_status
	cnop	0,4
_g_pata_status
	dc.w	1
	public	_DEBUG
	public	_bdev_register
	public	_g_buffer
	section	"BSS",bss
	cnop	0,4
_g_buffer
	ds.b	512
