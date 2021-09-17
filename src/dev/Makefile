override CFLAGS = +../../vbcc/config/m68k-foenix -I. -I.. -I../include

csources = $(wildcard *.c)
cobjects = $(subst .c,.o,$(csources))

.PHONY: all
all: $(cobjects)

%.o: %.c
	$(CC) -k -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	$(RM) $(aobjects) $(cobjects)
