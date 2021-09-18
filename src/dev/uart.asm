	idnt	"uart.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_uart_get_base
	cnop	0,4
_uart_get_base
	movem.l	l6,-(a7)
	move.w	(6+l8,a7),d0
	bne	l4
l3
	move.l	#12592120,d0
	bra	l1
l4
	move.l	#12591864,d0
l5
l1
l6	reg
l8	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_setbps
	cnop	0,4
_uart_setbps
	movem.l	l13,-(a7)
	move.w	(6+l15,a7),d3
	move.w	(10+l15,a7),d2
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_get_base
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l12
	lea	(3,a2),a0
	or.b	#128,(a0)
	moveq	#0,d0
	move.w	d2,d0
	and.l	#255,d0
	move.b	d0,(a2)
	moveq	#0,d0
	move.w	d2,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.b	d0,(1,a2)
	lea	(3,a2),a0
	and.b	#127,(a0)
l12
l9
l13	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l15	equ	12
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_setlcr
	cnop	0,4
_uart_setlcr
	movem.l	l22,-(a7)
	move.w	(6+l24,a7),d3
	move.b	(11+l24,a7),d2
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_get_base
	move.l	d0,a2
	pea	l18
	jsr	_DEBUG
	move.b	d2,(3,a2)
	addq.w	#8,a7
	cmp.b	(3,a2),d2
	beq	l20
l19
	pea	l21
	jsr	_DEBUG
	addq.w	#4,a7
l20
l16
l22	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l24	equ	12
	rts
	cnop	0,4
l21
	dc.b	76
	dc.b	67
	dc.b	82
	dc.b	32
	dc.b	109
	dc.b	105
	dc.b	115
	dc.b	109
	dc.b	97
	dc.b	116
	dc.b	99
	dc.b	104
	dc.b	101
	dc.b	100
	dc.b	33
	dc.b	0
	cnop	0,4
l18
	dc.b	117
	dc.b	97
	dc.b	114
	dc.b	116
	dc.b	95
	dc.b	115
	dc.b	101
	dc.b	116
	dc.b	108
	dc.b	99
	dc.b	114
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_init
	cnop	0,4
_uart_init
	movem.l	l30,-(a7)
	move.w	(6+l32,a7),d2
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_get_base
	move.l	d0,a2
	pea	l27
	jsr	_DEBUG
	addq.w	#8,a7
	move.l	a2,d0
	beq	l29
	move.l	#1,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_setbps
	move.l	#3,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_setlcr
	move.b	#193,(2,a2)
	add.w	#16,a7
l29
l25
l30	reg	a2/d2
	movem.l	(a7)+,a2/d2
l32	equ	8
	rts
	cnop	0,4
l27
	dc.b	117
	dc.b	97
	dc.b	114
	dc.b	116
	dc.b	95
	dc.b	105
	dc.b	110
	dc.b	105
	dc.b	116
	dc.b	0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_has_bytes
	cnop	0,4
_uart_has_bytes
	movem.l	l41,-(a7)
	move.w	(6+l43,a7),d2
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_get_base
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l36
	moveq	#0,d0
	move.b	(5,a2),d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l38
	moveq	#1,d0
	bra	l33
l38
	moveq	#0,d0
	bra	l33
l39
	bra	l40
l36
	moveq	#0,d0
l40
l33
l41	reg	a2/d2
	movem.l	(a7)+,a2/d2
l43	equ	8
	rts
; stacksize=16
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_put
	cnop	0,4
_uart_put
	movem.l	l51,-(a7)
	move.b	(11+l53,a7),d4
	move.w	(6+l53,a7),d3
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_get_base
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l47
	moveq	#0,d2
l48
	move.b	(5,a2),d2
l50
	moveq	#0,d0
	move.b	d2,d0
	and.b	#32,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l48
l49
	move.b	d4,(a2)
l47
l44
l51	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l53	equ	16
	rts
; stacksize=24
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_uart_get
	cnop	0,4
_uart_get
	movem.l	l61,-(a7)
	move.w	(6+l63,a7),d3
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_uart_get_base
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l57
	moveq	#0,d2
l58
	move.b	(5,a2),d2
l60
	moveq	#0,d0
	move.b	d2,d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l58
l59
	move.b	(a2),d0
l57
l54
l61	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l63	equ	12
	rts
; stacksize=20
	public	_DEBUG
