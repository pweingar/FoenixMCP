	idnt	"sdc.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_sdc_reset
	cnop	0,4
_sdc_reset
	movem.l	l4,-(a7)
	pea	l3
	jsr	_DEBUG
	addq.w	#4,a7
l1
l4	reg
l6	equ	0
	rts
	cnop	0,4
l3
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	115
	dc.b	101
	dc.b	116
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_detected
	cnop	0,4
_sdc_detected
	movem.l	l9,-(a7)
	moveq	#1,d0
l7
l9	reg
l11	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_protected
	cnop	0,4
_sdc_protected
	movem.l	l14,-(a7)
	moveq	#0,d0
l12
l14	reg
l16	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_set_led
	cnop	0,4
_sdc_set_led
	movem.l	l19,-(a7)
l17
l19	reg
l21	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_wait_busy
	cnop	0,4
_sdc_wait_busy
	movem.l	l29,-(a7)
	move.w	#10000,d1
l24
	move.w	d1,d0
	subq.w	#1,d1
	tst.w	d0
	bne	l28
l27
	moveq	#-3,d0
	bra	l22
l28
	move.b	12583684,d2
l26
	moveq	#0,d0
	move.b	d2,d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	moveq	#1,d3
	cmp.l	d0,d3
	beq	l24
l25
	moveq	#0,d0
l22
l29	reg	d2/d3
	movem.l	(a7)+,d2/d3
l31	equ	8
	rts
; stacksize=8
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_init
	cnop	0,4
_sdc_init
	movem.l	l46,-(a7)
	pea	l34
	jsr	_DEBUG
	jsr	_sdc_detected
	addq.w	#4,a7
	tst.w	d0
	bne	l36
l35
	move.b	#1,_g_sdc_status
	moveq	#-8,d0
	bra	l32
l36
	move.b	#1,12583682
	move.b	#1,12583683
	jsr	_sdc_wait_busy
	tst.w	d0
	bne	l38
l37
	move.b	12583685,_g_sdc_error
	bne	l40
l39
	pea	l41
	jsr	_DEBUG
	move.b	#0,_g_sdc_status
	moveq	#0,d0
	addq.w	#4,a7
	bra	l32
l40
	pea	l43
	jsr	_DEBUG
	move.b	#1,_g_sdc_status
	moveq	#-4,d0
	addq.w	#4,a7
	bra	l32
l42
	bra	l44
l38
	pea	l45
	jsr	_DEBUG
	move.b	#1,_g_sdc_status
	moveq	#-3,d0
	addq.w	#4,a7
l44
l32
l46	reg
l48	equ	0
	rts
	cnop	0,4
l41
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	58
	dc.b	32
	dc.b	83
	dc.b	85
	dc.b	67
	dc.b	67
	dc.b	69
	dc.b	83
	dc.b	83
	dc.b	0
	cnop	0,4
l43
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	58
	dc.b	32
	dc.b	68
	dc.b	69
	dc.b	86
	dc.b	95
	dc.b	67
	dc.b	65
	dc.b	78
	dc.b	78
	dc.b	79
	dc.b	84
	dc.b	95
	dc.b	73
	dc.b	78
	dc.b	73
	dc.b	84
	dc.b	0
	cnop	0,4
l45
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	58
	dc.b	32
	dc.b	68
	dc.b	69
	dc.b	86
	dc.b	95
	dc.b	84
	dc.b	73
	dc.b	77
	dc.b	69
	dc.b	79
	dc.b	85
	dc.b	84
	dc.b	0
	cnop	0,4
l34
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_read
	cnop	0,4
_sdc_read
	movem.l	l69,-(a7)
	move.w	(14+l71,a7),d6
	move.l	(4+l71,a7),d5
	move.l	(8+l71,a7),a2
	pea	l51
	jsr	_DEBUG
	jsr	_sdc_detected
	addq.w	#4,a7
	tst.w	d0
	bne	l53
l52
	move.b	#1,_g_sdc_status
	moveq	#-8,d0
	bra	l49
