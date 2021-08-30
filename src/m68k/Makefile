AS=vasmm68k_mot
CC=vc
CFLAGS=+../../vbcc/config/m68k-foenix -I. -I../include
DEPS=syscalls_m68k.h

startup.o: startup.s
	$(AS) -o $@ $<

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
