	idnt	"foenixmcp.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_initialize
	cnop	0,4
_initialize
	movem.l	l36,-(a7)
	jsr	_text_init
	pea	l3
	jsr	_DEBUG
	jsr	_cdev_init_system
	pea	l4
	jsr	_DEBUG
	jsr	_bdev_init_system
	pea	l5
	jsr	_DEBUG
	jsr	_con_install
	add.w	#12,a7
	tst.w	d0
	beq	l7
	pea	l8
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l9
l7
	pea	l10
	jsr	_DEBUG
	addq.w	#4,a7
l9
	jsr	_pata_install
	tst.w	d0
	beq	l12
	pea	l13
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l14
l12
	pea	l15
	jsr	_DEBUG
	addq.w	#4,a7
l14
	jsr	_sdc_install
	tst.w	d0
	beq	l17
	pea	l18
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l19
l17
	pea	l20
	jsr	_DEBUG
	addq.w	#4,a7
l19
	jsr	_ps2_init
	tst.w	d0
	beq	l22
	pea	l23
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l24
l22
	pea	l25
	jsr	_DEBUG
	addq.w	#4,a7
l24
	move.l	#2,-(a7)
	jsr	_bdev_init
	addq.w	#4,a7
	tst.w	d0
	beq	l27
	pea	l28
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l29
l27
	pea	l30
	jsr	_DEBUG
	addq.w	#4,a7
l29
	move.l	#0,-(a7)
	jsr	_bdev_init
	addq.w	#4,a7
	tst.w	d0
	beq	l32
	pea	l33
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l34
l32
	pea	l35
	jsr	_DEBUG
	addq.w	#4,a7
l34
l1
l36	reg
l38	equ	0
	rts
	cnop	0,4
l8
	dc.b	70
	dc.b	65
	dc.b	73
	dc.b	76
	dc.b	69
	dc.b	68
	dc.b	58
	dc.b	32
	dc.b	67
	dc.b	111
	dc.b	110
	dc.b	115
	dc.b	111
	dc.b	108
	dc.b	101
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	97
	dc.b	116
	dc.b	105
	dc.b	111
	dc.b	110
	dc.b	46
	dc.b	0
	cnop	0,4
l10
	dc.b	67
	dc.b	111
	dc.b	110
	dc.b	115
	dc.b	111
	dc.b	108
	dc.b	101
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	101
	dc.b	100
	dc.b	46
	dc.b	0
	cnop	0,4
l13
	dc.b	70
	dc.b	65
	dc.b	73
	dc.b	76
	dc.b	69
	dc.b	68
	dc.b	58
	dc.b	32
	dc.b	80
	dc.b	65
	dc.b	84
	dc.b	65
	dc.b	32
	dc.b	100
	dc.b	114
	dc.b	105
	dc.b	118
	dc.b	101
	dc.b	114
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	97
	dc.b	116
	dc.b	105
	dc.b	111
	dc.b	110
	dc.b	46
	dc.b	0
	cnop	0,4
l15
	dc.b	80
	dc.b	65
	dc.b	84
	dc.b	65
	dc.b	32
	dc.b	100
	dc.b	114
	dc.b	105
	dc.b	118
	dc.b	101
	dc.b	114
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	101
	dc.b	100
	dc.b	46
	dc.b	0
	cnop	0,4
l18
	dc.b	70
	dc.b	65
	dc.b	73
	dc.b	76
	dc.b	69
	dc.b	68
	dc.b	58
	dc.b	32
	dc.b	83
	dc.b	68
	dc.b	67
	dc.b	32
	dc.b	100
	dc.b	114
	dc.b	105
	dc.b	118
	dc.b	101
	dc.b	114
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	97
	dc.b	116
	dc.b	105
	dc.b	111
	dc.b	110
	dc.b	46
	dc.b	0
	cnop	0,4
l20
	dc.b	83
	dc.b	68
	dc.b	67
	dc.b	32
	dc.b	100
	dc.b	114
	dc.b	105
	dc.b	118
	dc.b	101
	dc.b	114
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	108
	dc.b	108
	dc.b	101
	dc.b	100
	dc.b	46
	dc.b	0
	cnop	0,4
