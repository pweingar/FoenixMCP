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

### Building for the M68000 CPU

Building this project requires the [VBCC](http://www.compilers.de/vbcc.html) compiler and the
```MAKE``` utility. Using your command line, go into the ```src``` directory and type:

```
make all
```
to build the kernel.

To remove binaries and intermediate files:

```
make clean
```

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
