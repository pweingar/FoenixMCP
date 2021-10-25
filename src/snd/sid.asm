	idnt	"sid.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_sid_get_base
	cnop	0,4
_sid_get_base
	movem.l	l8,-(a7)
	move.w	(6+l10,a7),d1
	move.w	d1,d0
	sub.w	#0,d0
	beq	l4
	subq.w	#1,d0
	beq	l5
	subq.w	#1,d0
	beq	l6
	bra	l7
l4
	move.l	#11670528,d0
	bra	l1
l5
	move.l	#11669504,d0
	bra	l1
l6
	move.l	#11670016,d0
	bra	l1
l7
	moveq	#0,d0
l3
l1
l8	reg
l10	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_init
	cnop	0,4
_sid_init
	movem.l	l19,-(a7)
	move.w	(6+l21,a7),d3
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_sid_get_base
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l14
	moveq	#0,d2
	bra	l16
l15
	move.b	#0,(0,a2,d2.l)
l18
	addq.l	#1,d2
l16
	moveq	#25,d0
	cmp.l	d2,d0
	bgt	l15
l17
l14
l11
l19	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l21	equ	12
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_init_all
	cnop	0,4
_sid_init_all
	movem.l	l28,-(a7)
	moveq	#0,d2
	bra	l25
l24
	move.l	d2,-(a7)
	jsr	_sid_init
	addq.w	#4,a7
l27
	addq.l	#1,d2
l25
	moveq	#5,d0
	cmp.l	d2,d0
	bgt	l24
l26
l22
l28	reg	d2
	movem.l	(a7)+,d2
l30	equ	4
	rts
; stacksize=32
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_test_internal
	cnop	0,4
_sid_test_internal
	movem.l	l65,-(a7)
	move.b	#41,11669509
	move.b	#41,11669516
	move.b	#41,11669523
	move.b	#41,11670021
	move.b	#41,11670028
	move.b	#41,11670035
	move.b	#31,11669510
	move.b	#31,11669517
	move.b	#31,11669524
	move.b	#31,11670022
	move.b	#31,11670029
	move.b	#31,11670036
	move.b	#15,11669528
	move.b	#15,11670040
	move.b	#96,11669504
	move.b	#22,11669505
	move.b	#96,11670016
	move.b	#22,11670017
	move.b	#17,11669508
	move.b	#17,11670020
	moveq	#0,d1
	bra	l34
l33
l36
	addq.l	#1,d1
l34
	cmp.l	#65536,d1
	bcs	l33
l35
	move.b	#49,11669511
	move.b	#8,11669512
	move.b	#49,11670023
	move.b	#8,11670024
	move.b	#17,11669515
	move.b	#17,11670027
	moveq	#0,d1
	bra	l38
l37
l40
	addq.l	#1,d1
l38
	cmp.l	#65536,d1
	bcs	l37
l39
	move.b	#135,11669518
	move.b	#33,11669519
	move.b	#135,11670030
	move.b	#33,11670031
	move.b	#17,11669522
	move.b	#17,11670034
	moveq	#0,d1
	bra	l42
l41
l44
	addq.l	#1,d1
l42
	cmp.l	#262144,d1
	bcs	l41
l43
	move.b	#16,11669508
	move.b	#16,11670020
	moveq	#0,d1
	bra	l46
l45
l48
	addq.l	#1,d1
l46
	cmp.l	#8192,d1
	bcs	l45
l47
	move.b	#16,11669515
	move.b	#16,11670027
	moveq	#0,d1
	bra	l50
l49
l52
	addq.l	#1,d1
l50
	cmp.l	#8192,d1
	bcs	l49
l51
	move.b	#16,11669515
	move.b	#16,11670027
	moveq	#0,d1
	bra	l54
l53
l56
	addq.l	#1,d1
l54
	cmp.l	#32768,d1
	bcs	l53
l55
	moveq	#0,d2
	bra	l58
l57
	moveq	#0,d1
	bra	l62
l61
l64
	addq.l	#1,d1
l62
	cmp.l	#1024,d1
	bcs	l61
l63
	moveq	#15,d0
	sub.b	d2,d0
	move.b	d0,11669528
	moveq	#15,d0
	sub.b	d2,d0
	move.b	d0,11670040
l60
	addq.b	#1,d2
l58
	cmp.b	#16,d2
	bcs	l57
l59
l31
l65	reg	d2
	movem.l	(a7)+,d2
l67	equ	4
	rts
; stacksize=4
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_sid_text_external
	cnop	0,4
_sid_text_external
	movem.l	l70,-(a7)
l68
l70	reg
l72	equ	0
	rts
; stacksize=0
