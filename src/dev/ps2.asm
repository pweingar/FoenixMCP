	idnt	"ps2.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_ps2_wait_out
	cnop	0,4
_ps2_wait_out
	movem.l	l8,-(a7)
	move.l	#12591204,a0
	moveq	#0,d1
	bra	l4
l3
	move.w	d1,d0
	addq.w	#1,d1
	cmp.w	#1000,d0
	ble	l7
l6
	moveq	#-1,d0
	bra	l1
l7
l4
	moveq	#0,d0
	move.b	(a0),d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l3
l5
	moveq	#0,d0
l1
l8	reg
l10	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_wait_in
	cnop	0,4
_ps2_wait_in
	movem.l	l18,-(a7)
	move.l	#12591204,a0
	moveq	#0,d1
	bra	l14
l13
	move.w	d1,d0
	addq.w	#1,d1
	cmp.w	#1000,d0
	ble	l17
l16
	moveq	#-1,d0
	bra	l11
l17
l14
	moveq	#0,d0
	move.b	(a0),d0
	and.b	#2,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l13
l15
	moveq	#0,d0
l11
l18	reg
l20	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_controller_cmd
	cnop	0,4
_ps2_controller_cmd
	movem.l	l27,-(a7)
	move.b	(7+l29,a7),d2
	move.l	#12591200,a2
	move.l	#12591200,a3
	jsr	_ps2_wait_in
	tst.w	d0
	beq	l24
	moveq	#-1,d0
	bra	l21
l24
	move.b	d2,(a2)
	jsr	_ps2_wait_out
	tst.w	d0
	beq	l26
	moveq	#-1,d0
	bra	l21
l26
	moveq	#0,d0
	move.b	(a3),d0
l21
l27	reg	a2/a3/d2
	movem.l	(a7)+,a2/a3/d2
l29	equ	12
	rts
; stacksize=16
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_controller_cmd_param
	cnop	0,4
_ps2_controller_cmd_param
	movem.l	l36,-(a7)
	move.b	(11+l38,a7),d3
	move.b	(7+l38,a7),d2
	move.l	#12591200,a2
	move.l	#12591200,a3
	jsr	_ps2_wait_in
	tst.w	d0
	beq	l33
	moveq	#-1,d0
	bra	l30
l33
	move.b	d2,(a2)
	jsr	_ps2_wait_in
	tst.w	d0
	beq	l35
	moveq	#-1,d0
	bra	l30
l35
	move.b	d3,(a3)
	moveq	#0,d0
l30
l36	reg	a2/a3/d2/d3
	movem.l	(a7)+,a2/a3/d2/d3
l38	equ	16
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_kbd_cmd_p
	cnop	0,4
_ps2_kbd_cmd_p
	movem.l	l45,-(a7)
	move.b	(11+l47,a7),d3
	move.b	(7+l47,a7),d2
	move.l	#12591200,a3
	move.l	#12591200,a2
	jsr	_ps2_wait_in
	tst.w	d0
	beq	l42
	moveq	#-1,d0
	bra	l39
l42
	move.b	d2,(a2)
	jsr	_ps2_wait_in
	tst.w	d0
	beq	l44
	moveq	#-1,d0
	bra	l39
l44
	move.b	d3,(a2)
	moveq	#0,d0
l39
l45	reg	a2/a3/d2/d3
	movem.l	(a7)+,a2/a3/d2/d3
l47	equ	16
	rts
; stacksize=20
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_kbd_cmd
	cnop	0,4
_ps2_kbd_cmd
	movem.l	l57,-(a7)
	move.b	(7+l59,a7),d3
	move.w	(10+l59,a7),d2
	move.l	#12591200,a2
	jsr	_ps2_wait_in
	tst.w	d0
	beq	l51
	moveq	#-1,d0
	bra	l48
l51
	move.b	d3,(a2)
l52
l53
	move.w	d2,d0
	subq.w	#1,d2
	tst.w	d0
	bgt	l52
l54
	jsr	_ps2_wait_out
	tst.w	d0
	beq	l56
	moveq	#-1,d0
	bra	l48
l56
	moveq	#0,d0
	move.b	(a2),d0
l48
l57	reg	a2/d2/d3
	movem.l	(a7)+,a2/d2/d3
l59	equ	12
	rts
; stacksize=16
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_flush_out
	cnop	0,4
