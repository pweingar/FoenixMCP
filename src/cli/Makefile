
UNIT := C256U_PLUS

# Define OS-dependent variables

ifeq ($(OS),Windows_NT)
	RM = del /F/Q
else
	RM = rm -f
endif

# Define model-specific variables, including tools, source files, compiler flags, etc.

ifeq ($(UNIT),C256U)
	CC=cc65816
	AS=as65816
	AR=nlib

	SRCS_FOR_UNIT=
	CFLAGS_FOR_UNIT=-DMODEL=1 -DCPU=255 --target Foenix --code-model large --data-model large
else ifeq ($(UNIT),C256U_PLUS)
	CC=cc65816
	AS=as65816
	AR=nlib

	SRCS_FOR_UNIT=
	CFLAGS_FOR_UNIT=-DMODEL=5 -DCPU=255 --target Foenix --code-model large --data-model large
else ifeq ($(UNIT),C256_FMX)
	CC=cc65816
	AS=as65816
	AR=nlib

	SRCS_FOR_UNIT=
	CFLAGS_FOR_UNIT=-DMODEL=0 -DCPU=255 --target Foenix --code-model large --data-model large
endif

INCLUDES=-I.. -I../include
CFLAGS=$(INCLUDES) $(CFLAGS_FOR_UNIT) -l
ASFLAGS=$(INCLUDES)

SRCS = cli.c dos_cmds.c dos_copy.c mem_cmds.c settings.c sound_cmds.c test_cmd2.c $(SRCS_FOR_UNIT)
OBJS = $(patsubst %.c,%.o,$(SRCS))
OBJS4RM = $(subst /,\\,$(OBJS))

.PHONY: all clean

all: cli.a

# Build the CLI library file for this model
cli.a: $(OBJS)
	$(AR) $@ $^

# Build the object files from C
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

# Clean up after a build
clean:
	$(RM) $(OBJS4RM) cli.a *.lst