l23
	dc.b	70
	dc.b	65
	dc.b	73
	dc.b	76
	dc.b	69
	dc.b	68
	dc.b	58
	dc.b	32
	dc.b	80
	dc.b	83
	dc.b	47
	dc.b	50
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	105
	dc.b	97
	dc.b	108
	dc.b	105
	dc.b	122
	dc.b	97
	dc.b	116
	dc.b	105
	dc.b	111
	dc.b	110
	dc.b	46
	dc.b	0
	cnop	0,4
l25
	dc.b	80
	dc.b	83
	dc.b	47
	dc.b	50
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	105
	dc.b	97
	dc.b	108
	dc.b	105
	dc.b	122
	dc.b	101
	dc.b	100
	dc.b	46
	dc.b	0
	cnop	0,4
l28
	dc.b	85
	dc.b	110
	dc.b	97
	dc.b	98
	dc.b	108
	dc.b	101
	dc.b	32
	dc.b	116
	dc.b	111
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	105
	dc.b	97
	dc.b	108
	dc.b	105
	dc.b	122
	dc.b	101
	dc.b	32
	dc.b	116
	dc.b	104
	dc.b	101
	dc.b	32
	dc.b	80
	dc.b	65
	dc.b	84
	dc.b	65
	dc.b	33
	dc.b	0
	cnop	0,4
l30
	dc.b	80
	dc.b	65
	dc.b	84
	dc.b	65
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	105
	dc.b	97
	dc.b	108
	dc.b	105
	dc.b	122
	dc.b	101
	dc.b	100
	dc.b	46
	dc.b	0
	cnop	0,4
l33
	dc.b	85
	dc.b	110
	dc.b	97
	dc.b	98
	dc.b	108
	dc.b	101
	dc.b	32
	dc.b	116
	dc.b	111
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	105
	dc.b	97
	dc.b	108
	dc.b	105
	dc.b	122
	dc.b	101
	dc.b	32
	dc.b	116
	dc.b	104
	dc.b	101
	dc.b	32
	dc.b	83
	dc.b	68
	dc.b	67
	dc.b	33
	dc.b	0
	cnop	0,4
l35
	dc.b	83
	dc.b	68
	dc.b	67
	dc.b	32
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	105
	dc.b	97
	dc.b	108
	dc.b	105
	dc.b	122
	dc.b	101
	dc.b	100
	dc.b	46
	dc.b	0
	cnop	0,4
l3
	dc.b	70
	dc.b	111
	dc.b	101
	dc.b	110
	dc.b	105
	dc.b	120
	dc.b	47
	dc.b	77
	dc.b	67
	dc.b	80
	dc.b	32
	dc.b	115
	dc.b	116
	dc.b	97
	dc.b	114
	dc.b	116
	dc.b	105
	dc.b	110
	dc.b	103
	dc.b	32
	dc.b	117
	dc.b	112
	dc.b	46
	dc.b	46
	dc.b	46
	dc.b	0
	cnop	0,4
l4
	dc.b	67
	dc.b	104
	dc.b	97
	dc.b	110
	dc.b	110
	dc.b	101
	dc.b	108
	dc.b	32
	dc.b	100
	dc.b	101
	dc.b	118
	dc.b	105
	dc.b	99
	dc.b	101
	dc.b	32
	dc.b	115
	dc.b	121
	dc.b	115
	dc.b	116
	dc.b	101
	dc.b	109
	dc.b	32
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	121
	dc.b	46
	dc.b	0
	cnop	0,4
l5
	dc.b	66
	dc.b	108
	dc.b	111
	dc.b	99
	dc.b	107
	dc.b	32
	dc.b	100
	dc.b	101
	dc.b	118
	dc.b	105
	dc.b	99
	dc.b	101
	dc.b	32
	dc.b	115
	dc.b	121
	dc.b	115
	dc.b	116
	dc.b	101
	dc.b	109
	dc.b	32
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	121
	dc.b	46
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_print
	cnop	0,4
_print
	movem.l	l45,-(a7)
	move.w	(6+l47,a7),d2
	move.l	(8+l47,a7),a3
	move.l	a3,a2
	bra	l42
l41
	move.b	(a2),d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_text_put_raw
	addq.w	#8,a7
l44
	addq.l	#1,a2
l42
	tst.b	(a2)
	bne	l41