_ps2_flush_out
	movem.l	l65,-(a7)
	move.l	#12591204,a0
	move.l	#12591200,a1
	bra	l63
l62
	move.b	(a1),d1
l63
	moveq	#0,d0
	move.b	(a0),d0
	and.b	#1,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l62
l64
l60
l65	reg
l67	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_toggle_modifier
	cnop	0,4
_kbd_toggle_modifier
	movem.l	l70,-(a7)
	move.w	(6+l72,a7),d1
	move.b	d1,d0
	eor.b	d0,526+_g_kbd_control
l68
l70	reg
l72	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_makebreak_modifier
	cnop	0,4
_kbd_makebreak_modifier
	movem.l	l78,-(a7)
	move.w	(10+l80,a7),d2
	move.w	(6+l80,a7),d1
	tst.w	d2
	beq	l76
	move.w	d1,d0
	ext.l	d0
	not.l	d0
	and.b	d0,526+_g_kbd_control
	bra	l77
l76
	move.b	d1,d0
	or.b	d0,526+_g_kbd_control
l77
l73
l78	reg	d2
	movem.l	(a7)+,d2
l80	equ	4
	rts
; stacksize=4
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_enqueue_scan
	cnop	0,4
_kbd_enqueue_scan
	movem.l	l105,-(a7)
	move.b	(7+l107,a7),d3
	beq	l84
l85
	cmp.b	#128,d3
	beq	l84
l83
	move.b	#128,d2
	and.b	d3,d2
	moveq	#0,d0
	move.b	d3,d0
	and.b	#127,d0
	and.w	#255,d0
	and.l	#65535,d0
	moveq	#29,d1
	cmp.l	d0,d1
	beq	l89
	moveq	#42,d1
	cmp.l	d0,d1
	beq	l87
	moveq	#54,d1
	cmp.l	d0,d1
	beq	l88
	moveq	#56,d1
	cmp.l	d0,d1
	beq	l91
	moveq	#58,d1
	cmp.l	d0,d1
	beq	l95
	moveq	#69,d1
	cmp.l	d0,d1
	beq	l98
	moveq	#70,d1
	cmp.l	d0,d1
	beq	l101
	moveq	#91,d1
	cmp.l	d0,d1
	beq	l94
	moveq	#92,d1
	cmp.l	d0,d1
	beq	l92
	moveq	#93,d1
	cmp.l	d0,d1
	beq	l93
	moveq	#94,d1
	cmp.l	d0,d1
	beq	l90
	bra	l104
l87
l88
	moveq	#0,d0
	move.b	d2,d0
	move.l	d0,-(a7)
	move.l	#8,-(a7)
	jsr	_kbd_makebreak_modifier
	addq.w	#8,a7
	bra	l86
l89
l90
	moveq	#0,d0
	move.b	d2,d0
	move.l	d0,-(a7)
	move.l	#16,-(a7)
	jsr	_kbd_makebreak_modifier
	addq.w	#8,a7
	bra	l86
l91
l92
	moveq	#0,d0
	move.b	d2,d0
	move.l	d0,-(a7)
	move.l	#32,-(a7)
	jsr	_kbd_makebreak_modifier
	addq.w	#8,a7
	bra	l86
l93
	moveq	#0,d0
	move.b	d2,d0
	move.l	d0,-(a7)
	move.l	#128,-(a7)
	jsr	_kbd_makebreak_modifier
	addq.w	#8,a7
	bra	l86
l94
	moveq	#0,d0
	move.b	d2,d0
	move.l	d0,-(a7)
	move.l	#64,-(a7)
	jsr	_kbd_makebreak_modifier
	addq.w	#8,a7
	bra	l86
l95
	tst.b	d2
	bne	l97
l96
	move.l	#4,-(a7)
	jsr	_kbd_toggle_modifier
	addq.w	#4,a7
l97
	bra	l86
l98
	tst.b	d2
	bne	l100
l99
	move.l	#2,-(a7)
	jsr	_kbd_toggle_modifier
	addq.w	#4,a7
l100
	bra	l86
l101
	tst.b	d2
	bne	l103
l102
	move.l	#1,-(a7)
	jsr	_kbd_toggle_modifier
	addq.w	#4,a7
l103
	bra	l86
