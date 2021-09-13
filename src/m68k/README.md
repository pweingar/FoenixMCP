# m68k

This folder contains the various source files needed to compile Foenix/MCP
for the Motorola 68000 series processors.

## System Call Convention

System calls will be made through the ``TRAP`` instruction.
The caller will be expected to push parameters onto the stack using the
VBCC ABI, and then issuing a ``TRAP`` instruction.

* Stack pointer is A7
* All arguments are passed on the stack
* Registers D0, D1, A0, and A1 will be used as scratch.
* Registers D2 - D7, and A2 - A6 will be preserved by the kernel.
* Results are returned in D0.

## BIOS

All BIOS calls are issued through a ``TRAP #13`` instruction. Register D0 contains the number of the system call to make.