l43
l39
l45	reg	a2/a3/d2
	movem.l	(a7)+,a2/a3/d2
l47	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_print_hex
	cnop	0,4
_print_hex
	movem.l	l50,-(a7)
	move.w	(6+l52,a7),d4
	move.w	(10+l52,a7),d3
	move.b	d3,d0
	and.b	#240,d0
	and.w	#255,d0
	and.l	#65535,d0
	asr.l	#4,d0
	move.w	d0,d2
	lea	_hex_digits,a0
	move.b	(0,a0,d2.w),_number
	move.b	d3,d0
	and.b	#15,d0
	moveq	#0,d2
	move.b	d0,d2
	lea	_hex_digits,a0
	move.b	(0,a0,d2.w),1+_number
	move.b	#0,2+_number
	pea	_number
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
l48
l50	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l52	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_print_hex_16
	cnop	0,4
_print_hex_16
	movem.l	l55,-(a7)
	move.w	(6+l57,a7),d4
	move.w	(10+l57,a7),d3
	moveq	#0,d0
	move.w	d3,d0
	moveq	#12,d1
	asr.l	d1,d0
	and.b	#15,d0
	and.l	#255,d0
	move.w	d0,d2
	lea	_hex_digits,a0
	move.b	(0,a0,d2.w),_number
	moveq	#0,d0
	move.w	d3,d0
	asr.l	#8,d0
	and.b	#15,d0
	and.l	#255,d0
	move.w	d0,d2
	lea	_hex_digits,a0
	move.b	(0,a0,d2.w),1+_number
	moveq	#0,d0
	move.w	d3,d0
	asr.l	#4,d0
	and.b	#15,d0
	and.l	#255,d0
	move.w	d0,d2
	lea	_hex_digits,a0
	move.b	(0,a0,d2.w),2+_number
	move.b	d3,d0
	and.b	#15,d0
	moveq	#0,d2
	move.b	d0,d2
	lea	_hex_digits,a0
	move.b	(0,a0,d2.w),3+_number
	move.b	#0,4+_number
	pea	_number
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
l53
l55	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l57	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_try_mo
	cnop	0,4
_try_mo
	movem.l	l69,-(a7)
	move.w	(6+l71,a7),d2
	move.l	#12582976,a2
	move.l	#12582978,a3
	pea	l60
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
l61
	move.w	(a3),d3
	move.w	#32768,d0
	and.w	d3,d0
	and.l	#65535,d0
	cmp.l	#32768,d0
	beq	l65
l64
	move.w	(a2),d4
	pea	l66
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	moveq	#0,d0
	move.w	d3,d0
	move.l	d0,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print_hex_16
	pea	l67
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	moveq	#0,d0
	move.w	d4,d0
	move.l	d0,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print_hex_16
	pea	l68
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	add.w	#40,a7
l65
l63
	bra	l61
l62
l58
l69	reg	a2/a3/d2/d3/d4
	movem.l	(a7)+,a2/a3/d2/d3/d4
l71	equ	20
	rts
	cnop	0,4
l66
	dc.b	91
	dc.b	0
	cnop	0,4
l67
	dc.b	93
	dc.b	58
	dc.b	32
	dc.b	123
	dc.b	0
	cnop	0,4
l68
	dc.b	125
	dc.b	10
	dc.b	32
	dc.b	0
	cnop	0,4
l60
	dc.b	109
	dc.b	111
	dc.b	62
	dc.b	32
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_dos_cmd_dir
	cnop	0,4
_dos_cmd_dir
	movem.l	l100,-(a7)
	move.w	(6+l102,a7),d2
	pea	l74
	jsr	_DEBUG
	move.l	#0,-(a7)
	pea	l75
	pea	_my_fs
	jsr	_f_mount
	move.l	d0,d3
	pea	l76
	jsr	_DEBUG
	add.w	#20,a7
	tst.l	d3
	bne	l78
l77
	pea	l79
	pea	_my_dir
	jsr	_f_opendir
	move.l	d0,d3
	pea	l80
	jsr	_DEBUG
	add.w	#12,a7
	tst.l	d3
	bne	l82
l81
l83
	pea	_my_file
	pea	_my_dir
	jsr	_f_readdir
	move.l	d0,d3
	pea	l86
	jsr	_DEBUG
	add.w	#12,a7
	tst.l	d3
	bne	l88
