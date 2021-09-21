# m68k

This folder contains the various source files needed to compile Foenix/MCP
for the Motorola 68000 series processors.

## System Call Convention

System calls will be made through the ``TRAP`` instruction.

* Stack pointer is A7
* Registers D1 - D7, and A0 - A6 will be preserved by the kernel.
* First parameter is in D1, second in D2, and so on.
* The caller places the function number in D0.
* All BIOS calls are issued through a ``TRAP #15`` instruction.
* Results are returned in D0.