l104
l86
	moveq	#0,d0
	move.b	526+_g_kbd_control,d0
	lsl.l	#8,d0
	moveq	#0,d1
	move.b	d3,d1
	or.l	d1,d0
	move.l	d0,-(a7)
	pea	6+_g_kbd_control
	jsr	_rb_word_put
	addq.w	#8,a7
l84
l81
l105	reg	d2/d3
	movem.l	(a7)+,d2/d3
l107	equ	8
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_get_scancode
	cnop	0,4
_kbd_get_scancode
	movem.l	l110,-(a7)
	pea	6+_g_kbd_control
	jsr	_rb_word_get
	addq.w	#4,a7
l108
l110	reg
l112	equ	0
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_handle_irq
	cnop	0,4
_kbd_handle_irq
	movem.l	l172,-(a7)
	move.l	#12591200,a2
	move.b	(a2),d2
	beq	l116
	beq	l118
l119
	cmp.b	#128,d2
	beq	l118
l117
	move.b	#128,d4
	and.b	d2,d4
	move.l	_g_kbd_control,d0
	move.l	d0,d1
	cmp.l	#10,d1
	bhi	l171
	lsl.l	#2,d1
	move.l	l175(pc,d1.l),a0
	jmp	(a0)
	cnop	0,4
l175
	dc.l	l121
	dc.l	l128
	dc.l	l135
	dc.l	l139
	dc.l	l151
	dc.l	l155
	dc.l	l159
	dc.l	l163
	dc.l	l167
	dc.l	l143
	dc.l	l147
l121
	move.b	d2,d0
	sub.b	#224,d0
	beq	l123
	subq.b	#1,d0
	beq	l124
	bra	l125
l123
	moveq	#1,d0
	move.l	d0,_g_kbd_control
	bra	l122
l124
	moveq	#4,d0
	move.l	d0,_g_kbd_control
	bra	l122
l125
	moveq	#0,d0
	move.b	d2,d0
	and.w	#255,d0
	and.b	#127,d0
	and.w	#255,d0
	and.l	#65535,d0
	and.l	#65535,d0
	lea	_g_kbd_set1_base,a0
	move.b	(0,a0,d0.l),d3
	beq	l127
l126
	moveq	#0,d0
	move.b	d3,d0
	moveq	#0,d1
	move.b	d4,d1
	or.l	d1,d0
	move.l	d0,-(a7)
	jsr	_kbd_enqueue_scan
	addq.w	#4,a7
l127
l122
	bra	l120
l128
	move.b	d2,d0
	sub.b	#42,d0
	beq	l130
	sub.b	#141,d0
	beq	l131
	bra	l132
l130
	moveq	#2,d0
	move.l	d0,_g_kbd_control
	bra	l129
l131
	moveq	#9,d0
	move.l	d0,_g_kbd_control
	bra	l129
l132
	moveq	#0,d0
	move.b	d2,d0
	and.w	#255,d0
	and.b	#127,d0
	and.w	#255,d0
	and.l	#65535,d0
	and.l	#65535,d0
	lea	_g_kbd_set1_e0,a0
	move.b	(0,a0,d0.l),d3
	beq	l134
l133
	moveq	#0,d0
	move.b	d3,d0
	moveq	#0,d1
	move.b	d4,d1
	or.l	d1,d0
	move.l	d0,-(a7)
	jsr	_kbd_enqueue_scan
	addq.w	#4,a7
l134
	move.l	#0,_g_kbd_control
l129
	bra	l120
l135
	move.b	d2,d0
	sub.b	#224,d0
	beq	l137
	bra	l138
l137
	moveq	#3,d0
	move.l	d0,_g_kbd_control
	bra	l136
l138
	move.l	#0,_g_kbd_control
l136
	bra	l120
l139
	move.b	d2,d0
	sub.b	#55,d0
	beq	l141
	bra	l142
l141
	move.l	#0,_g_kbd_control
	move.l	#96,-(a7)
	jsr	_kbd_enqueue_scan
	addq.w	#4,a7
	bra	l140
l142
	move.l	#0,_g_kbd_control
l140
	bra	l120
l143
	move.b	d2,d0
	sub.b	#224,d0
	beq	l145
	bra	l146
l145
	moveq	#10,d0
	move.l	d0,_g_kbd_control
	bra	l144
l146
	move.l	#0,_g_kbd_control
