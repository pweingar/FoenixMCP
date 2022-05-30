csources = $(wildcard *.c)
cobjects = $(subst .c,.o,$(csources))

cobjects = $(subst .c,.o,$(csources))

.PHONY: all
all: $(cobjects)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

.PHONY: clean

clean:
	$(RM) *.o *.asm
