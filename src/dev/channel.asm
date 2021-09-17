	idnt	"channel.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_cdev_init_system
	cnop	0,4
_cdev_init_system
	movem.l	l11,-(a7)
	moveq	#0,d1
	bra	l4
l3
	moveq	#46,d0
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
	lea	_g_channel_devs,a0
	move.w	#0,(0,a0,d0.l)
	moveq	#46,d0
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
	lea	_g_channel_devs,a0
	move.l	#0,(2,a0,d0.l)
l6
	addq.l	#1,d1
l4
	moveq	#8,d0
	cmp.l	d1,d0
	bgt	l3
l5
	moveq	#0,d1
	bra	l8
l7
	moveq	#36,d0
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
	lea	_g_channels,a0
	move.w	#-1,(0,a0,d0.l)
	moveq	#36,d0
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
	lea	_g_channels,a0
	move.w	#-1,(2,a0,d0.l)
l10
	addq.l	#1,d1
l8
	moveq	#16,d0
	cmp.l	d1,d0
	bgt	l7
l9
	move.w	#0,_g_channels
	move.w	#0,2+_g_channels
	move.w	#1,36+_g_channels
	move.w	#1,38+_g_channels
l1
l11	reg	d2/d3
	movem.l	(a7)+,d2/d3
l13	equ	8
	rts
; stacksize=8
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_cdev_register
	cnop	0,4
_cdev_register
	movem.l	l19,-(a7)
	move.l	(4+l21,a7),a3
	move.w	(a3),d1
	cmp.w	#8,d1
	bge	l17
l16
	move.w	d1,d0
	ext.l	d0
	moveq	#46,d2
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
	lea	_g_channel_devs,a0
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
	move.l	(30,a3),(30,a2)
	move.l	(38,a3),(38,a2)
	move.l	(34,a3),(34,a2)
	move.l	(42,a3),(42,a2)
	moveq	#0,d0
	bra	l14
l17
	moveq	#-2,d0
l18
l14
l19	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l21	equ	20
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_alloc
	cnop	0,4
_chan_alloc
	movem.l	l30,-(a7)
	moveq	#0,d1
	bra	l25
l24
	moveq	#36,d0
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
	lea	_g_channels,a0
	tst.w	(0,a0,d0.l)
	bge	l29
l28
	moveq	#36,d0
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
	lea	_g_channels,a0
	move.w	d1,(0,a0,d0.l)
	moveq	#36,d0
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
	lea	_g_channels,a0
	add.l	d0,a0
	move.l	a0,d0
	bra	l22
l29
l27
	addq.l	#1,d1
l25
	moveq	#16,d0
	cmp.l	d1,d0
	bgt	l24
l26
	moveq	#0,d0
l22
l30	reg	d2/d3
	movem.l	(a7)+,d2/d3
l32	equ	8
	rts
; stacksize=8
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_get_record
	cnop	0,4
_chan_get_record
	movem.l	l35,-(a7)
	move.w	(6+l37,a7),d1
	move.w	d1,d0
	ext.l	d0
	moveq	#36,d2
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
	lea	_g_channels,a0
	add.l	d0,a0
	move.l	a0,d0
l33
l35	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l37	equ	12
	rts
; stacksize=12
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_free
	cnop	0,4
_chan_free
	movem.l	l40,-(a7)
	move.l	(4+l42,a7),a1
	move.w	#-1,(a1)
	move.w	#0,(2,a1)
l38
l40	reg
l42	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_cdev_init
	cnop	0,4
_cdev_init
	subq.w	#8,a7
	movem.l	l50,-(a7)
	move.w	(14+l52,a7),d2
	cmp.w	#8,d2
	bge	l46
l45
	move.w	d2,d0
	ext.l	d0
	moveq	#46,d1
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
	lea	_g_channel_devs,a0
	add.l	d0,a0
	move.l	a0,a2
	cmp.w	(a2),d2
	bne	l48
l47
	move.l	(6,a2),a1
	move.l	a1,(4+l52,a7)
	jsr	(a1)
	bra	l43
l48
	moveq	#-2,d0
l49
l46
l43
l50	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l52	equ	16
	addq.w	#8,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_get_records
	cnop	0,4