l144
	bra	l120
l147
	move.b	d2,d0
	sub.b	#170,d0
	beq	l149
	bra	l150
l149
	move.l	#0,_g_kbd_control
	move.l	#208,-(a7)
	jsr	_kbd_enqueue_scan
	addq.w	#4,a7
	bra	l148
l150
	move.l	#0,_g_kbd_control
l148
	bra	l120
l151
	move.b	d2,d0
	sub.b	#29,d0
	beq	l153
	bra	l154
l153
	moveq	#5,d0
	move.l	d0,_g_kbd_control
	bra	l152
l154
	move.l	#0,_g_kbd_control
l152
	bra	l120
l155
	move.b	d2,d0
	sub.b	#69,d0
	beq	l157
	bra	l158
l157
	moveq	#6,d0
	move.l	d0,_g_kbd_control
	bra	l156
l158
	move.l	#0,_g_kbd_control
l156
	bra	l120
l159
	move.b	d2,d0
	sub.b	#225,d0
	beq	l161
	bra	l162
l161
	moveq	#7,d0
	move.l	d0,_g_kbd_control
	bra	l160
l162
	move.l	#0,_g_kbd_control
l160
	bra	l120
l163
	move.b	d2,d0
	sub.b	#157,d0
	beq	l165
	bra	l166
l165
	moveq	#8,d0
	move.l	d0,_g_kbd_control
	bra	l164
l166
	move.l	#0,_g_kbd_control
l164
	bra	l120
l167
	move.b	d2,d0
	sub.b	#197,d0
	beq	l169
	bra	l170
l169
	move.l	#0,_g_kbd_control
	move.l	#97,-(a7)
	jsr	_kbd_enqueue_scan
	addq.w	#4,a7
	bra	l168
l170
	move.l	#0,_g_kbd_control
l168
l171
l120
l118
l116
l113
l172	reg	a2/d2/d3/d4
	movem.l	(a7)+,a2/d2/d3/d4
l174	equ	16
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_getc
	cnop	0,4
_kbd_getc
	movem.l	l204,-(a7)
	pea	266+_g_kbd_control
	jsr	_rb_word_empty
	addq.w	#4,a7
	tst.w	d0
	bne	l179
l178
	pea	266+_g_kbd_control
	jsr	_rb_word_get
	addq.w	#4,a7
	bra	l176
l179
	jsr	_kbd_get_scancode
	move.w	d0,d4
	bra	l182
l181
	move.b	d4,d0
	and.b	#128,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l185
l184
	moveq	#0,d0
	move.w	d4,d0
	asr.l	#8,d0
	and.l	#255,d0
	move.b	d0,d3
	moveq	#0,d0
	move.w	d4,d0
	and.l	#127,d0
	move.b	d0,d2
	cmp.b	#56,d2
	bcc	l187
l186
	moveq	#0,d0
	move.b	d3,d0
	and.b	#28,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	bne	l189
l188
	moveq	#0,d0
	move.b	d2,d0
	move.l	528+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l189
	moveq	#0,d0
	move.b	d3,d0
	and.b	#16,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l192
	moveq	#0,d0
	move.b	d3,d0
	and.b	#8,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l194
	moveq	#0,d0
	move.b	d2,d0
	move.l	540+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l194
	moveq	#0,d0
	move.b	d2,d0
	move.l	536+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l195
	bra	l196
l192
	moveq	#0,d0
	move.b	d3,d0
	and.b	#4,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l198
	moveq	#0,d0
	move.b	d3,d0
	and.b	#8,d0
	and.w	#255,d0
	and.l	#65535,d0
	tst.l	d0
	beq	l200
	moveq	#0,d0
	move.b	d2,d0
	move.l	548+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l200
	moveq	#0,d0
	move.b	d2,d0
	move.l	544+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l201
	bra	l202
l198
	moveq	#0,d0
	move.b	d2,d0
	move.l	532+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l202
l196
l190
	bra	l203
l187
	moveq	#0,d0
	move.b	d2,d0
	move.l	528+_g_kbd_control,a0
	move.b	(0,a0,d0.l),d0
	bra	l176
l203
l185
	jsr	_kbd_get_scancode
	move.w	d0,d4
l182
	tst.w	d4
	bne	l181
l183
	moveq	#0,d0
l180
l176
l204	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l206	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_ps2_init
	cnop	0,4
