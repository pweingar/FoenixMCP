	.public sys_int_enable_all
	.public sys_int_disable_all
	.public sys_int_disable
	.public sys_int_enable
	.public sys_int_register
	.public sys_int_pending
	.public sys_get_info
	.public sys_int_clear
	.public sys_chan_read_b
	.public sys_chan_read
	.public sys_chan_readline
	.public sys_chan_write_b
	.public sys_chan_write
	.public sys_chan_status
	.public sys_chan_flush
	.public sys_chan_seek
	.public sys_chan_ioctrl
	.public sys_chan_open
	.public sys_chan_close
	.public sys_chan_swap
	.public sys_chan_device
	.public sys_bdev_register
	.public sys_bdev_read
	.public sys_bdev_write
	.public sys_bdev_status
	.public sys_bdev_flush
	.public sys_bdev_ioctrl
	.public sys_mem_get_ramtop
	.public sys_mem_reserve
	.public sys_time_jiffies
	.public sys_rtc_set_time
	.public sys_rtc_get_time
	.public sys_kbd_scancode
	.public sys_err_message
	.public sys_kbd_layout
	.public sys_var_set
	.public sys_var_get
	.public sys_txt_get_capabilities
	.public sys_txt_set_mode
	.public sys_txt_setsizes
	.public sys_txt_set_xy
	.public sys_txt_get_xy
	.public sys_txt_get_region
	.public sys_txt_set_region
	.public sys_txt_set_color
	.public sys_txt_get_color
	.public sys_txt_set_cursor_visible
	.public sys_txt_set_font
	.public sys_txt_get_sizes
	.public sys_txt_set_border
	.public sys_txt_set_border_color
	.public sys_txt_put
	.public sys_txt_print

	.extern int_enable_all
	.extern int_disable_all
	.extern int_disable
	.extern int_enable
	.extern int_register
	.extern int_pending
	.extern get_info
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
	.extern time_jiffies
	.extern rtc_set_time
	.extern rtc_get_time
	.extern kbd_scancode
	.extern err_message
	.extern kbd_layout
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

sys_int_enable_all:           	jmp long:int_enable_all
sys_int_disable_all:          	jmp long:int_disable_all
sys_int_disable:              	jmp long:int_disable
sys_int_enable:               	jmp long:int_enable
sys_int_register:             	jmp long:int_register
sys_int_pending:              	jmp long:int_pending
sys_get_info:                 	jmp long:get_info
sys_int_clear:                	jmp long:int_clear
sys_chan_read_b:              	jmp long:chan_read_b
sys_chan_read:                	jmp long:chan_read
sys_chan_readline:            	jmp long:chan_readline
sys_chan_write_b:             	jmp long:chan_write_b
sys_chan_write:               	jmp long:chan_write
sys_chan_status:              	jmp long:chan_status
sys_chan_flush:               	jmp long:chan_flush
sys_chan_seek:                	jmp long:chan_seek
sys_chan_ioctrl:              	jmp long:chan_ioctrl
sys_chan_open:                	jmp long:chan_open
sys_chan_close:               	jmp long:chan_close
sys_chan_swap:                	jmp long:chan_swap
sys_chan_device:              	jmp long:chan_device
sys_bdev_register:            	jmp long:bdev_register
sys_bdev_read:                	jmp long:bdev_read
sys_bdev_write:               	jmp long:bdev_write
sys_bdev_status:              	jmp long:bdev_status
sys_bdev_flush:               	jmp long:bdev_flush
sys_bdev_ioctrl:              	jmp long:bdev_ioctrl
sys_mem_get_ramtop:           	jmp long:mem_get_ramtop
sys_mem_reserve:              	jmp long:mem_reserve
sys_time_jiffies:             	jmp long:time_jiffies
sys_rtc_set_time:             	jmp long:rtc_set_time
sys_rtc_get_time:             	jmp long:rtc_get_time
sys_kbd_scancode:             	jmp long:kbd_scancode
sys_err_message:              	jmp long:err_message
sys_kbd_layout:               	jmp long:kbd_layout
sys_var_set:                  	jmp long:var_set
sys_var_get:                  	jmp long:var_get
sys_txt_get_capabilities:     	jmp long:txt_get_capabilities
sys_txt_set_mode:             	jmp long:txt_set_mode
sys_txt_setsizes:             	jmp long:txt_setsizes
sys_txt_set_xy:               	jmp long:txt_set_xy
sys_txt_get_xy:               	jmp long:txt_get_xy
sys_txt_get_region:           	jmp long:txt_get_region
sys_txt_set_region:           	jmp long:txt_set_region
sys_txt_set_color:            	jmp long:txt_set_color
sys_txt_get_color:            	jmp long:txt_get_color
sys_txt_set_cursor_visible:   	jmp long:txt_set_cursor_visible
sys_txt_set_font:             	jmp long:txt_set_font
sys_txt_get_sizes:            	jmp long:txt_get_sizes
sys_txt_set_border:           	jmp long:txt_set_border
sys_txt_set_border_color:     	jmp long:txt_set_border_color
sys_txt_put:                  	jmp long:txt_put
sys_txt_print:                	jmp long:txt_print
