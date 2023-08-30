							.public sjt_txt_get_capabilities
							.public sjt_txt_set_mode
							.public sjt_txt_setsizes
							.public sjt_txt_set_resolution
							.public sjt_txt_set_border
							.public sjt_txt_set_border_color
							.public sjt_txt_set_font
							.public sjt_txt_set_cursor
							.public sjt_txt_set_cursor_visible
							.public sjt_txt_get_region
							.public sjt_txt_set_region
							.public sjt_txt_set_color
							.public sjt_txt_get_color
							.public sjt_txt_set_xy
							.public sjt_txt_get_xy
							.public sjt_txt_put
							.public sjt_txt_put_raw
							.public sjt_txt_print
							.public sjt_txt_scroll
							.public sjt_txt_fill
							.public sjt_txt_clear
							.public sjt_txt_clear_line
							.public sjt_txt_insert
							.public sjt_txt_delete
							.public sjt_txt_get_sizes
							
							.extern txt_get_capabilities
							.extern txt_set_mode
							.extern txt_setsizes
							.extern txt_set_resolution
							.extern txt_set_border
							.extern txt_set_border_color
							.extern txt_set_font
							.extern txt_set_cursor
							.extern txt_set_cursor_visible
							.extern txt_get_region
							.extern txt_set_region
							.extern txt_set_color
							.extern txt_get_color
							.extern txt_set_xy
							.extern txt_get_xy
							.extern txt_put
							.extern text_put_raw
							.extern txt_print
							.extern txt_scroll
							.extern txt_fill
							.extern txt_clear
							.extern txt_clear_line
							.extern txt_insert
							.extern txt_delete
							.extern txt_get_sizes


							.section jumptable

sjt_txt_get_capabilities:	jmp long:txt_get_capabilities
sjt_txt_set_mode:			jmp long:txt_set_mode
sjt_txt_setsizes:			jmp long:txt_setsizes
sjt_txt_set_resolution:		jmp long:txt_set_resolution
sjt_txt_set_border:			jmp long:txt_set_border
sjt_txt_set_border_color:	jmp long:txt_set_border_color
sjt_txt_set_font:			jmp long:txt_set_font
sjt_txt_set_cursor:			jmp long:txt_set_cursor
sjt_txt_set_cursor_visible:	jmp long:txt_set_cursor_visible
sjt_txt_get_region:			jmp long:txt_get_region
sjt_txt_set_region:			jmp long:txt_set_region
sjt_txt_set_color:			jmp long:txt_set_color
sjt_txt_get_color:			jmp long:txt_get_color
sjt_txt_set_xy:				jmp long:txt_set_xy
sjt_txt_get_xy:				jmp long:txt_get_xy
sjt_txt_put:				jmp long:txt_put
sjt_txt_put_raw:			jmp long:text_put_raw
sjt_txt_print:				jmp long:txt_print
sjt_txt_scroll:				jmp long:txt_scroll
sjt_txt_fill:				jmp long:txt_fill
sjt_txt_clear:				jmp long:txt_clear
sjt_txt_clear_line:			jmp long:txt_clear_line
sjt_txt_insert:				jmp long:txt_insert
sjt_txt_delete:				jmp long:txt_delete
sjt_txt_get_sizes:			jmp long:txt_get_sizes