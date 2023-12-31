
UNIT := C256U_PLUS
MEMORY := RAM

# Define OS-dependent variables

ifeq ($(OS),Windows_NT)
	RM = del /F/Q
else
	RM = rm -f
endif

# Define model-specific variables, including tools, source files, compiler flags, etc.

ifeq ($(UNIT),C256U)
	CPU=w65816
	SRCS_FOR_UNIT=
	CFLAGS_FOR_UNIT=-DMODEL=1 -DCPU=255 --target Foenix --code-model large --data-model large
	LDFLAGS_FOR_UNIT=C256/ld_lc_c256_u.scm clib-lc-ld.a
else ifeq ($(UNIT),C256U_PLUS)
	CPU=w65816
	SRCS_FOR_UNIT=C256/jumptable.s C256/syscalls.s C256/syscalls_c256.c C256/io_stubs.c
	CFLAGS_FOR_UNIT=-DMODEL=5 -DCPU=255 --target Foenix --code-model large --data-model large
	LDFLAGS_FOR_UNIT=C256/ld_lc_c256_fmx.scm clib-lc-ld.a --rtattr printf=medium
else ifeq ($(UNIT),C256_FMX)
	CPU=w65816
	SRCS_FOR_UNIT=
	CFLAGS_FOR_UNIT=-DMODEL=0 -DCPU=255 --target Foenix --code-model large --data-model large
	LDFLAGS_FOR_UNIT=C256/ld_lc_c256_fmx.scm clib-lc-ld.a 
endif

ifeq ($(CPU),w65816)
	CC=cc65816
	AS=as65816
	LD=ln65816
	AR=nlib
endif

INCLUDES=-I. -I./include
CFLAGS=$(INCLUDES) $(CFLAGS_FOR_UNIT) -l
ASFLAGS=$(INCLUDES)
LDFLAGS=--target foenix --output-format s37 $(LDFLAGS_FOR_UNIT) --list-file foenixmcp.map

SRCS = foenixmcp.c log.c memory.c ring_buffer.c simpleio.c sys_general.c variables.c utilities.c $(SRCS_FOR_UNIT)
OBJS = $(patsubst %.s,%.o,$(patsubst %.c,%.o,$(SRCS)))
OBJS4RM = $(subst /,\\,$(OBJS))
LIBS = dev/devices.a snd/sound.a fatfs/fatfs.a

.PHONY: clean

foenixmcp.s37: $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $^

dev/devices.a:
	$(MAKE) --directory=dev

snd/sound.a:
	$(MAKE) --directory=snd

fatfs/fatfs.a:
	$(MAKE) --directory=fatfs

# Build the object files from C
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

# Build the object files from assembly
%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $^

# Clean up after a build
clean:
	$(RM) $(OBJS4RM) foenixmcp.s37 *.lst *.map
	$(MAKE) --directory=dev clean
	$(MAKE) --directory=snd clean
	$(MAKE) --directory=fatfs clean