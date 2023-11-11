
#
# User may over-ride the UNIT and MEMORY parameters to specify target machine
# and where the MCP will run (ram or flash)
#
UNIT := a2560x
MEMORY := flash

# CPU_WDC65816                0x16  /* CPU code for the Western Design Center 65816 */
# CPU_M68000                  0x20  /* CPU code for the Motorola 68000 */
# CPU_M68010                  0x21  /* CPU code for the Motorola 68010 */
# CPU_M68020                  0x22  /* CPU code for the Motorola 68020 */
# CPU_M68030                  0x23  /* CPU code for the Motorola 68030 */
# CPU_M68040                  0x24  /* CPU code for the Motorola 68040 */
# CPU_I486DX                  0x34  /* CPU code for the Intel 486DX */

# MODEL_FOENIX_FMX            0
# MODEL_FOENIX_C256U          1
# MODEL_FOENIX_GENX           4
# MODEL_FOENIX_C256U_PLUS     5
# MODEL_FOENIX_A2560U_PLUS    6
# MODEL_FOENIX_A2560X         8
# MODEL_FOENIX_A2560U         9
# MODEL_FOENIX_A2560K         11

# Determine target CPU and MODEL based on the UNIT
# The comments are on a separate line to avoid inserting white space in the commande line where these variables are used

ifeq ($(UNIT),a2560k)
# M68040V
export CPU_NUMBER = 6
# VASM CPU flag
export VASM_CPU = -m68040
# VBCC CPU flag
export VBCC_CPU = 68040
# A2560K
export MODEL_NUMBER = 11
export cpu = m68040

else ifeq ($(UNIT),genx)
# M68040V
export CPU_NUMBER = 6
# VASM CPU flag
export VASM_CPU = -m68040
# VBCC CPU flag
export VBCC_CPU = 68040
# GenX
export MODEL_NUMBER = 4
export cpu = m68040

else ifeq ($(UNIT),a2560x)
# M68040V
export CPU_NUMBER = 6
# VASM CPU flag
export VASM_CPU = -m68040
# VBCC CPU flag
export VBCC_CPU = 68040
# A2560X
export MODEL_NUMBER = 8
export cpu = m68040

else ifeq ($(UNIT),a2560u)
# M680SEC00
export CPU_NUMBER = 0
# VASM CPU flag
export VASM_CPU = -m68000
# VBCC CPU flag
export VBCC_CPU = 68000
export MODEL_NUMBER = 9
export cpu = m68k
endif

# Determine the correct configuration file (barring OS)

ifeq ($(MEMORY),ram)
export CFG_FILE = $(VBCC)/config/$(UNIT)_ram
else
export CFG_FILE = $(VBCC)/config/$(UNIT)_flash
endif

export AS = vasmm68k_mot
export ASFLAGS = $(VASM_CPU) -quiet -Fvobj -nowarn=62 -DMODEL=$(MODEL_NUMBER) 
export CC = vc
DEFINES = -DCPU=$(CPU_NUMBER) -DMODEL=$(MODEL_NUMBER) # -D__NOINLINE__ # -DKBD_POLLED 

# Maximum working logging level to compile
#define LOG_FATAL   0   /* Log a critical or fatal event */
#define LOG_ERROR   1   /* Log an error */
#define LOG_INFO    2   /* Log miscellaneous information */
#define LOG_DEBUG   3   /* Log a debugging message */
#define LOG_TRACE   4   /* Log tracing information (like entry into a subroutine) */
#define LOG_VERBOSE 5   /* Log a truly verbose message... the sort you almost never want to bother with */
DEFAULT_LOG_LEVEL=0
#define LOG_CHANNEL_UART0 -1
#define LOG_CHANNEL_CHANNEL_A 0
#define LOG_CHANNEL_CHANNEL_B 1
#define LOG_CHANNEL_CHANNEL_A_LOW_LEVEL 10
LOG_CHANNEL=0
ifeq ("$(DEFAULT_LOG_LEVEL)","")
else
 DEFINES := $(DEFINES) -DDEFAULT_LOG_LEVEL=$(DEFAULT_LOG_LEVEL)
endif
ifeq ("$(LOG_CHANNEL)","")
else
 DEFINES := $(DEFINES) -DLOG_CHANNEL=$(LOG_CHANNEL)
endif


export DEFINES:=$(DEFINES)

