	idnt	"fsys.c"
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	section	"CODE",code
	public	_fatfs_to_foenix
	cnop	0,4
_fatfs_to_foenix
	movem.l	l6,-(a7)
	move.l	(4+l8,a7),d0
	bne	l4
l3
	moveq	#0,d0
	bra	l1
l4
	moveq	#-1,d0
l5
l1
l6	reg
l8	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_open
	cnop	0,4
_fsys_open
	movem.l	l25,-(a7)
	move.w	(10+l27,a7),d5
	move.l	(4+l27,a7),a3
	move.l	#0,a2
	moveq	#-1,d3
	moveq	#0,d2
	bra	l12
l11
	lea	_g_fil_state,a0
	tst.b	(0,a0,d2.w)
	bne	l16
l15
	lea	_g_fil_state,a0
	move.b	#1,(0,a0,d2.w)
	move.w	d2,d3
	bra	l13
l16
l14
	addq.w	#1,d2
l12
	cmp.w	#8,d2
	blt	l11
l13
	tst.w	d3
	bge	l18
l17
	moveq	#-11,d0
	bra	l9
l18
	jsr	_chan_alloc
	move.l	d0,a2
	tst.l	d0
	beq	l20
	move.w	#6,(2,a2)
	move.w	d5,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a3,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	#550,d1
	move.l	d0,d6
	move.l	d1,d7
	swap	d6
	swap	d7
	mulu.w	d1,d6
	mulu.w	d0,d7
	mulu.w	d1,d0
	add.w	d7,d6
	swap	d6
	clr.w	d6
	add.l	d6,d0
	lea	_g_file,a0
	pea	(0,a0,d0.l)
	jsr	_f_open
	move.l	d0,d4
	add.w	#12,a7
	bne	l22
l21
	move.w	d3,d0
	ext.l	d0
	move.l	#255,d1
	move.l	d0,d6
	move.l	d1,d7
	swap	d6
	swap	d7
	mulu.w	d1,d6
	mulu.w	d0,d7
	mulu.w	d1,d0
	add.w	d7,d6
	swap	d6
	clr.w	d6
	add.l	d6,d0
	move.b	d0,(4,a2)
	move.w	d3,d0
	bra	l9
l22
	lea	_g_fil_state,a0
	move.b	#0,(0,a0,d3.w)
	move.l	a2,-(a7)
	jsr	_chan_free
	move.l	d4,-(a7)
	jsr	_fatfs_to_foenix
	addq.w	#8,a7
	bra	l9
l23
	bra	l24
l20
	lea	_g_fil_state,a0
	move.b	#0,(0,a0,d3.w)
	moveq	#-11,d0
l24
l9
l25	reg	a2/a3/d2/d3/d4/d5/d6/d7
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5/d6/d7
l27	equ	32
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_close
	cnop	0,4
_fsys_close
	movem.l	l30,-(a7)
	move.w	(6+l32,a7),d4
	move.l	#0,a2
	moveq	#0,d2
	move.w	d4,d0
	ext.l	d0
	move.l	d0,-(a7)
	jsr	_chan_get_record
	move.l	d0,a2
	moveq	#0,d0
	move.b	(4,a2),d0
	move.w	d0,d2
	move.w	d2,d0
	ext.l	d0
	move.l	#550,d1
	move.l	d0,d5
	move.l	d1,d6
	swap	d5
	swap	d6
	mulu.w	d1,d5
	mulu.w	d0,d6
	mulu.w	d1,d0
	add.w	d6,d5
	swap	d5
	clr.w	d5
	add.l	d5,d0
	lea	_g_file,a0
	pea	(0,a0,d0.l)
	jsr	_f_close
	move.l	d0,d3
	move.l	a2,-(a7)
	jsr	_chan_free
	lea	_g_fil_state,a0
	move.b	#0,(0,a0,d2.w)
	move.l	d3,-(a7)
	jsr	_fatfs_to_foenix
	add.w	#16,a7
