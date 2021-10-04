# Sample: HelloPGZ #

One of the simple executable formats that the FoenixMCP supports is the PGZ format inherited from the original Foenix stock kernel. This assembly demo uses the VASM assembler to create a simple Motorola SREC file that prints "Hello, world." This SREC file can be converted to PGZ using the SRECPGZ C program included in the utilities folder of this repository.

## Building ##

Make sure that the VASM binaries are in your executable path and then use the `build.bat` file to create the SREC file.