_ps2_init
	movem.l	l213,-(a7)
	move.l	#12591200,a2
	move.l	#12591200,a3
	move.l	#0,_g_kbd_control
	pea	6+_g_kbd_control
	jsr	_rb_word_init
	pea	266+_g_kbd_control
	jsr	_rb_word_init
	move.l	#_g_us_sc_unmodified,528+_g_kbd_control
	move.l	#_g_us_sc_shift,532+_g_kbd_control
	move.l	#_g_us_sc_ctrl,536+_g_kbd_control
	move.l	#_g_us_sc_ctrl_shift,540+_g_kbd_control
	move.l	#_g_us_sc_lock,544+_g_kbd_control
	move.l	#_g_us_sc_lock_shift,548+_g_kbd_control
	move.l	#173,-(a7)
	jsr	_ps2_controller_cmd
	move.l	#167,-(a7)
	jsr	_ps2_controller_cmd
	jsr	_ps2_flush_out
	move.l	#170,-(a7)
	jsr	_ps2_controller_cmd
	add.w	#20,a7
	cmp.w	#85,d0
	beq	l210
l209
	moveq	#-1,d0
	bra	l207
l210
	move.l	#171,-(a7)
	jsr	_ps2_controller_cmd
	addq.w	#4,a7
	tst.w	d0
	beq	l212
l211
	moveq	#-2,d0
	bra	l207
l212
	move.l	#67,-(a7)
	move.l	#96,-(a7)
	jsr	_ps2_controller_cmd_param
	jsr	_ps2_wait_in
	move.b	#174,(a2)
	move.l	#1000,-(a7)
	move.l	#255,-(a7)
	jsr	_ps2_kbd_cmd
	move.l	#0,-(a7)
	move.l	#244,-(a7)
	jsr	_ps2_kbd_cmd
	jsr	_ps2_flush_out
	moveq	#0,d0
	add.w	#24,a7
l207
l213	reg	a2/a3
	movem.l	(a7)+,a2/a3
l215	equ	8
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_kbd_getc_poll
	cnop	0,4
_kbd_getc_poll
	movem.l	l221,-(a7)
	jsr	_ps2_wait_out
	tst.w	d0
	bne	l219
l218
	jsr	_kbd_handle_irq
	jsr	_kbd_getc
	bra	l216
l219
	moveq	#0,d0
l220
l216
l221	reg
l223	equ	0
	rts
	public	_g_kbd_set1_base
	section	"DATA",data
	cnop	0,4
_g_kbd_set1_base
	dc.b	0
	dc.b	1
	dc.b	2
	dc.b	3
	dc.b	4
	dc.b	5
	dc.b	6
	dc.b	7
	dc.b	8
	dc.b	9
	dc.b	10
	dc.b	11
	dc.b	12
	dc.b	13
	dc.b	14
	dc.b	15
	dc.b	16
	dc.b	17
	dc.b	18
	dc.b	19
	dc.b	20
	dc.b	21
	dc.b	22
	dc.b	23
	dc.b	24
	dc.b	25
	dc.b	26
	dc.b	27
	dc.b	28
	dc.b	29
	dc.b	30
	dc.b	31
	dc.b	32
	dc.b	33
	dc.b	34
	dc.b	35
	dc.b	36
	dc.b	37
	dc.b	38
	dc.b	39
	dc.b	40
	dc.b	41
	dc.b	42
	dc.b	43
	dc.b	44
	dc.b	45
	dc.b	46
	dc.b	47
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
	dc.b	58
	dc.b	59
	dc.b	60
	dc.b	61
	dc.b	62
	dc.b	63
	dc.b	64
	dc.b	65
	dc.b	66
	dc.b	67
	dc.b	68
	dc.b	69
	dc.b	70
	dc.b	71
	dc.b	72
	dc.b	73
	dc.b	74
	dc.b	75
	dc.b	76
	dc.b	77
	dc.b	78
	dc.b	79
	dc.b	80
	dc.b	81
	dc.b	82
	dc.b	83
	dc.b	84
	dc.b	85
	dc.b	86
	dc.b	87
	dc.b	88
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_kbd_set1_e0
	cnop	0,4
