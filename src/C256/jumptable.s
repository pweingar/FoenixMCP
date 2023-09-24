	.public sjt_int_enable_all
	.public sjt_int_disable_all
	.public sjt_int_disable
	.public sjt_int_enable
	.public sjt_int_register
	.public sjt_int_pending
	.public sjt_int_clear
	.public sjt_chan_read_b
	.public sjt_chan_read
	.public sjt_chan_readline
	.public sjt_chan_write_b
	.public sjt_chan_write
	.public sjt_chan_status
	.public sjt_chan_flush
	.public sjt_chan_seek
	.public sjt_chan_ioctrl
	.public sjt_chan_open
	.public sjt_chan_close
	.public sjt_chan_swap
	.public sjt_chan_device
	.public sjt_bdev_register
	.public sjt_bdev_read
	.public sjt_bdev_write
	.public sjt_bdev_status
	.public sjt_bdev_flush
	.public sjt_bdev_ioctrl
	.public sjt_mem_get_ramtop
	.public sjt_mem_reserve
	.public sjt_err_message
	.public sjt_var_set
	.public sjt_var_get
	.public sjt_txt_get_capabilities
	.public sjt_txt_set_mode
	.public sjt_txt_setsizes
	.public sjt_txt_set_xy
	.public sjt_txt_get_xy
	.public sjt_txt_get_region
	.public sjt_txt_set_region
	.public sjt_txt_set_color
	.public sjt_txt_get_color
	.public sjt_txt_set_cursor_visible
	.public sjt_txt_set_font
	.public sjt_txt_get_sizes
	.public sjt_txt_set_border
	.public sjt_txt_set_border_color
	.public sjt_txt_put
	.public sjt_txt_print

	.extern int_enable_all
	.extern int_disable_all
	.extern int_disable
	.extern int_enable
	.extern int_register
	.extern int_pending
	.extern int_clear
	.extern chan_read_b
	.extern chan_read
	.extern chan_readline
	.extern chan_write_b
	.extern chan_write
	.extern chan_status
	.extern chan_flush
	.extern chan_seek
	.extern chan_ioctrl
	.extern chan_open
	.extern chan_close
	.extern chan_swap
	.extern chan_device
	.extern bdev_register
	.extern bdev_read
	.extern bdev_write
	.extern bdev_status
	.extern bdev_flush
	.extern bdev_ioctrl
	.extern mem_get_ramtop
	.extern mem_reserve
	.extern err_message
	.extern var_set
	.extern var_get
	.extern txt_get_capabilities
	.extern txt_set_mode
	.extern txt_setsizes
	.extern txt_set_xy
	.extern txt_get_xy
	.extern txt_get_region
	.extern txt_set_region
	.extern txt_set_color
	.extern txt_get_color
	.extern txt_set_cursor_visible
	.extern txt_set_font
	.extern txt_get_sizes
	.extern txt_set_border
	.extern txt_set_border_color
	.extern txt_put
	.extern txt_print

	.section jumptable

sjt_int_enable_all:           	jmp long:int_enable_all
sjt_int_disable_all:          	jmp long:int_disable_all
sjt_int_disable:              	jmp long:int_disable
sjt_int_enable:               	jmp long:int_enable
sjt_int_register:             	jmp long:int_register
sjt_int_pending:              	jmp long:int_pending
sjt_int_clear:                	jmp long:int_clear
sjt_chan_read_b:              	jmp long:chan_read_b
sjt_chan_read:                	jmp long:chan_read
sjt_chan_readline:            	jmp long:chan_readline
sjt_chan_write_b:             	jmp long:chan_write_b
sjt_chan_write:               	jmp long:chan_write
sjt_chan_status:              	jmp long:chan_status
sjt_chan_flush:               	jmp long:chan_flush
sjt_chan_seek:                	jmp long:chan_seek
sjt_chan_ioctrl:              	jmp long:chan_ioctrl
sjt_chan_open:                	jmp long:chan_open
sjt_chan_close:               	jmp long:chan_close
sjt_chan_swap:                	jmp long:chan_swap
sjt_chan_device:              	jmp long:chan_device
sjt_bdev_register:            	jmp long:bdev_register
sjt_bdev_read:                	jmp long:bdev_read
sjt_bdev_write:               	jmp long:bdev_write
sjt_bdev_status:              	jmp long:bdev_status
sjt_bdev_flush:               	jmp long:bdev_flush
sjt_bdev_ioctrl:              	jmp long:bdev_ioctrl
sjt_mem_get_ramtop:           	jmp long:mem_get_ramtop
sjt_mem_reserve:              	jmp long:mem_reserve
sjt_err_message:              	jmp long:err_message
sjt_var_set:                  	jmp long:var_set
sjt_var_get:                  	jmp long:var_get
sjt_txt_get_capabilities:     	jmp long:txt_get_capabilities
sjt_txt_set_mode:             	jmp long:txt_set_mode
sjt_txt_setsizes:             	jmp long:txt_setsizes
sjt_txt_set_xy:               	jmp long:txt_set_xy
sjt_txt_get_xy:               	jmp long:txt_get_xy
sjt_txt_get_region:           	jmp long:txt_get_region
sjt_txt_set_region:           	jmp long:txt_set_region
sjt_txt_set_color:            	jmp long:txt_set_color
sjt_txt_get_color:            	jmp long:txt_get_color
sjt_txt_set_cursor_visible:   	jmp long:txt_set_cursor_visible
sjt_txt_set_font:             	jmp long:txt_set_font
sjt_txt_get_sizes:            	jmp long:txt_get_sizes
sjt_txt_set_border:           	jmp long:txt_set_border
sjt_txt_set_border_color:     	jmp long:txt_set_border_color
sjt_txt_put:                  	jmp long:txt_put
sjt_txt_print:                	jmp long:txt_print