# When compiling an image for flash, size to which the image must be padded
PAD_FLASH_SIZE=524288

ifeq ($(OS),Windows_NT)
	export CFLAGS = -cpu=$(VBCC_CPU) +$(CFG_FILE) -I. -I$(CURDIR)/include -I$(CURDIR)
	export RM = cmd /C del /Q /F
	PAD_CMD = fsutil seteof $@ $(PAD_FLASH_SIZE)
else
	export CFLAGS = -cpu=$(VBCC_CPU) +$(CFG_FILE)_linux -I. -I$(CURDIR)/include -I$(CURDIR)
	export RM = rm -f
	PAD_CMD = truncate -s $(PAD_FLASH_SIZE) $@
endif

cpu_assembly_src := $(wildcard $(cpu)/*.s)
cpu_c_src := $(wildcard $(cpu)/*.c)
cpu_assembly_obj := $(subst .s,.o,$(cpu_assembly_src))
cpu_c_obj := $(subst .c,.o,$(cpu_c_src))

dev_base_sources = dev/block.c dev/channel.c dev/console.c dev/fsys.c dev/pata.c dev/ps2.c dev/rtc.c dev/sdc.c dev/txt_screen.c dev/uart.c
ifeq ($(UNIT),a2560k)
dev_c_src := $(dev_base_sources) dev/fdc.c dev/kbd_mo.c dev/lpt.c dev/midi.c dev/txt_a2560k_a.o dev/txt_a2560k_b.o m68040/fdc_m68040.o
else ifeq ($(UNIT),genx)
dev_c_src := $(dev_base_sources) dev/fdc.c dev/lpt.c dev/midi.c dev/txt_a2560k_a.o dev/txt_a2560k_b.o m68040/fdc_m68040.o
else ifeq ($(UNIT),a2560x)
dev_c_src := $(dev_base_sources) dev/fdc.c dev/lpt.c dev/midi.c dev/txt_a2560k_a.o dev/txt_a2560k_b.o m68040/fdc_m68040.o
else ifeq ($(UNIT),a2560u)
dev_c_src := $(dev_base_sources) dev/txt_a2560u.o
else
dev_c_src := $(dev_base_sources)
endif
dev_c_obj := $(subst .c,.o,$(dev_c_src))

snd_c_src := $(wildcard snd/*.c)
snd_c_obj := $(subst .c,.o,$(snd_c_src))
fat_c_src := $(wildcard fatfs/*.c)
fat_c_obj := $(subst .c,.o,$(fat_c_src))
cli_c_src := $(wildcard cli/*.c)
cli_c_obj := $(subst .c,.o,$(cli_c_src))
c_src :=  $(wildcard *.c)
c_obj := $(subst .c,.o,$(c_src))

.PHONY: all $(cpu) dev fatfs snd cli

ifeq ($(MEMORY),ram)
all: foenixmcp.s68 $(cpu) dev snd cli
else
all: foenixmcp.bin $(cpu) dev snd cli
endif

$(cpu):
	$(MAKE) --directory=$@

dev:
	$(MAKE) --directory=dev

fatfs:
	$(MAKE) --directory=fatfs

snd:
	$(MAKE) --directory=snd

cli:
	$(MAKE) --directory=cli

foenixmcp.s68: $(c_obj) $(cpu) dev fatfs snd cli
	$(CC) $(CFLAGS) $(DEFINES) -o foenixmcp.s68 $(c_obj) $(cpu_c_obj) $(dev_c_obj) $(fat_c_obj) $(snd_c_obj) $(cli_c_obj)

foenixmcp.bin: $(c_obj) $(cpu) dev fatfs snd cli
	$(CC) $(CFLAGS) $(DEFINES) -o foenixmcp.bin $(c_obj) $(cpu_c_obj) $(dev_c_obj) $(fat_c_obj) $(snd_c_obj) $(cli_c_obj)
	$(PAD_CMD)

%.o: %.c $(DEPS)
	$(CC) -S -c -o $@ $< $(CFLAGS) $(DEFINES)

.PHONEY: clean

clean:
	$(RM) *.s68 *.o *.asm
	$(MAKE) --directory=$(cpu) clean
	$(MAKE) --directory=dev clean
	$(MAKE) --directory=fatfs clean
	$(MAKE) --directory=snd clean
	$(MAKE) --directory=cli clean
