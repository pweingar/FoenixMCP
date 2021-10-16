
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
export MODEL=13

export VER_MAJOR = 0
export VER_MINOR = 0
export VER_BUILD = 1

export AS = vasmm68k_mot
export ASFLAGS = -quiet -Fvobj -nowarn=62
export CC = vc
export DEFINES = -DCPU=$(CPU) -DMODEL=$(MODEL) -DVER_MAJOR=$(VER_MAJOR) -DVER_MINOR=$(VER_MINOR) -DVER_BUILD=$(VER_BUILD) # -DKBD_POLLED
export CFLAGS = +../vbcc/config/m68k-foenix -I. -Iinclude
export RM = cmd /C del /Q /F

cpu = m68k
cpu_assembly_src := $(wildcard $(cpu)/*.s)
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

.PHONY: all $(cpu) dev fatfs snd cli

all: foenixmcp.s68 $(cpu) dev snd cli

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
