
UNIT := C256U_PLUS

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
	SRCS_FOR_UNIT=
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

INCLUDES=-I.. -I../include
CFLAGS=$(INCLUDES) $(CFLAGS_FOR_UNIT) -l
ASFLAGS=$(INCLUDES)

SRCS = c256_diskio.c ff.c ffsystem.c ffunicode.c $(SRCS_FOR_UNIT)
OBJS = $(patsubst %.c,%.o,$(SRCS))
OBJS4RM = $(subst /,\\,$(OBJS))

.PHONY: all clean

all: fatfs.a

# Build the devices library file for this model
fatfs.a: $(OBJS)
	$(AR) $@ $^

# Build the object files from C
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

# Clean up after a build
clean:
	$(RM) $(OBJS4RM) fatfs.a *.lst