l53
	move.l	#1,-(a7)
	jsr	_sdc_set_led
	moveq	#9,d0
	move.l	d5,d4
	lsl.l	d0,d4
	move.l	#255,d0
	and.l	d4,d0
	move.b	d0,12583687
	move.l	d4,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.b	d0,12583688
	moveq	#16,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#255,d0
	move.b	d0,12583689
	moveq	#24,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#255,d0
	move.b	d0,12583690
	move.b	#2,12583682
	move.b	#1,12583683
	jsr	_sdc_wait_busy
	addq.w	#4,a7
	tst.w	d0
	bne	l55
l54
	move.b	12583685,_g_sdc_error
	beq	l57
l56
	move.l	#0,-(a7)
	jsr	_sdc_set_led
	moveq	#-5,d0
	addq.w	#4,a7
	bra	l49
l57
	moveq	#0,d0
	move.b	12583698,d0
	lsl.l	#8,d0
	moveq	#0,d1
	move.b	12583699,d1
	or.l	d1,d0
	move.w	d0,d3
	cmp.w	d3,d6
	bge	l60
l59
	moveq	#-7,d0
	bra	l49
l60
	moveq	#0,d2
	bra	l62
l61
	move.b	12583696,(0,a2,d2.w)
l64
	addq.w	#1,d2
l62
	cmp.w	d2,d3
	bgt	l61
l63
	move.l	#0,-(a7)
	jsr	_sdc_set_led
	move.b	12583685,_g_sdc_error
	addq.w	#4,a7
	beq	l66
l65
	moveq	#-5,d0
	bra	l49
l66
	move.w	d3,d0
	bra	l49
l67
l58
	bra	l68
l55
	move.l	#0,-(a7)
	jsr	_sdc_set_led
	moveq	#-3,d0
	addq.w	#4,a7
l68
l49
l69	reg	a2/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/d2/d3/d4/d5/d6
l71	equ	24
	rts
	cnop	0,4
l51
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_write
	cnop	0,4
_sdc_write
	movem.l	l96,-(a7)
	move.l	(4+l98,a7),d5
	move.w	(14+l98,a7),d3
	move.l	(8+l98,a7),a2
	pea	l74
	jsr	_DEBUG
	jsr	_sdc_detected
	addq.w	#4,a7
	tst.w	d0
	bne	l76
l75
	move.b	#1,_g_sdc_status
	moveq	#-8,d0
	bra	l72
l76
	move.l	#1,-(a7)
	jsr	_sdc_set_led
	addq.w	#4,a7
	cmp.w	#512,d3
	bgt	l78
l77
	moveq	#0,d2
	bra	l80
l79
	move.b	(0,a2,d2.w),12583712
l82
	addq.w	#1,d2
l80
	cmp.w	d2,d3
	bgt	l79
l81
	cmp.w	#512,d3
	bge	l84
l83
	moveq	#0,d2
	bra	l86
l85
	move.b	#0,12583712
l88
	addq.w	#1,d2
l86
	move.w	d2,d0
	ext.l	d0
	move.w	d3,d1
	ext.l	d1
	neg.l	d1
	add.l	#512,d1
	cmp.l	d0,d1
	bgt	l85
l87
l84
	bra	l89
l78
	moveq	#-7,d0
	bra	l72
l89
	moveq	#9,d0
	move.l	d5,d4
	lsl.l	d0,d4
	move.l	#255,d0
	and.l	d4,d0
	move.b	d0,12583687
	move.l	d4,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.b	d0,12583688
	moveq	#16,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#255,d0
	move.b	d0,12583689
	moveq	#24,d1
	move.l	d4,d0
	asr.l	d1,d0
	and.l	#255,d0
	move.b	d0,12583690
	move.b	#3,12583682
	move.b	#1,12583683
	jsr	_sdc_wait_busy
	tst.w	d0
	bne	l91
l90
	move.b	12583685,_g_sdc_error
	beq	l93
