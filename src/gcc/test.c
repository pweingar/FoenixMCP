#include <stdio.h>

#include "foenix.h"

void main(void)
{
    short w = sys_bdev_status(0);
    printf("%d", w);
/*
    short rs;
    long rl;
    long s = 0x12345678;

rs = sys_exit(1);
sys_warmboot();
rl= sys_int_register(1,s);
sys_int_enable(1);
sys_int_disable(1);
sys_int_enable_all();
sys_int_disable_all();
sys_int_clear(1);
sys_int_pending(1);
sys_get_information(s);

rs = sys_chan_read(1,s,1);
rs = sys_chan_read_b(1);
rs = sys_chan_read_line(1,s,1);
rs = sys_chan_write_b(1,'c');
rs = sys_chan_flush(1);
rs = sys_chan_seek(1,s,1);
rs = sys_chan_status(1);
rs = sys_chan_ioctrl(1,1,s,1);
rs = sys_chan_register(s);
rs = sys_chan_open(s,1);
rs = sys_chan_close(1);

rs = sys_bdev_register(s);
rs = sys_bdev_read(1,s,s,1);
rs = sys_bdev_write(1,s,s,1);
rs = sys_bdev_status(1);
rs = sys_bdev_flush(1);
rs = sys_bdev_ioctrl(1,1,s,1);

rs = sys_fsys_open(s,1);
rs = sys_fsys_close(1);
rs = sys_fsys_opendir(s);
rs = sys_fsys_closedir(1);
rs = sys_fsys_readdir(1,s);
rs = sys_fsys_findfirst(s,s,s);
rs = sys_fsys_findnext(1,s);
rs = sys_fsys_delete(s);
rs = sys_fsys_rename(s,s);
rs = sys_fsys_mkdir(s);
rs = sys_fsys_load(s,s,s);
rs = sys_fsys_setcwd(s);
rs = sys_fsys_getcwd(s,1);
rs = sys_fsys_get_label(s,s);
rs = sys_fsys_set_label(1,s);
rs = fsys_register_loader(s,s);

rs = sys_proc_run(s,1,s);

rl = sys_rtc_get_jiffies();
sys_rtc_set_time(s);
rl = sys_rtc_get_time();
rs = sys_kbd_scancode();
rl = sys_err_message(1);
*/
}