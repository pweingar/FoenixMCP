asources = startup_m68040.s fdc_m68040.s
aobjects = $(subst .s,.o,$(asources))
csources = bios_m68040.c
cobjects = $(subst .c,.o,$(csources))

.PHONY: all
all: $(aobjects) $(cobjects)

%.o: $.s
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

.PHONY: clean

clean:
	$(RM) $(aobjects) $(cobjects)
