override CFLAGS = +../../vbcc/config/m68k-foenix -I. -I.. -I../include

csources = $(wildcard *.c)
cobjects = $(subst .c,.o,$(csources))

.PHONY: all
all: $(cobjects)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

.PHONY: clean

clean:
	$(RM) $(aobjects) $(cobjects) *.asm
