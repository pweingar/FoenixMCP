							.public sys_txt_get_capabilities
							.public sys_txt_set_mode
							.public sys_txt_setsizes
							.public sys_txt_set_resolution
							.public sys_txt_set_border
							.public sys_txt_set_border_color
							.public sys_txt_set_font
							.public sys_txt_set_cursor
							.public sys_txt_set_cursor_visible
							.public sys_txt_get_region
							.public sys_txt_set_region
							.public sys_txt_set_color
							.public sys_txt_get_color
							.public sys_txt_set_xy
							.public sys_txt_get_xy
							.public sys_txt_put
							.public sys_txt_put_raw
							.public sys_txt_print
							.public sys_txt_scroll
							.public sys_txt_fill
							.public sys_txt_clear
							.public sys_txt_clear_line
							.public sys_txt_insert
							.public sys_txt_delete
							.public sys_txt_get_sizes
							
							.extern sjt_txt_get_capabilities
							.extern sjt_txt_set_mode
							.extern sjt_txt_setsizes
							.extern sjt_txt_set_resolution
							.extern sjt_txt_set_border
							.extern sjt_txt_set_border_color
							.extern sjt_txt_set_font
							.extern sjt_txt_set_cursor
							.extern sjt_txt_set_cursor_visible
							.extern sjt_txt_get_region
							.extern sjt_txt_set_region
							.extern sjt_txt_set_color
							.extern sjt_txt_get_color
							.extern sjt_txt_set_xy
							.extern sjt_txt_get_xy
							.extern sjt_txt_put
							.extern sjt_txt_put_raw
							.extern sjt_txt_print
							.extern sjt_txt_scroll
							.extern sjt_txt_fill
							.extern sjt_txt_clear
							.extern sjt_txt_clear_line
							.extern sjt_txt_insert
							.extern sjt_txt_delete
							.extern sjt_txt_get_sizes

							.section farcode

sys_txt_get_capabilities:	jmp long:sjt_txt_get_capabilities
sys_txt_set_mode:			jmp long:sjt_txt_set_mode
sys_txt_setsizes:			jmp long:sjt_txt_setsizes
sys_txt_set_resolution:		jmp long:sjt_txt_set_resolution
sys_txt_set_border:			jmp long:sjt_txt_set_border
sys_txt_set_border_color:	jmp long:sjt_txt_set_border_color
sys_txt_set_font:			jmp long:sjt_txt_set_font
sys_txt_set_cursor:			jmp long:sjt_txt_set_cursor
sys_txt_set_cursor_visible:	jmp long:sjt_txt_set_cursor_visible
sys_txt_get_region:			jmp long:sjt_txt_get_region
sys_txt_set_region:			jmp long:sjt_txt_set_region
sys_txt_set_color:			jmp long:sjt_txt_set_color
sys_txt_get_color:			jmp long:sjt_txt_get_color
sys_txt_set_xy:				jmp long:sjt_txt_set_xy
sys_txt_get_xy:				jmp long:sjt_txt_get_xy
sys_txt_put:				jmp long:sjt_txt_put
sys_txt_put_raw:			jmp long:sjt_txt_put_raw
sys_txt_print:				jmp long:sjt_txt_print
sys_txt_scroll:				jmp long:sjt_txt_scroll
sys_txt_fill:				jmp long:sjt_txt_fill
sys_txt_clear:				jmp long:sjt_txt_clear
sys_txt_clear_line:			jmp long:sjt_txt_clear_line
sys_txt_insert:				jmp long:sjt_txt_insert
sys_txt_delete:				jmp long:sjt_txt_delete
sys_txt_get_sizes:			jmp long:sjt_txt_get_sizes