_g_kbd_set1_e0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	112
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	113
	dc.b	0
	dc.b	0
	dc.b	109
	dc.b	94
	dc.b	0
	dc.b	0
	dc.b	114
	dc.b	0
	dc.b	110
	dc.b	0
	dc.b	111
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	115
	dc.b	0
	dc.b	116
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	108
	dc.b	0
	dc.b	0
	dc.b	92
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	97
	dc.b	99
	dc.b	104
	dc.b	100
	dc.b	0
	dc.b	105
	dc.b	0
	dc.b	107
	dc.b	0
	dc.b	102
	dc.b	106
	dc.b	103
	dc.b	98
	dc.b	101
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	93
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_us_sc_unmodified
	cnop	0,4
_g_us_sc_unmodified
	dc.b	0
	dc.b	27
	dc.b	49
	dc.b	50
	dc.b	51
	dc.b	52
	dc.b	53
	dc.b	54
	dc.b	55
	dc.b	56
	dc.b	57
	dc.b	48
	dc.b	45
	dc.b	61
	dc.b	8
	dc.b	9
	dc.b	113
	dc.b	119
	dc.b	101
	dc.b	114
	dc.b	116
	dc.b	121
	dc.b	117
	dc.b	105
	dc.b	111
	dc.b	112
	dc.b	91
	dc.b	93
	dc.b	13
	dc.b	0
	dc.b	97
	dc.b	115
	dc.b	100
	dc.b	102
	dc.b	103
	dc.b	104
	dc.b	106
	dc.b	107
	dc.b	108
	dc.b	59
	dc.b	39
	dc.b	96
	dc.b	0
	dc.b	92
	dc.b	122
	dc.b	120
	dc.b	99
	dc.b	118
	dc.b	98
	dc.b	110
	dc.b	109
	dc.b	44
	dc.b	46
	dc.b	47
	dc.b	0
	dc.b	42
	dc.b	0
	dc.b	32
	dc.b	0
	dc.b	-118
	dc.b	-117
	dc.b	-116
	dc.b	-115
	dc.b	-114
	dc.b	-113
	dc.b	-112
	dc.b	-111
	dc.b	-110
	dc.b	-109
	dc.b	0
	dc.b	0
	dc.b	-128
	dc.b	-122
	dc.b	-124
	dc.b	45
	dc.b	-119
	dc.b	53
	dc.b	-120
	dc.b	43
	dc.b	-125
	dc.b	-121
	dc.b	-123
	dc.b	-127
	dc.b	-126
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-108
	dc.b	-107
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-127
	dc.b	-128
	dc.b	-124
	dc.b	-126
	dc.b	-125
	dc.b	-123
	dc.b	-122
	dc.b	-119
	dc.b	-121
	dc.b	-120
	dc.b	47
	dc.b	13
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_us_sc_shift
	cnop	0,4
_g_us_sc_shift
	dc.b	0
	dc.b	27
	dc.b	33
	dc.b	64
	dc.b	35
	dc.b	36
	dc.b	37
	dc.b	94
	dc.b	38
	dc.b	42
	dc.b	40
	dc.b	41
	dc.b	95
	dc.b	43
	dc.b	8
	dc.b	9
	dc.b	81
	dc.b	87
	dc.b	69
	dc.b	82
	dc.b	84
	dc.b	89
	dc.b	85
	dc.b	73
	dc.b	79
	dc.b	80
	dc.b	123
	dc.b	125
	dc.b	10
	dc.b	0
	dc.b	65
	dc.b	83
	dc.b	68
	dc.b	70
	dc.b	71
	dc.b	72
	dc.b	74
	dc.b	75
	dc.b	76
	dc.b	58
	dc.b	34
	dc.b	126
	dc.b	0
	dc.b	124
	dc.b	90
	dc.b	88
	dc.b	67
	dc.b	86
	dc.b	66
	dc.b	78
	dc.b	77
	dc.b	60
	dc.b	62
	dc.b	63
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	32
	dc.b	0
	dc.b	-118
	dc.b	-117
	dc.b	-116
	dc.b	-115
	dc.b	-114
	dc.b	-113
	dc.b	-112
	dc.b	-111
	dc.b	-110
	dc.b	-109
	dc.b	0
	dc.b	0
	dc.b	-128
	dc.b	-122
	dc.b	-124
	dc.b	45
	dc.b	-119
	dc.b	53
	dc.b	-120
	dc.b	43
	dc.b	-125
	dc.b	-121
	dc.b	-123
	dc.b	-127
	dc.b	-126
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-108
	dc.b	-107
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-127
	dc.b	-128
	dc.b	-124
	dc.b	-126
	dc.b	-125
	dc.b	-123
	dc.b	-122
	dc.b	-119
	dc.b	-121
	dc.b	-120
	dc.b	47
	dc.b	13
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_us_sc_ctrl
	cnop	0,4
