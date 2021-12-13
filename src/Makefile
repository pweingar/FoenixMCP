# Makefile for FoenixMCP

.PHONY: help
help:
	@echo Target          Description
	@echo --------------  ---------------------------
	@echo foenixmcp.bin   ROM image to flash using e.g. C256Mgr
	@echo foenixmcp.s68   S-record that can be uploaded to memory for testing purposes
	@echo all             Builds all of the above
	@echo clean           Removes all compilation products and by-products


# CPU_WDC65816                0x16  /* CPU code for the Western Design Center 65816 */
# CPU_M68000                  0x20  /* CPU code for the Motorola 68000 */
# CPU_M68010                  0x21  /* CPU code for the Motorola 68010 */
# CPU_M68020                  0x22  /* CPU code for the Motorola 68020 */
# CPU_M68030                  0x23  /* CPU code for the Motorola 68030 */
# CPU_M68040                  0x24  /* CPU code for the Motorola 68040 */
# CPU_I486DX                  0x34  /* CPU code for the Intel 486DX */
export CPU=32

# MODEL_FOENIX_FMX            0
# MODEL_FOENIX_C256U          1
# MODEL_FOENIX_GENX           4
# MODEL_FOENIX_C256U_PLUS     5
# MODEL_FOENIX_A2560U_PLUS    6
# MODEL_FOENIX_A2560X         8
# MODEL_FOENIX_A2560U         9
# MODEL_FOENIX_A2560K         13
export MODEL=9
ifeq ($(MODEL),9)
	export MACHINE_TYPE=MACHINE_TYPE_A2560U
	export CPU=68000
else ifeq ($(MODEL),13)
	export MACHINE_TYPE=MACHINE_TYPE_A2560K
	export CPU=68040
else
	# TODO
endif

# OS tools
ifeq ($(OS),Windows_NT)
	export RM = cmd /C del /Q /F
else
	export RM = rm -f
endif

# VBCC suite commands
# The VBCC environment variable is supposed to be set ! See VBCC documentation.
export AS = vasmm68k_mot
export ASFLAGS = -quiet -Fvobj -nowarn=62
export CC = vc

# Compilation flags, defines, optimization etc.
DEFINES = -DCPU=$(CPU) -DMODEL=$(MODEL) -D$(MACHINE_TYPE) # -DKBD_POLLED
CFLAG_OPTFLAGS=-O0
CFLAGS_COMMON=-I. -I$(CURDIR)/include -I$(CURDIR) -cpu=$(CPU) $(CFLAG_OPTFLAGS)

# Adapt configuration file to use according to OS
# (configs flavors have backward or forward slashes in them)
ifneq ($(OS),Windows_NT)
	VBCC_CONFIG_SUFFIX=-linux
endif
export CFLAGS = +$(VBCC)/config/$(VBCC_CONFIG)$(VBCC_CONFIG_SUFFIX) $(CFLAGS_COMMON) $(DEFINES)

# Prepare list of files making up FoenixMCP
cpu = m68k
cpu_assembly_src :=
cpu_c_src := $(wildcard $(cpu)/*.c)
cpu_assembly_obj := $(subst .s,.o,$(cpu_assembly_src))
cpu_c_obj := $(subst .c,.o,$(cpu_c_src))
dev_c_src := $(wildcard dev/*.c)
dev_c_obj := $(subst .c,.o,$(dev_c_src))
snd_c_src := $(wildcard snd/*.c)
snd_c_obj := $(subst .c,.o,$(snd_c_src))
fat_c_src := $(wildcard fatfs/*.c)
fat_c_obj := $(subst .c,.o,$(fat_c_src))
cli_c_src := $(wildcard cli/*.c)
cli_c_obj := $(subst .c,.o,$(cli_c_src))
c_src :=  $(wildcard *.c)
c_obj := $(subst .c,.o,$(c_src))
OBJS=$(c_obj) $(cpu_assembly_obj) $(cpu_c_obj) $(dev_c_obj) $(fat_c_obj) $(snd_c_obj) $(cli_c_obj)


.PHONY: all clean $(cpu) dev fatfs snd cli
all: foenixmcp.s68 foenixmcp.bin $(cpu) dev snd cli

# Don't display "Entering/leaving directory x".
MAKEFLAGS = --no-print-directory

# Build subdirectories
SUBDIRS := $(cpu) dev fatfs snd cli
.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all_foenixmcp_objects
all_foenixmcp_objects: $(c_obj) $(cpu) dev fatfs snd cli

foenixmcp.bin: VBCC_CONFIG=a2560u_flash
foenixmcp.bin: export CFLAGS=+$(VBCC)/config/$(VBCC_CONFIG)$(VBCC_CONFIG_SUFFIX) $(CFLAGS_COMMON) $(DEFINES)
foenixmcp.bin: all_foenixmcp_objects
	$(CC) $(CFLAGS) -o $@ $(OBJS)

foenixmcp.s68: VBCC_CONFIG=m68k-foenix
foenixmcp.s68: export CFLAGS=+$(VBCC)/config/$(VBCC_CONFIG)$(VBCC_CONFIG_SUFFIX) $(CFLAGS_COMMON) $(DEFINES)
foenixmcp.s68: all_foenixmcp_objects
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# VBCC reciepe for C files
%.o: %.c $(DEPS)
	$(CC) -S -c -o $@ $< $(CFLAGS)

clean:
	$(RM) foenixmcp.s68 foenixmcp.bin *.o *.asm
	$(MAKE) --directory=$(cpu) clean
	$(MAKE) --directory=dev clean
	$(MAKE) --directory=fatfs clean
	$(MAKE) --directory=snd clean
	$(MAKE) --directory=cli clean