l89
	tst.b	22+_my_file
	beq	l88
l87
	moveq	#0,d0
	move.b	8+_my_file,d0
	and.b	#2,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l91
l90
	lea	22+_my_file,a0
	inline
	move.l	a0,d0
.l1
	tst.b	(a0)+
	bne	.l1
	sub.l	a0,d0
	not.l	d0
	einline
	move.l	d0,-(a7)
	pea	22+_my_file
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_write
	moveq	#0,d0
	move.b	8+_my_file,d0
	and.b	#16,d0
	and.w	#255,d0
	and.l	#65535,d0
	add.w	#12,a7
	tst.l	d0
	beq	l93
	move.l	#47,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_write_b
	addq.w	#8,a7
l93
	move.l	#10,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_write_b
	addq.w	#8,a7
l91
	bra	l94
l88
	bra	l84
l94
l85
	bra	l83
l84
	pea	_my_dir
	jsr	_f_closedir
	addq.w	#4,a7
	bra	l95
l82
	lea	l96,a2
	move.l	a2,a0
	inline
	move.l	a0,d0
.l1
	tst.b	(a0)+
	bne	.l1
	sub.l	a0,d0
	not.l	d0
	einline
	move.l	d0,-(a7)
	move.l	a2,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_write
	add.w	#12,a7
l95
	move.l	#0,-(a7)
	pea	l97
	move.l	#0,-(a7)
	jsr	_f_mount
	add.w	#12,a7
	bra	l98
l78
	lea	l99,a3
	move.l	a3,a0
	inline
	move.l	a0,d0
.l1
	tst.b	(a0)+
	bne	.l1
	sub.l	a0,d0
	not.l	d0
	einline
	move.l	d0,-(a7)
	move.l	a3,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_write
	add.w	#12,a7
l98
	moveq	#0,d0
l72
l100	reg	a2/a3/d2/d3
	movem.l	(a7)+,a2/a3/d2/d3
l102	equ	16
	rts
	cnop	0,4
l86
	dc.b	102
	dc.b	95
	dc.b	114
	dc.b	101
	dc.b	97
	dc.b	100
	dc.b	100
	dc.b	105
	dc.b	114
	dc.b	0
	cnop	0,4
l96
	dc.b	67
	dc.b	111
	dc.b	117
	dc.b	108
	dc.b	100
	dc.b	32
	dc.b	110
	dc.b	111
	dc.b	116
	dc.b	32
	dc.b	111
	dc.b	112
	dc.b	101
	dc.b	110
	dc.b	32
	dc.b	100
	dc.b	105
	dc.b	114
	dc.b	101
	dc.b	99
	dc.b	116
	dc.b	111
	dc.b	114
	dc.b	121
	dc.b	46
	dc.b	13
	dc.b	0
	cnop	0,4
l79
	dc.b	47
	dc.b	0
	cnop	0,4
l80
	dc.b	102
	dc.b	95
	dc.b	111
	dc.b	112
	dc.b	101
	dc.b	110
	dc.b	100
	dc.b	105
	dc.b	114
	dc.b	0
	cnop	0,4
l97
	dc.b	0
	cnop	0,4
l99
	dc.b	67
	dc.b	111
	dc.b	117
	dc.b	108
	dc.b	100
	dc.b	32
	dc.b	110
	dc.b	111
	dc.b	116
	dc.b	32
	dc.b	109
	dc.b	111
	dc.b	117
	dc.b	110
	dc.b	116
	dc.b	32
	dc.b	100
	dc.b	114
	dc.b	105
	dc.b	118
	dc.b	101
	dc.b	46
	dc.b	13
	dc.b	0
	cnop	0,4
l74
	dc.b	100
	dc.b	111
	dc.b	115
	dc.b	95
	dc.b	99
	dc.b	109
	dc.b	100
	dc.b	95
	dc.b	100
	dc.b	105
	dc.b	114
	dc.b	0
	cnop	0,4
l75
	dc.b	48
	dc.b	58
	dc.b	0
	cnop	0,4
l76
	dc.b	102
	dc.b	95
	dc.b	109
	dc.b	111
	dc.b	117
	dc.b	110
	dc.b	116
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_repl
	cnop	0,4