_g_us_sc_ctrl
	dc.b	0
	dc.b	27
	dc.b	49
	dc.b	50
	dc.b	51
	dc.b	52
	dc.b	53
	dc.b	30
	dc.b	55
	dc.b	56
	dc.b	57
	dc.b	48
	dc.b	31
	dc.b	61
	dc.b	8
	dc.b	9
	dc.b	17
	dc.b	23
	dc.b	5
	dc.b	18
	dc.b	20
	dc.b	25
	dc.b	21
	dc.b	9
	dc.b	15
	dc.b	16
	dc.b	27
	dc.b	29
	dc.b	10
	dc.b	0
	dc.b	1
	dc.b	19
	dc.b	4
	dc.b	6
	dc.b	7
	dc.b	8
	dc.b	10
	dc.b	11
	dc.b	12
	dc.b	59
	dc.b	34
	dc.b	96
	dc.b	0
	dc.b	92
	dc.b	26
	dc.b	24
	dc.b	3
	dc.b	22
	dc.b	2
	dc.b	14
	dc.b	13
	dc.b	44
	dc.b	46
	dc.b	28
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	32
	dc.b	0
	dc.b	-118
	dc.b	-117
	dc.b	-116
	dc.b	-115
	dc.b	-114
	dc.b	-113
	dc.b	-112
	dc.b	-111
	dc.b	-110
	dc.b	-109
	dc.b	0
	dc.b	0
	dc.b	-128
	dc.b	-122
	dc.b	-124
	dc.b	45
	dc.b	-119
	dc.b	53
	dc.b	-120
	dc.b	43
	dc.b	-125
	dc.b	-121
	dc.b	-123
	dc.b	-127
	dc.b	-126
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-108
	dc.b	-107
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-127
	dc.b	-128
	dc.b	-124
	dc.b	-126
	dc.b	-125
	dc.b	-123
	dc.b	-122
	dc.b	-119
	dc.b	-121
	dc.b	-120
	dc.b	47
	dc.b	13
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_us_sc_lock
	cnop	0,4
_g_us_sc_lock
	dc.b	0
	dc.b	27
	dc.b	49
	dc.b	50
	dc.b	51
	dc.b	52
	dc.b	53
	dc.b	54
	dc.b	55
	dc.b	56
	dc.b	57
	dc.b	48
	dc.b	45
	dc.b	61
	dc.b	8
	dc.b	9
	dc.b	81
	dc.b	87
	dc.b	69
	dc.b	82
	dc.b	84
	dc.b	89
	dc.b	85
	dc.b	73
	dc.b	79
	dc.b	80
	dc.b	91
	dc.b	93
	dc.b	13
	dc.b	0
	dc.b	65
	dc.b	83
	dc.b	68
	dc.b	70
	dc.b	71
	dc.b	72
	dc.b	74
	dc.b	75
	dc.b	76
	dc.b	59
	dc.b	39
	dc.b	96
	dc.b	0
	dc.b	92
	dc.b	90
	dc.b	88
	dc.b	67
	dc.b	86
	dc.b	66
	dc.b	78
	dc.b	77
	dc.b	44
	dc.b	46
	dc.b	47
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	32
	dc.b	0
	dc.b	-118
	dc.b	-117
	dc.b	-116
	dc.b	-115
	dc.b	-114
	dc.b	-113
	dc.b	-112
	dc.b	-111
	dc.b	-110
	dc.b	-109
	dc.b	0
	dc.b	0
	dc.b	55
	dc.b	56
	dc.b	57
	dc.b	45
	dc.b	52
	dc.b	53
	dc.b	54
	dc.b	43
	dc.b	49
	dc.b	50
	dc.b	51
	dc.b	48
	dc.b	46
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-108
	dc.b	-107
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-127
	dc.b	-128
	dc.b	-124
	dc.b	-126
	dc.b	-125
	dc.b	-123
	dc.b	-122
	dc.b	-119
	dc.b	-121
	dc.b	-120
	dc.b	47
	dc.b	13
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_us_sc_lock_shift
	cnop	0,4
