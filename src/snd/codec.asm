	idnt	"codec.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_codec_wait
	cnop	0,4
_codec_wait
	movem.l	l6,-(a7)
l3
l4
	move.w	11668992,d0
	cmp.w	#32768,d0
	beq	l3
l5
l1
l6	reg
l8	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_init_codec
	cnop	0,4
_init_codec
	movem.l	l11,-(a7)
	move.w	#6656,11668992
	jsr	_codec_wait
	move.w	#10783,11668992
	jsr	_codec_wait
	move.w	#8961,11668992
	jsr	_codec_wait
	move.w	#11271,11668992
	jsr	_codec_wait
	move.w	#5122,11668992
	jsr	_codec_wait
	move.w	#5634,11668992
	jsr	_codec_wait
	move.w	#6213,11668992
	jsr	_codec_wait
l9
l11	reg
l13	equ	0
	rts
; stacksize=4