l92
	move.l	#0,-(a7)
	jsr	_sdc_set_led
	moveq	#-6,d0
	addq.w	#4,a7
	bra	l72
l93
	move.w	d3,d0
	bra	l72
l94
	bra	l95
l91
	move.l	#0,-(a7)
	jsr	_sdc_set_led
	moveq	#-3,d0
	addq.w	#4,a7
l95
l72
l96	reg	a2/d2/d3/d4/d5
	movem.l	(a7)+,a2/d2/d3/d4/d5
l98	equ	20
	rts
	cnop	0,4
l74
	dc.b	115
	dc.b	100
	dc.b	99
	dc.b	95
	dc.b	119
	dc.b	114
	dc.b	105
	dc.b	116
	dc.b	101
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_status
	cnop	0,4
_sdc_status
	movem.l	l105,-(a7)
	moveq	#0,d2
	move.b	_g_sdc_status,d2
	jsr	_sdc_detected
	tst.w	d0
	beq	l102
	or.w	#2,d2
l102
	jsr	_sdc_protected
	tst.w	d0
	beq	l104
	or.w	#4,d2
l104
	move.w	d2,d0
l99
l105	reg	d2
	movem.l	(a7)+,d2
l107	equ	4
	rts
; stacksize=8
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_error
	cnop	0,4
_sdc_error
	movem.l	l110,-(a7)
	moveq	#0,d0
	move.b	_g_sdc_error,d0
l108
l110	reg
l112	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_flush
	cnop	0,4
_sdc_flush
	movem.l	l115,-(a7)
	moveq	#0,d0
l113
l115	reg
l117	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_sector_count
	cnop	0,4
_sdc_sector_count
	movem.l	l120,-(a7)
	move.w	#1000,d0
l118
l120	reg
l122	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_ioctrl
	cnop	0,4
_sdc_ioctrl
	movem.l	l130,-(a7)
	move.w	(6+l132,a7),d2
	move.l	(8+l132,a7),a2
	move.w	d2,d0
	subq.w	#1,d0
	beq	l126
	subq.w	#1,d0
	beq	l127
	subq.w	#1,d0
	beq	l128
	bra	l129
l126
	move.l	a2,a5
	jsr	_sdc_sector_count
	move.w	d0,d1
	ext.l	d1
	move.l	d1,(a5)
	bra	l125
l127
	move.l	a2,a4
	move.w	#512,(a4)
	bra	l125
l128
	move.l	a2,a3
	moveq	#1,d0
	move.l	d0,(a3)
	bra	l125
l129
	moveq	#0,d0
l125
l123
l130	reg	a2/a3/a4/a5/d2
	movem.l	(a7)+,a2/a3/a4/a5/d2
l132	equ	20
	rts
; stacksize=24
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sdc_install
	cnop	0,4
_sdc_install
	sub.w	#32,a7
	movem.l	l137,-(a7)
	pea	l135
	jsr	_DEBUG
	jsr	_sdc_reset
	move.w	#0,(4+l139,a7)
	move.l	#l136,(6+l139,a7)
	move.l	#_sdc_init,(10+l139,a7)
	move.l	#_sdc_read,(14+l139,a7)
	move.l	#_sdc_write,(18+l139,a7)
	move.l	#_sdc_flush,(26+l139,a7)
	move.l	#_sdc_status,(22+l139,a7)
	move.l	#_sdc_ioctrl,(30+l139,a7)
	lea	(4+l139,a7),a0
	move.l	a0,-(a7)
	jsr	_bdev_register
	addq.w	#8,a7
l133
l137	reg
l139	equ	0
	add.w	#32,a7
	rts
	cnop	0,4
l135
	dc.b	115
	dc.b	100
	dc.b	99
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
l136
	dc.b	83
	dc.b	68
	dc.b	67
	dc.b	0
	public	_g_sdc_status
	section	"DATA",data
	cnop	0,4
_g_sdc_status
	dc.b	1
	public	_g_sdc_error
	cnop	0,4
_g_sdc_error
	dc.b	0
	public	_DEBUG
	public	_bdev_register
