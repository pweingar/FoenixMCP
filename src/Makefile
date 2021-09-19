export AS = vasmm68k_mot
export ASFLAGS = -quiet -Fvobj -nowarn=62
export CC = vc
export CFLAGS = +../vbcc/config/m68k-foenix -I. -Iinclude -DCPU=32 -DSYSTEM=4
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

foenixmcp.s68: foenixmcp.o log.o ring_buffer.o interrupt.o syscalls.o $(cpu) dev fatfs snd cli
	$(CC) $(CFLAGS) -o foenixmcp.s68 foenixmcp.o log.o ring_buffer.o interrupt.o syscalls.o $(cpu_c_obj) $(dev_c_obj) $(fat_c_obj) $(snd_c_obj) $(cli_c_obj)

%.o: %.c $(DEPS)
	$(CC) -S -c -o $@ $< $(CFLAGS)

.PHONEY: clean

clean:
	$(RM) *.s68 *.o *.asm
	$(MAKE) --directory=$(cpu) clean
	$(MAKE) --directory=dev clean
	$(MAKE) --directory=fatfs clean
	$(MAKE) --directory=snd clean
	$(MAKE) --directory=cli clean
