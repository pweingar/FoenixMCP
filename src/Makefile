AS=vasmm68k_mot
CC=vc
CFLAGS=+../vbcc/config/m68k-foenix -I. -Iinclude
DEPS=syscalls_m68k.h

startup_m68k.o: startup_m68k.s
	$(AS) -Fvobj -o $@ $<

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

foenixmcp: foenixmcp.o bios_m68k.o syscalls_m68k.o startup_m68k.o
	$(CC) $(CFLAGS) -o foenixmcp foenixmcp.o bios_m68k.o syscalls_m68k.o

PHONEY: clean

clean:
	rm *.o
