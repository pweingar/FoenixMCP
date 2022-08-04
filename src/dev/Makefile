csources = $(wildcard *.c)
csources_unported = block.c channel.c console.c fsys.c pata.c ps2.c rtc.c sdc.c uart.c fdc.c lpt.c midi.c
csources := $(filter csources_unported,$(csources))

cobjects = $(subst .c,.o,$(csources))

cobjects = $(subst .c,.o,$(csources))

.PHONY: all
all: $(cobjects)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

.PHONY: clean

clean:
	$(RM) *.o *.asm
