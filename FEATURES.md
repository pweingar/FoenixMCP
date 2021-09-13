# Features

1. Raw text output support to channels A and B
1. ANSI terminal support for channels A and B
1. A2560K "Moe" keyboard driver
1. PS/2 keyboard driver
1. Channel (Stream) driver model
1. System call library
1. Channel driver for console (raw output and ANSI output)
1. Channel driver for the serial ports
1. Channel driver for the parallel port
1. Channel driver for the MIDI ports
1. Block driver model
1. SDC block driver
1. PATA block driver
1. Floppy block driver
1. FatFS library integration
1. Memory management
1. PGX file loader
1. PGZ file loader
1. ELF file loader
1. Command Line Interface
1. Mouse driver

## CLI Features

1. Ability to load a file
1. Auto-run/configuration file
1. Built-in commands: 
    1. DIR [path]
    1. COPY [path] TO [path]
    1. RENAME [path] TO [path]
    1. DELETE [path]
    1. CD [path]
    1. PWD
    1. FORMAT [drive] -- Format a drive... should this be built in?
    1. PRINT [expression]
    1. POKE [address], [value] -- value to an address
    1. PEEK [address] -- value from an address
    1. CALL [address] -- Start running assembly code
    1. DUMP [address], [size]
    1. SETCOLOR [lut], [index], [r], [g], [b] -- set a color LUT value
    1. TIME
    1. DATE
    1. GRAPHICS
    1. BORDER
    1. BACKGROUND
