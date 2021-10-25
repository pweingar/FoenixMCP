	idnt	"psg.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_psg_mute_all
	cnop	0,4
_psg_mute_all
	movem.l	l7,-(a7)
	moveq	#0,d2
	bra	l4
l3
	move.l	#15,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_psg_attenuation
	addq.w	#8,a7
l6
	addq.w	#1,d2
l4
	cmp.w	#3,d2
	blt	l3
l5
l1
l7	reg	d2
	movem.l	(a7)+,d2
l9	equ	4
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_psg_tone
	cnop	0,4
_psg_tone
	movem.l	l14,-(a7)
	move.l	(8+l16,a7),d4
	move.w	(6+l16,a7),d3
	cmp.w	#3,d3
	bcc	l13
l12
	move.l	d4,d0
	lsl.l	#5,d0
	move.l	d0,-(a7)
	move.l	d0,-(a7)
	move.l	#357954500,-(a7)
	public	__ldivs
	jsr	__ldivs
	addq.w	#8,a7
	move.l	d0,d2
	opt	om-
	movem.l	(a7)+,d0
	opt	om+
	move.b	d3,d0
	and.b	#3,d0
	and.w	#255,d0
	and.l	#65535,d0
	lsl.l	#5,d0
	or.l	#128,d0
	moveq	#15,d1
	and.l	d2,d1
	or.l	d1,d0
	move.b	d0,12714240
	move.w	d2,d0
	and.w	#1008,d0
	and.l	#65535,d0
	asr.l	#4,d0
	move.b	d0,12714240
l13
l10
l14	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l16	equ	12
	rts
; stacksize=24
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_psg_attenuation
	cnop	0,4
_psg_attenuation
	movem.l	l19,-(a7)
	move.w	(10+l21,a7),d3
	move.w	(6+l21,a7),d2
	move.b	d2,d0
	and.b	#3,d0
	and.w	#255,d0
	and.l	#65535,d0
	lsl.l	#5,d0
	or.l	#144,d0
	move.w	d3,d1
	ext.l	d1
	and.l	#15,d1
	or.l	d1,d0
	move.b	d0,12714240
l17
l19	reg	d2/d3
	movem.l	(a7)+,d2/d3
l21	equ	8
	rts
; stacksize=8
