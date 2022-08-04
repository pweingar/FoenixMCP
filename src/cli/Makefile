csources = $(wildcard *.c)
csources_unported := cli.c dis68k.c dos_cmds.c dos_copy.c mem_cmds.c settings.c sound_cmds.c test_cmd2.c
csources := $(filter-out $(csources_unported),$(csources))
cobjects = $(subst .c,.o,$(csources))

.PHONY: all
all: $(cobjects)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES) -I../include -I..

.PHONY: clean

clean:
	$(RM) $(aobjects) $(cobjects) *.asm