l28
l30	reg	a2/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/d2/d3/d4/d5/d6
l32	equ	24
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_opendir
	cnop	0,4
_fsys_opendir
	movem.l	l35,-(a7)
	moveq	#-1,d0
l33
l35	reg
l37	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_closedir
	cnop	0,4
_fsys_closedir
	movem.l	l40,-(a7)
	moveq	#-1,d0
l38
l40	reg
l42	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_readdir
	cnop	0,4
_fsys_readdir
	movem.l	l45,-(a7)
	moveq	#-1,d0
l43
l45	reg
l47	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_findfirst
	cnop	0,4
_fsys_findfirst
	movem.l	l50,-(a7)
	moveq	#-1,d0
l48
l50	reg
l52	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_findnext
	cnop	0,4
_fsys_findnext
	movem.l	l55,-(a7)
	moveq	#-1,d0
l53
l55	reg
l57	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_mkdir
	cnop	0,4
_fsys_mkdir
	movem.l	l60,-(a7)
	moveq	#-1,d0
l58
l60	reg
l62	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_delete
	cnop	0,4
_fsys_delete
	movem.l	l65,-(a7)
	moveq	#-1,d0
l63
l65	reg
l67	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_rename
	cnop	0,4
_fsys_rename
	movem.l	l70,-(a7)
	moveq	#-1,d0
l68
l70	reg
l72	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_setcwd
	cnop	0,4
_fsys_setcwd
	movem.l	l75,-(a7)
	moveq	#-1,d0
l73
l75	reg
l77	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_getcwd
	cnop	0,4
_fsys_getcwd
	movem.l	l80,-(a7)
	moveq	#-1,d0
l78
l80	reg
l82	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_init
	cnop	0,4
_fchan_init
	movem.l	l85,-(a7)
	moveq	#0,d0
l83
l85	reg
l87	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_to_file
	cnop	0,4
_fchan_to_file
	movem.l	l93,-(a7)
	move.l	(4+l95,a7),a1
	moveq	#0,d0
	move.b	(4,a1),d0
	move.w	d0,d1
	cmp.w	#8,d1
	bge	l91
l90
	move.w	d1,d0
	ext.l	d0
	move.l	#550,d2
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
	lea	_g_file,a0
	add.l	d0,a0
	move.l	a0,d0
	bra	l88
l91
	moveq	#0,d0
l92
l88
l93	reg	d2/d3/d4
	movem.l	(a7)+,d2/d3/d4
l95	equ	12
	rts
; stacksize=12
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_read
	cnop	0,4
