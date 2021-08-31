AS=vasmm68k_mot
CC=vc
CFLAGS=+../vbcc/config/m68k-foenix -I. -Iinclude -k
DEPS=syscalls_m68k.h

foenixmcp.s68: foenixmcp.o bios_m68k.o syscalls_m68k.o startup_m68k.o
	$(CC) $(CFLAGS) -o foenixmcp.s68 foenixmcp.o bios_m68k.o syscalls_m68k.o

startup_m68k.o: startup_m68k.s
	$(AS) -Fvobj -o $@ $<

%.o: %.c $(DEPS)
	$(CC) -S -c -o $@ $< $(CFLAGS)

PHONEY: clean

clean:
	del *.o
