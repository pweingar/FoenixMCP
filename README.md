# Foenix/MCP

A portable kernel for the Foenix series of computers.

## Overview

The purpose of this project is to provide a simple kernel for the Foenix series
of retro-style computers. Emphasis is placed upon the word "simple." This
operating system will be more related to MS-DOS or CP/M than to Unix or Linux.
The main purposes of this kernel are:

* Boot the Foenix computer to a reasonably usable state.
* Provide access to files on IDE hard drives, SD cards, and floppy disk (when
  the computer supports floppies).
* Provide an environment for loading and running binary programs from media.
* Provide access to those devices which are tedious to access at the lowest
  level (_e.g._ storage media, keyboard, text mode screen).
* Provide a common programmatic interface for all the computers in the line.
* Be broadly portable across the different processors the Foenix computers
  support (at the time of this writing, this includes the 65816, and the 68000
  series).

### Foenix Computers Supported

[Flesh this out as we get there. Intention is to support the FMX, U, U+, A2560K,
A2560X, A2560U (68000 and 65816), and GenX.]

## Building

Please refer to processor specific building instructions that will be included.
For the 65816 and 68000 computers, the project will be built using the VBCC
compiler.

### Building for the M68000 CPU Family

Building this project requires the [VBCC](http://www.compilers.de/vbcc.html) compiler and the
```MAKE``` utility. Using your command line, go into the ```src``` directory and type:

```
export VBCC = <use-your-projects-dir>/FoenixMCP/vbcc
make all
```
to build the kernel.

To remove binaries and intermediate files:

```
make clean
```

#### Build Options

There are three option variables that can be set in the makefile:

* `UNIT`: This specifies which Foenix model is the targetted machine for the build. There are currently two options:
    1. `UNIT=a2560u`, this covers both the A2560U and A2560U+. It sets the CPU to be the M68000.
    2. `UNIT=a2560k`, this covers the A2560K and sets the CPU to M68040.
* `MEMORY`: This species where in memory Foenix/MCP will run. There are two values here:
    1. `MEMORY=ram`: This specifies that the MCP will run from RAM.
    2. `MEMORY=flash`: This specifies that the MCP will be programmed to flash memory.
* `KBD_POLLED`: As a temporary feature, if this option variable is defined, it will specify to the kernel that polled I/O is to be used for keyboard access instead of interrupt driven I/O.

Examples:
```
make all UNIT=a2560u MEMORY=ram
```
Builds an SREC file suitable for loading into RAM of the A2560U/U+.

```
make all UNIT=a2560k MEMORY=flash
```
Builds a binary file suitable for programming the flash of the A2560K.

## License

Most of the source code for this kernel is made available under the open source
BSD 3-Clause License (see LICENSE). However, the kernel also includes portions
of other projects, which are covered under their licenses. In particular, the
kernel currently uses the FatFS embedded file system, which is covered under
separate license. Please see the [src/fatfs](src/fatfs) directory for license
details.

## Features

See the [features list](FEATURES.md) for the features intended for this kernel
and the status of implementation.


# //END-OF-LINE
