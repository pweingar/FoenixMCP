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

.PHONY: all $(cpu) dev

all: foenixmcp.s68 $(cpu) dev

$(cpu):
	$(MAKE) --directory=$@

dev:
	$(MAKE) --directory=dev

foenixmcp.s68: foenixmcp.o log.o $(cpu) dev
	$(CC) $(CFLAGS) -o foenixmcp.s68 foenixmcp.o log.o $(cpu_c_obj) $(dev_c_obj)

%.o: %.c $(DEPS)
	$(CC) -S -c -o $@ $< $(CFLAGS)

.PHONEY: clean

clean:
	$(RM) *.s68 *.o
	$(MAKE) --directory=$(cpu) clean
	$(MAKE) --directory=dev clean