_repl
	subq.w	#4,a7
	movem.l	l111,-(a7)
	move.w	(10+l113,a7),d3
	pea	l105
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
l106
	jsr	_kbd_getc_poll
	move.b	d0,d2
	beq	l110
	move.b	d2,(1+l113,a7)
	move.b	#0,(2+l113,a7)
	lea	(1+l113,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
l110
	bra	l106
l108
l103
l111	reg	d2/d3
	movem.l	(a7)+,d2/d3
l113	equ	8
	addq.w	#4,a7
	rts
	cnop	0,4
l105
	dc.b	62
	dc.b	32
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_test_get_mbr
	cnop	0,4
_test_get_mbr
	sub.w	#516,a7
	movem.l	l138,-(a7)
	move.w	(526+l140,a7),d5
	move.w	(522+l140,a7),d3
	moveq	#0,d2
	bra	l117
l116
	lea	(4+l140,a7),a0
	move.b	#0,(0,a0,d2.w)
l119
	addq.w	#1,d2
l117
	cmp.w	#512,d2
	blt	l116
l118
	pea	l120
	move.l	#1,-(a7)
	jsr	_print
	move.l	#512,-(a7)
	lea	(16+l140,a7),a0
	move.l	a0,-(a7)
	move.l	#0,-(a7)
	move.w	d5,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_bdev_read
	move.w	d0,d4
	add.w	#24,a7
	ble	l122
l121
	moveq	#0,d2
	bra	l124
l123
	move.w	d2,d0
	ext.l	d0
	move.l	#16,-(a7)
	move.l	d0,-(a7)
	public	__lmods
	jsr	__lmods
	addq.w	#8,a7
	tst.l	d0
	bne	l128
l127
	pea	l129
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
l128
	lea	(4+l140,a7),a0
	add.w	d2,a0
	move.b	(a0),d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print_hex
	pea	l130
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	add.w	#16,a7
l126
	addq.w	#1,d2
l124
	cmp.w	d2,d4
	bgt	l123
l125
	pea	l131
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_print
	addq.w	#8,a7
	bra	l132
l122
	tst.w	d4
	bge	l134
l133
	pea	l135
	jsr	_DEBUG
	addq.w	#4,a7
	bra	l136
l134
	pea	l137
	jsr	_DEBUG
	addq.w	#4,a7
l136
l132
l114
l138	reg	d2/d3/d4/d5
	movem.l	(a7)+,d2/d3/d4/d5
l140	equ	16
	add.w	#516,a7
	rts
	cnop	0,4
l129
	dc.b	10
	dc.b	0
	cnop	0,4
l130
	dc.b	32
	dc.b	0
	cnop	0,4
l131
	dc.b	10
	dc.b	0
	cnop	0,4
l135
	dc.b	73
	dc.b	68
	dc.b	69
	dc.b	32
	dc.b	114
	dc.b	101
	dc.b	116
	dc.b	117
	dc.b	114
	dc.b	110
	dc.b	101
	dc.b	100
	dc.b	32
	dc.b	97
	dc.b	110
	dc.b	32
	dc.b	101
	dc.b	114
	dc.b	114
	dc.b	111
	dc.b	114
	dc.b	46
	dc.b	0
	cnop	0,4
l137
	dc.b	73
	dc.b	68
	dc.b	69
	dc.b	32
	dc.b	114
	dc.b	101
	dc.b	116
	dc.b	117
	dc.b	114
	dc.b	110
	dc.b	101
	dc.b	100
	dc.b	32
	dc.b	110
	dc.b	111
	dc.b	116
	dc.b	104
	dc.b	105
	dc.b	110
	dc.b	103
	dc.b	46
	dc.b	0
	cnop	0,4
l120
	dc.b	77
	dc.b	97
	dc.b	115
	dc.b	116
	dc.b	101
	dc.b	114
	dc.b	32
	dc.b	66
	dc.b	111
	dc.b	111
	dc.b	116
	dc.b	32
	dc.b	82
	dc.b	101
	dc.b	99
	dc.b	111
	dc.b	114
	dc.b	100
	dc.b	58
	dc.b	10
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_send
	cnop	0,4
_uart_send
	movem.l	l147,-(a7)
	move.w	(6+l149,a7),d3
	move.l	(8+l149,a7),a2
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_init
	moveq	#0,d2
	addq.w	#4,a7
	bra	l144
l143
	lea	(a2,d2.l),a0
	move.b	(a0),d0
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_put
	addq.w	#8,a7
l146
	addq.l	#1,d2
l144
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
	bhi	l143
l145
l141
l147	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l149	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_main
	cnop	0,4
_main
	movem.l	l159,-(a7)
	jsr	_initialize
	pea	l152
	move.l	#0,-(a7)
	jsr	_print
	pea	l153
	move.l	#1,-(a7)
	jsr	_print
	pea	l154
	move.l	#0,-(a7)
	jsr	_uart_send
	pea	l155
	jsr	_DEBUG
	add.w	#28,a7
l156
	bra	l156
l158
	moveq	#0,d0
l150
l159	reg
l161	equ	0
	rts
	cnop	0,4
l152
	dc.b	70
	dc.b	111
	dc.b	101
	dc.b	110
	dc.b	105
	dc.b	120
	dc.b	47
	dc.b	77
	dc.b	67
	dc.b	80
	dc.b	10
	dc.b	10
	dc.b	84
	dc.b	101
	dc.b	120
	dc.b	116
	dc.b	32
	dc.b	67
	dc.b	104
	dc.b	97
	dc.b	110
	dc.b	110
	dc.b	101
	dc.b	108
	dc.b	32
	dc.b	65
	dc.b	10
	dc.b	0
	cnop	0,4
l153
	dc.b	70
	dc.b	111
	dc.b	101
	dc.b	110
	dc.b	105
	dc.b	120
	dc.b	47
	dc.b	77
	dc.b	67
	dc.b	80
	dc.b	10
	dc.b	10
	dc.b	84
	dc.b	101
	dc.b	120
	dc.b	116
	dc.b	32
	dc.b	67
	dc.b	104
	dc.b	97
	dc.b	110
	dc.b	110
	dc.b	101
	dc.b	108
	dc.b	32
	dc.b	66
	dc.b	10
	dc.b	0
	cnop	0,4
l154
	dc.b	84
	dc.b	104
	dc.b	105
	dc.b	115
	dc.b	32
	dc.b	105
	dc.b	115
	dc.b	32
	dc.b	97
	dc.b	32
	dc.b	116
	dc.b	101
	dc.b	115
	dc.b	116
	dc.b	32
	dc.b	111
	dc.b	102
	dc.b	32
	dc.b	116
	dc.b	104
	dc.b	101
	dc.b	32
	dc.b	85
	dc.b	65
	dc.b	82
	dc.b	84
	dc.b	46
	dc.b	0
	cnop	0,4
l155
	dc.b	83
	dc.b	116
	dc.b	111
	dc.b	112
	dc.b	112
	dc.b	105
	dc.b	110
	dc.b	103
	dc.b	46
	dc.b	0
	public	_hex_digits
	section	"DATA",data
	cnop	0,4
_hex_digits
	dc.b	48
	dc.b	49
	dc.b	50
	dc.b	51
	dc.b	52
	dc.b	53
	dc.b	54
	dc.b	55
	dc.b	56
	dc.b	57
	dc.b	65
	dc.b	66
	dc.b	67
	dc.b	68
	dc.b	69
	dc.b	70
	dc.b	0
	public	_bdev_init_system
	public	_bdev_init
	public	_bdev_read
	public	_cdev_init_system
	public	_chan_write
	public	_chan_write_b
	public	_con_install
	public	_text_init
	public	_text_put_raw
	public	_pata_install
	public	_ps2_init
	public	_kbd_getc_poll
	public	_sdc_install
	public	_uart_init
	public	_uart_put
	public	_f_opendir
	public	_f_closedir
	public	_f_readdir
	public	_f_mount
	public	_DEBUG
	public	_number
	section	"BSS",bss
	cnop	0,4
_number
	ds.b	5
	public	_my_dir
	cnop	0,4
_my_dir
	ds.b	48
	public	_my_file
	cnop	0,4
_my_file
	ds.b	278
	public	_my_fs
	cnop	0,4
_my_fs
	ds.b	564
	public	_line
	cnop	0,4
_line
	ds.b	255