_fchan_read
	sub.w	#12,a7
	movem.l	l103,-(a7)
	move.w	(26+l105,a7),d3
	move.l	(20+l105,a7),a4
	move.l	(16+l105,a7),a3
	move.l	a3,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l99
	lea	(8+l105,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a4,-(a7)
	move.l	a2,-(a7)
	jsr	_f_read
	move.l	d0,d2
	add.w	#16,a7
	bne	l101
l100
	move.w	(10+l105,a7),d0
	bra	l96
l101
	move.l	d2,-(a7)
	jsr	_fatfs_to_foenix
	addq.w	#4,a7
	bra	l96
l102
l99
	moveq	#-10,d0
l96
l103	reg	a2/a3/a4/d2/d3
	movem.l	(a7)+,a2/a3/a4/d2/d3
l105	equ	20
	add.w	#12,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_readline
	cnop	0,4
_fchan_readline
	movem.l	l113,-(a7)
	move.w	(14+l115,a7),d2
	move.l	(4+l115,a7),a5
	move.l	(8+l115,a7),a3
	move.l	a5,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l109
	move.l	a2,-(a7)
	move.w	d2,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a3,-(a7)
	jsr	_f_gets
	move.l	d0,a4
	add.w	#12,a7
	move.l	a4,d0
	beq	l111
	move.l	a3,a0
	inline
	move.l	a0,d0
.l1
	tst.b	(a0)+
	bne	.l1
	sub.l	a0,d0
	not.l	d0
	einline
	bra	l106
l111
	moveq	#0,d0
	move.b	(17,a2),d0
	move.l	d0,-(a7)
	jsr	_fatfs_to_foenix
	addq.w	#4,a7
	bra	l106
l112
l109
	moveq	#-10,d0
l106
l113	reg	a2/a3/a4/a5/d2
	movem.l	(a7)+,a2/a3/a4/a5/d2
l115	equ	20
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_read_b
	cnop	0,4
_fchan_read_b
	movem.l	l118,-(a7)
	moveq	#0,d0
l116
l118	reg
l120	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_write
	cnop	0,4
_fchan_write
	sub.w	#12,a7
	movem.l	l128,-(a7)
	move.w	(26+l130,a7),d3
	move.l	(20+l130,a7),a4
	move.l	(16+l130,a7),a3
	move.l	a3,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l124
	lea	(8+l130,a7),a0
	move.l	a0,-(a7)
	move.w	d3,d0
	ext.l	d0
	move.l	d0,-(a7)
	move.l	a4,-(a7)
	move.l	a2,-(a7)
	jsr	_f_write
	move.l	d0,d2
	add.w	#16,a7
	bne	l126
l125
	move.w	(10+l130,a7),d0
	bra	l121
l126
	move.l	d2,-(a7)
	jsr	_fatfs_to_foenix
	addq.w	#4,a7
	bra	l121
l127
l124
	moveq	#-10,d0
l121
l128	reg	a2/a3/a4/d2/d3
	movem.l	(a7)+,a2/a3/a4/d2/d3
l130	equ	20
	add.w	#12,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_write_b
	cnop	0,4
_fchan_write_b
	sub.w	#16,a7
	movem.l	l138,-(a7)
	move.b	(27+l140,a7),d3
	move.l	(20+l140,a7),a3
	move.l	a3,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l134
	move.b	d3,(12+l140,a7)
	lea	(8+l140,a7),a0
	move.l	a0,-(a7)
	move.l	#1,-(a7)
	lea	(20+l140,a7),a0
	move.l	a0,-(a7)
	move.l	a2,-(a7)
	jsr	_f_write
	move.l	d0,d2
	add.w	#16,a7
	bne	l136
l135
	move.w	(10+l140,a7),d0
	bra	l131
l136
	move.l	d2,-(a7)
	jsr	_fatfs_to_foenix
	addq.w	#4,a7
	bra	l131
l137
l134
	moveq	#-10,d0
l131
l138	reg	a2/a3/d2/d3
	movem.l	(a7)+,a2/a3/d2/d3
l140	equ	16
	add.w	#16,a7
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_status
	cnop	0,4
_fchan_status
	movem.l	l152,-(a7)
	move.l	(4+l154,a7),a3
	move.l	a3,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l144
	moveq	#0,d2
	move.l	(18,a2),d0
	cmp.l	(12,a2),d0
	beq	l147
l149
	moveq	#0,d0
	bra	l148
l147
	moveq	#1,d0
l148
	tst.l	d0
	beq	l146
	or.w	#1,d2
l146
	tst.b	(17,a2)
	beq	l151
l150
	or.w	#2,d2
l151
	move.w	d2,d0
	bra	l141
l144
	moveq	#-10,d0
l141
l152	reg	a2/a3/d2
	movem.l	(a7)+,a2/a3/d2
l154	equ	12
	rts
; stacksize=32
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_flush
	cnop	0,4
_fchan_flush
	movem.l	l159,-(a7)
	move.l	(4+l161,a7),a3
	move.l	a3,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l158
	move.l	a2,-(a7)
	jsr	_f_sync
	move.l	d0,d2
	move.l	d2,-(a7)
	jsr	_fatfs_to_foenix
	addq.w	#8,a7
	bra	l155
l158
	moveq	#-10,d0
l155
l159	reg	a2/a3/d2
	movem.l	(a7)+,a2/a3/d2
l161	equ	12
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_seek
	cnop	0,4
_fchan_seek
	movem.l	l171,-(a7)
	move.w	(14+l173,a7),d5
	move.l	(8+l173,a7),d4
	move.l	(4+l173,a7),a3
	move.l	a3,-(a7)
	jsr	_fchan_to_file
	move.l	d0,a2
	addq.w	#4,a7
	move.l	a2,d0
	beq	l165
	tst.w	d5
	bne	l167
l166
	move.l	d4,-(a7)
	move.l	a2,-(a7)
	jsr	_f_lseek
	move.l	d0,d2
	move.l	d2,-(a7)
	jsr	_fatfs_to_foenix
	add.w	#12,a7
	bra	l162
l167
	cmp.w	#1,d5
	bne	l170
l169
	move.l	(18,a2),d3
	move.l	d3,d0
	add.l	d4,d0
	move.l	d0,-(a7)
	move.l	a2,-(a7)
	jsr	_f_lseek
	move.l	d0,d2
	move.l	d2,-(a7)
	jsr	_fatfs_to_foenix
	add.w	#12,a7
	bra	l162
l170
l168
l165
	moveq	#-10,d0
l162
l171	reg	a2/a3/d2/d3/d4/d5
	movem.l	(a7)+,a2/a3/d2/d3/d4/d5
l173	equ	24
	rts
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fchan_ioctrl
	cnop	0,4
_fchan_ioctrl
	movem.l	l176,-(a7)
	moveq	#0,d0
l174
l176	reg
l178	equ	0
	rts
; stacksize=0
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_fsys_init
	cnop	0,4
_fsys_init
	movem.l	l190,-(a7)
	moveq	#0,d2
	bra	l182
l181
	lea	_g_dir_state,a0
	move.b	#0,(0,a0,d2.l)
l184
	addq.l	#1,d2
l182
	moveq	#8,d0
	cmp.l	d2,d0
	bgt	l181
l183
	moveq	#0,d2
	bra	l186
l185
	lea	_g_fil_state,a0
	move.b	#0,(0,a0,d2.l)
l188
	addq.l	#1,d2
l186
	moveq	#8,d0
	cmp.l	d2,d0
	bgt	l185
l187
	move.w	#6,_g_file_dev
	move.l	#l189,2+_g_file_dev
	move.l	#_fchan_init,6+_g_file_dev
	move.l	#_fchan_ioctrl,42+_g_file_dev
	move.l	#_fchan_read,10+_g_file_dev
	move.l	#_fchan_read_b,18+_g_file_dev
	move.l	#_fchan_readline,14+_g_file_dev
	move.l	#_fchan_write,22+_g_file_dev
	move.l	#_fchan_write_b,26+_g_file_dev
	move.l	#_fchan_seek,38+_g_file_dev
	move.l	#_fchan_status,30+_g_file_dev
	move.l	#_fchan_flush,34+_g_file_dev
	pea	_g_file_dev
	jsr	_cdev_register
	addq.w	#4,a7
l179
l190	reg	d2
	movem.l	(a7)+,d2
l192	equ	4
	rts
	cnop	0,4
l189
	dc.b	70
	dc.b	73
	dc.b	76
	dc.b	69
	dc.b	0
	public	_f_open
	public	_f_close
	public	_f_read
	public	_f_write
	public	_f_lseek
	public	_f_sync
	public	_f_gets
	public	_cdev_register
	public	_chan_alloc
	public	_chan_free
	public	_chan_get_record
	public	_g_drive
	section	"BSS",bss
	cnop	0,4
_g_drive
	ds.b	4512
	public	_g_dir_state
	cnop	0,4
_g_dir_state
	ds.b	8
	public	_g_directory
	cnop	0,4
_g_directory
	ds.b	384
	public	_g_fil_state
	cnop	0,4
_g_fil_state
	ds.b	8
	public	_g_file
	cnop	0,4
_g_file
	ds.b	4400
	public	_g_file_dev
	cnop	0,4
_g_file_dev
	ds.b	46