_chan_get_records
	movem.l	l64,-(a7)
	move.l	(12+l66,a7),a2
	move.w	(6+l66,a7),d1
	move.l	(8+l66,a7),a1
	cmp.w	#16,d1
	bge	l56
l55
	move.w	d1,d0
	ext.l	d0
	moveq	#36,d2
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
	lea	_g_channels,a0
	add.l	d0,a0
	move.l	a0,(a1)
	move.l	(a1),a0
	cmp.w	(a0),d1
	bne	l58
l57
	move.l	(a1),a0
	cmp.w	#8,(2,a0)
	bge	l60
l59
	move.l	(a1),a0
	move.w	(2,a0),d0
	ext.l	d0
	moveq	#46,d2
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
	lea	_g_channel_devs,a0
	add.l	d0,a0
	move.l	a0,(a2)
	moveq	#0,d0
	bra	l53
l60
	moveq	#-2,d0
	bra	l53
l61
	bra	l62
l58
	moveq	#-2,d0
	bra	l53
l62
	bra	l63
l56
	moveq	#-10,d0
l63
l53
l64	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l66	equ	16
	rts
; stacksize=16
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_read
	cnop	0,4
_chan_read
	sub.w	#16,a7
	movem.l	l72,-(a7)
	move.w	(30+l74,a7),d4
	move.w	(22+l74,a7),d3
	move.l	(24+l74,a7),a2
	lea	(4+l74,a7),a0
	move.l	a0,-(a7)
	lea	(4+l74,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l70
l69
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a2,-(a7)
	move.l	(8+l74,a7),-(a7)
	move.l	(16+l74,a7),a0
	move.l	(10,a0),a1
	move.l	a1,(22+l74,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l67
l70
	move.w	d2,d0
l71
l67
l72	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l74	equ	16
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_readline
	cnop	0,4
_chan_readline
	sub.w	#16,a7
	movem.l	l80,-(a7)
	move.w	(30+l82,a7),d4
	move.w	(22+l82,a7),d3
	move.l	(24+l82,a7),a2
	lea	(4+l82,a7),a0
	move.l	a0,-(a7)
	lea	(4+l82,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l78
l77
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a2,-(a7)
	move.l	(8+l82,a7),-(a7)
	move.l	(16+l82,a7),a0
	move.l	(14,a0),a1
	move.l	a1,(22+l82,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l75
l78
	move.w	d2,d0
l79
l75
l80	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l82	equ	16
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_read_b
	cnop	0,4
_chan_read_b
	sub.w	#16,a7
	movem.l	l88,-(a7)
	move.w	(22+l90,a7),d3
	lea	(4+l90,a7),a0
	move.l	a0,-(a7)
	lea	(4+l90,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l86
l85
	move.l	(0+l90,a7),-(a7)
	move.l	(8+l90,a7),a0
	move.l	(18,a0),a1
	move.l	a1,(14+l90,a7)
	jsr	(a1)
	addq.w	#4,a7
	bra	l83
l86
	move.w	d2,d0
l87
l83
l88	reg	d2/d3
	movem.l	(a7)+,d2/d3
l90	equ	8
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_write
	cnop	0,4
_chan_write
	sub.w	#16,a7
	movem.l	l97,-(a7)
	move.w	(30+l99,a7),d4
	move.w	(22+l99,a7),d3
	move.l	(24+l99,a7),a2
	lea	(4+l99,a7),a0
	move.l	a0,-(a7)
	lea	(4+l99,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l94
l93
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a2,-(a7)
	move.l	(8+l99,a7),-(a7)
	move.l	(16+l99,a7),a0
	move.l	(22,a0),a1
	move.l	a1,(22+l99,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l91
l94
	pea	l96
	jsr	_DEBUG
	move.w	d2,d0
	addq.w	#4,a7
l95
l91
l97	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l99	equ	16
	add.w	#16,a7
	rts
	cnop	0,4
l96
	dc.b	99
	dc.b	104
	dc.b	97
	dc.b	110
	dc.b	95
	dc.b	119
	dc.b	114
	dc.b	105
	dc.b	116
	dc.b	101
	dc.b	32
	dc.b	101
	dc.b	114
	dc.b	114
	dc.b	111
	dc.b	114
	dc.b	10
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_write_b
	cnop	0,4
_chan_write_b
	sub.w	#16,a7
	movem.l	l105,-(a7)
	move.b	(27+l107,a7),d4
	move.w	(22+l107,a7),d3
	lea	(4+l107,a7),a0
	move.l	a0,-(a7)
	lea	(4+l107,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l103
l102
	moveq	#0,d0
	move.b	d4,d0
	move.l	d0,-(a7)
	move.l	(4+l107,a7),-(a7)
	move.l	(12+l107,a7),a0
	move.l	(26,a0),a1
	move.l	a1,(18+l107,a7)
	jsr	(a1)
	addq.w	#8,a7
	bra	l100
l103
	move.w	d2,d0
l104
l100
l105	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l107	equ	12
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_status
	cnop	0,4
_chan_status
	sub.w	#16,a7
	movem.l	l113,-(a7)
	move.w	(22+l115,a7),d3
	lea	(4+l115,a7),a0
	move.l	a0,-(a7)
	lea	(4+l115,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l111
l110
	move.l	(0+l115,a7),-(a7)
	move.l	(8+l115,a7),a0
	move.l	(30,a0),a1
	move.l	a1,(14+l115,a7)
	jsr	(a1)
	addq.w	#4,a7
	bra	l108
l111
	move.w	d2,d0
l112
l108
l113	reg	d2/d3
	movem.l	(a7)+,d2/d3
l115	equ	8
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_flush
	cnop	0,4
_chan_flush
	sub.w	#16,a7
	movem.l	l121,-(a7)
	move.w	(22+l123,a7),d3
	lea	(4+l123,a7),a0
	move.l	a0,-(a7)
	lea	(4+l123,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l119
l118
	move.l	(0+l123,a7),-(a7)
	move.l	(8+l123,a7),a0
	move.l	(34,a0),a1
	move.l	a1,(14+l123,a7)
	jsr	(a1)
	addq.w	#4,a7
	bra	l116
l119
	move.w	d2,d0
l120
l116
l121	reg	d2/d3
	movem.l	(a7)+,d2/d3
l123	equ	8
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_seek
	cnop	0,4
_chan_seek
	sub.w	#16,a7
	movem.l	l129,-(a7)
	move.w	(30+l131,a7),d5
	move.l	(24+l131,a7),d4
	move.w	(22+l131,a7),d3
	lea	(4+l131,a7),a0
	move.l	a0,-(a7)
	lea	(4+l131,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l127
l126
	move.w	d5,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	d4,-(a7)
	move.l	(8+l131,a7),-(a7)
	move.l	(16+l131,a7),a0
	move.l	(38,a0),a1
	move.l	a1,(22+l131,a7)
	jsr	(a1)
	add.w	#12,a7
	bra	l124
l127
	move.w	d2,d0
l128
l124
l129	reg	d2/d3/d4/d5
	movem.l	(a7)+,d2/d3/d4/d5
l131	equ	16
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_chan_ioctrl
	cnop	0,4
_chan_ioctrl
	sub.w	#16,a7
	movem.l	l137,-(a7)
	move.w	(34+l139,a7),d5
	move.w	(26+l139,a7),d4
	move.w	(22+l139,a7),d3
	move.l	(28+l139,a7),a2
	lea	(4+l139,a7),a0
	move.l	a0,-(a7)
	lea	(4+l139,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_records
	move.w	d0,d2
	add.w	#12,a7
	bne	l135
l134
	move.w	d5,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a2,-(a7)
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	(12+l139,a7),-(a7)
	move.l	(20+l139,a7),a0
	move.l	(42,a0),a1
	move.l	a1,(26+l139,a7)
	jsr	(a1)
	add.w	#16,a7
	bra	l132
l135
	move.w	d2,d0
l136
l132
l137	reg	a2/d2/d3/d4/d5
	movem.l	(a7)+,a2/d2/d3/d4/d5
l139	equ	20
	add.w	#16,a7
	rts
	public	_DEBUG
	public	_g_channel_devs
	section	"BSS",bss
	cnop	0,4
_g_channel_devs
	ds.b	368
	public	_g_channels
	cnop	0,4
_g_channels
	ds.b	576