_g_us_sc_lock_shift
	dc.b	0
	dc.b	27
	dc.b	33
	dc.b	64
	dc.b	35
	dc.b	36
	dc.b	37
	dc.b	94
	dc.b	38
	dc.b	42
	dc.b	40
	dc.b	41
	dc.b	95
	dc.b	43
	dc.b	8
	dc.b	9
	dc.b	113
	dc.b	119
	dc.b	101
	dc.b	114
	dc.b	116
	dc.b	121
	dc.b	117
	dc.b	105
	dc.b	111
	dc.b	112
	dc.b	123
	dc.b	125
	dc.b	10
	dc.b	0
	dc.b	97
	dc.b	115
	dc.b	100
	dc.b	102
	dc.b	103
	dc.b	104
	dc.b	106
	dc.b	107
	dc.b	108
	dc.b	58
	dc.b	34
	dc.b	126
	dc.b	0
	dc.b	124
	dc.b	122
	dc.b	120
	dc.b	99
	dc.b	118
	dc.b	98
	dc.b	110
	dc.b	109
	dc.b	60
	dc.b	62
	dc.b	63
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	32
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-113
	dc.b	-112
	dc.b	-111
	dc.b	-110
	dc.b	-109
	dc.b	0
	dc.b	0
	dc.b	55
	dc.b	56
	dc.b	57
	dc.b	45
	dc.b	52
	dc.b	53
	dc.b	54
	dc.b	43
	dc.b	49
	dc.b	50
	dc.b	51
	dc.b	48
	dc.b	46
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-108
	dc.b	-107
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-127
	dc.b	-128
	dc.b	-124
	dc.b	-126
	dc.b	-125
	dc.b	-123
	dc.b	-122
	dc.b	-119
	dc.b	-121
	dc.b	-120
	dc.b	47
	dc.b	13
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_g_us_sc_ctrl_shift
	cnop	0,4
_g_us_sc_ctrl_shift
	dc.b	0
	dc.b	27
	dc.b	33
	dc.b	64
	dc.b	35
	dc.b	36
	dc.b	37
	dc.b	94
	dc.b	38
	dc.b	42
	dc.b	40
	dc.b	41
	dc.b	95
	dc.b	43
	dc.b	8
	dc.b	9
	dc.b	17
	dc.b	23
	dc.b	5
	dc.b	18
	dc.b	20
	dc.b	25
	dc.b	21
	dc.b	9
	dc.b	15
	dc.b	16
	dc.b	27
	dc.b	29
	dc.b	10
	dc.b	0
	dc.b	1
	dc.b	19
	dc.b	4
	dc.b	6
	dc.b	7
	dc.b	8
	dc.b	10
	dc.b	11
	dc.b	12
	dc.b	59
	dc.b	34
	dc.b	96
	dc.b	0
	dc.b	92
	dc.b	26
	dc.b	24
	dc.b	3
	dc.b	22
	dc.b	2
	dc.b	14
	dc.b	13
	dc.b	44
	dc.b	46
	dc.b	28
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	32
	dc.b	0
	dc.b	-118
	dc.b	-117
	dc.b	-116
	dc.b	-115
	dc.b	-114
	dc.b	-113
	dc.b	-112
	dc.b	-111
	dc.b	-110
	dc.b	-109
	dc.b	0
	dc.b	0
	dc.b	-128
	dc.b	-122
	dc.b	-124
	dc.b	45
	dc.b	-119
	dc.b	53
	dc.b	-120
	dc.b	43
	dc.b	-125
	dc.b	-121
	dc.b	-123
	dc.b	-127
	dc.b	-126
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-108
	dc.b	-107
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	-127
	dc.b	-128
	dc.b	-124
	dc.b	-126
	dc.b	-125
	dc.b	-123
	dc.b	-122
	dc.b	-119
	dc.b	-121
	dc.b	-120
	dc.b	47
	dc.b	13
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	dc.b	0
	public	_rb_word_init
	public	_rb_word_empty
	public	_rb_word_put
	public	_rb_word_get
	public	_g_kbd_control
	section	"BSS",bss
	cnop	0,4
_g_kbd_control
	ds.b	552
