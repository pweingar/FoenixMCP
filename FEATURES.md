# Features

1. [x] Raw text output support to channels A and B
1. [x] ANSI terminal support for channels A and B
1. [x] A2560K "Moe" keyboard driver
1. [x] PS/2 keyboard driver
1. [x] Channel (Stream) driver model
1. [ ] System call library
1. [x] Channel driver for console (raw output and ANSI output)
1. [ ] Channel driver for the serial ports
1. [ ] Channel driver for the parallel port
1. [ ] Channel driver for the MIDI ports
1. [x] Block driver model
1. [x] SDC block driver
1. [x] PATA block driver
1. [ ] Floppy block driver
1. [x] FatFS library integration
1. [ ] Memory management
1. [x] PGX file loader
1. [x] PGZ file loader
1. [ ] ELF file loader
1. [x] Command Line Interface
1. [ ] Mouse driver

## CLI Features

1. [x] Ability to load a file
1. [ ] Auto-run/configuration file
1. Built-in commands:
    1. [x] DIR [path]
    1. [ ] COPY [path] TO [path]
    1. [ ] RENAME [path] TO [path]
    1. [x] DELETE [path]
    1. [x] CD [path]
    1. [x] PWD
    1. [ ] FORMAT [drive] -- Format a drive... should this be built in?
    1. [ ] PRINT [expression]
    1. [x] POKE [address], [value] -- value to an address
    1. [x] PEEK [address] -- value from an address
    1. [ ] CALL [address] -- Start running assembly code
    1. [x] DUMP [address], [size]
    1. [ ] SETCOLOR [lut], [index], [r], [g], [b] -- set a color LUT value
    1. [x] SETTIME
    1. [x] GETTIME
    1. [ ] GRAPHICS
    1. [ ] BORDER
    1. [ ] BACKGROUND
