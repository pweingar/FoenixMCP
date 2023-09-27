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

	.extern sjt_int_enable_all
	.extern sjt_int_disable_all
	.extern sjt_int_disable
	.extern sjt_int_enable
	.extern sjt_int_register
	.extern sjt_int_pending
	.extern sjt_get_info
	.extern sjt_int_clear
	.extern sjt_chan_read_b
	.extern sjt_chan_read
	.extern sjt_chan_readline
	.extern sjt_chan_write_b
	.extern sjt_chan_write
	.extern sjt_chan_status
	.extern sjt_chan_flush
	.extern sjt_chan_seek
	.extern sjt_chan_ioctrl
	.extern sjt_chan_open
	.extern sjt_chan_close
	.extern sjt_chan_swap
	.extern sjt_chan_device
	.extern sjt_bdev_register
	.extern sjt_bdev_read
	.extern sjt_bdev_write
	.extern sjt_bdev_status
	.extern sjt_bdev_flush
	.extern sjt_bdev_ioctrl
	.extern sjt_mem_get_ramtop
	.extern sjt_mem_reserve
	.extern sjt_time_jiffies
	.extern sjt_rtc_set_time
	.extern sjt_rtc_get_time
	.extern sjt_kbd_scancode
	.extern sjt_err_message
	.extern sjt_kbd_layout
	.extern sjt_var_set
	.extern sjt_var_get
	.extern sjt_txt_get_capabilities
	.extern sjt_txt_set_mode
	.extern sjt_txt_setsizes
	.extern sjt_txt_set_xy
	.extern sjt_txt_get_xy
	.extern sjt_txt_get_region
	.extern sjt_txt_set_region
	.extern sjt_txt_set_color
	.extern sjt_txt_get_color
	.extern sjt_txt_set_cursor_visible
	.extern sjt_txt_set_font
	.extern sjt_txt_get_sizes
	.extern sjt_txt_set_border
	.extern sjt_txt_set_border_color
	.extern sjt_txt_put
	.extern sjt_txt_print

	.section farcode

sys_int_enable_all:           	jmp long:sjt_int_enable_all
sys_int_disable_all:          	jmp long:sjt_int_disable_all
sys_int_disable:              	jmp long:sjt_int_disable
sys_int_enable:               	jmp long:sjt_int_enable
sys_int_register:             	jmp long:sjt_int_register
sys_int_pending:              	jmp long:sjt_int_pending
sys_get_info:                 	jmp long:sjt_get_info
sys_int_clear:                	jmp long:sjt_int_clear
sys_chan_read_b:              	jmp long:sjt_chan_read_b
sys_chan_read:                	jmp long:sjt_chan_read
sys_chan_readline:            	jmp long:sjt_chan_readline
sys_chan_write_b:             	jmp long:sjt_chan_write_b
sys_chan_write:               	jmp long:sjt_chan_write
sys_chan_status:              	jmp long:sjt_chan_status
sys_chan_flush:               	jmp long:sjt_chan_flush
sys_chan_seek:                	jmp long:sjt_chan_seek
sys_chan_ioctrl:              	jmp long:sjt_chan_ioctrl
sys_chan_open:                	jmp long:sjt_chan_open
sys_chan_close:               	jmp long:sjt_chan_close
sys_chan_swap:                	jmp long:sjt_chan_swap
sys_chan_device:              	jmp long:sjt_chan_device
sys_bdev_register:            	jmp long:sjt_bdev_register
sys_bdev_read:                	jmp long:sjt_bdev_read
sys_bdev_write:               	jmp long:sjt_bdev_write
sys_bdev_status:              	jmp long:sjt_bdev_status
sys_bdev_flush:               	jmp long:sjt_bdev_flush
sys_bdev_ioctrl:              	jmp long:sjt_bdev_ioctrl
sys_mem_get_ramtop:           	jmp long:sjt_mem_get_ramtop
sys_mem_reserve:              	jmp long:sjt_mem_reserve
sys_time_jiffies:             	jmp long:sjt_time_jiffies
sys_rtc_set_time:             	jmp long:sjt_rtc_set_time
sys_rtc_get_time:             	jmp long:sjt_rtc_get_time
sys_kbd_scancode:             	jmp long:sjt_kbd_scancode
sys_err_message:              	jmp long:sjt_err_message
sys_kbd_layout:               	jmp long:sjt_kbd_layout
sys_var_set:                  	jmp long:sjt_var_set
sys_var_get:                  	jmp long:sjt_var_get
sys_txt_get_capabilities:     	jmp long:sjt_txt_get_capabilities
sys_txt_set_mode:             	jmp long:sjt_txt_set_mode
sys_txt_setsizes:             	jmp long:sjt_txt_setsizes
sys_txt_set_xy:               	jmp long:sjt_txt_set_xy
sys_txt_get_xy:               	jmp long:sjt_txt_get_xy
sys_txt_get_region:           	jmp long:sjt_txt_get_region
sys_txt_set_region:           	jmp long:sjt_txt_set_region
sys_txt_set_color:            	jmp long:sjt_txt_set_color
sys_txt_get_color:            	jmp long:sjt_txt_get_color
sys_txt_set_cursor_visible:   	jmp long:sjt_txt_set_cursor_visible
sys_txt_set_font:             	jmp long:sjt_txt_set_font
sys_txt_get_sizes:            	jmp long:sjt_txt_get_sizes
sys_txt_set_border:           	jmp long:sjt_txt_set_border
sys_txt_set_border_color:     	jmp long:sjt_txt_set_border_color
sys_txt_put:                  	jmp long:sjt_txt_put
sys_txt_print:                	jmp long:sjt_txt_print
