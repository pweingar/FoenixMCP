	.public sys_proc_exit
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
	.public sys_cdev_register
	.public sys_bdev_register
	.public sys_bdev_read
	.public sys_bdev_write
	.public sys_bdev_status
	.public sys_bdev_flush
	.public sys_bdev_ioctrl
	.public sys_fsys_open
	.public sys_fsys_close
	.public sys_fsys_opendir
	.public sys_fsys_closedir
	.public sys_fsys_readdir
	.public sys_fsys_findfirst
	.public sys_fsys_findnext
	.public sys_fsys_get_label
	.public sys_fsys_set_label
	.public sys_fsys_mkdir
	.public sys_fsys_delete
	.public sys_fsys_rename
	.public sys_fsys_set_cwd
	.public sys_fsys_get_cwd
	.public sys_fsys_load
	.public sys_fsys_register_loader
	.public sys_fsys_stat
	.public sys_mem_get_ramtop
	.public sys_mem_reserve
	.public sys_time_jiffies
	.public sys_rtc_set_time
	.public sys_rtc_get_time
	.public sys_kbd_scancode
	.public sys_err_message
	.public sys_kbd_layout
	.public sys_proc_run
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

	.extern proc_exit
	.extern int_enable_all
	.extern int_disable_all
	.extern int_disable
	.extern int_enable
	.extern int_register
	.extern int_pending
	.extern sys_get_information
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
	.extern cdev_register
	.extern bdev_register
	.extern bdev_read
	.extern bdev_write
	.extern bdev_status
	.extern bdev_flush
	.extern bdev_ioctrl
	.extern fsys_open
	.extern fsys_close
	.extern fsys_opendir
	.extern fsys_closedir
	.extern fsys_readdir
	.extern fsys_findfirst
	.extern fsys_findnext
	.extern fsys_getlabel
	.extern fsys_setlabel
	.extern fsys_mkdir
	.extern fsys_delete
	.extern fsys_rename
	.extern fsys_set_cwd
	.extern fsys_get_cwd
	.extern fsys_load
	.extern fsys_register_loader
	.extern fsys_stat
	.extern mem_get_ramtop
	.extern mem_reserve
	.extern timers_jiffies
	.extern rtc_set_time
	.extern rtc_get_time
	.extern kbd_get_scancode
	.extern err_message
	.extern kbd_layout
	.extern proc_run
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

sys_proc_exit:                	jmp long:proc_exit
sys_int_enable_all:           	jmp long:int_enable_all
sys_int_disable_all:          	jmp long:int_disable_all
sys_int_disable:              	jmp long:int_disable
sys_int_enable:               	jmp long:int_enable
sys_int_register:             	jmp long:int_register
sys_int_pending:              	jmp long:int_pending
sys_get_info:                 	jmp long:sys_get_information
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
sys_cdev_register:            	jmp long:cdev_register
sys_bdev_register:            	jmp long:bdev_register
sys_bdev_read:                	jmp long:bdev_read
sys_bdev_write:               	jmp long:bdev_write
sys_bdev_status:              	jmp long:bdev_status
sys_bdev_flush:               	jmp long:bdev_flush
sys_bdev_ioctrl:              	jmp long:bdev_ioctrl
sys_fsys_open:                	jmp long:fsys_open
sys_fsys_close:               	jmp long:fsys_close
sys_fsys_opendir:             	jmp long:fsys_opendir
sys_fsys_closedir:            	jmp long:fsys_closedir
sys_fsys_readdir:             	jmp long:fsys_readdir
sys_fsys_findfirst:           	jmp long:fsys_findfirst
sys_fsys_findnext:            	jmp long:fsys_findnext
sys_fsys_get_label:           	jmp long:fsys_getlabel
sys_fsys_set_label:           	jmp long:fsys_setlabel
sys_fsys_mkdir:               	jmp long:fsys_mkdir
sys_fsys_delete:              	jmp long:fsys_delete
sys_fsys_rename:              	jmp long:fsys_rename
sys_fsys_set_cwd:             	jmp long:fsys_set_cwd
sys_fsys_get_cwd:             	jmp long:fsys_get_cwd
sys_fsys_load:                	jmp long:fsys_load
sys_fsys_register_loader:     	jmp long:fsys_register_loader
sys_fsys_stat:                	jmp long:fsys_stat
sys_mem_get_ramtop:           	jmp long:mem_get_ramtop
sys_mem_reserve:              	jmp long:mem_reserve
sys_time_jiffies:             	jmp long:timers_jiffies
sys_rtc_set_time:             	jmp long:rtc_set_time
sys_rtc_get_time:             	jmp long:rtc_get_time
sys_kbd_scancode:             	jmp long:kbd_get_scancode
sys_err_message:              	jmp long:err_message
sys_kbd_layout:               	jmp long:kbd_layout
sys_proc_run:                 	jmp long:proc_run
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
