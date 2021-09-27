# Sample: HelloPGX #

One of the simple executable formats that the FoenixMCP supports is the PGX format inherited from the original Foenix stock kernel. This assembly demo uses the VASM assembler to create a simple PGX file that prints "Hello, world."

## Building ##

Make sure that the VASM binaries are in your executable path and then use the `build.bat` file to create the PGX file.

## PGX Format ##

PGX is a very simple binary file format. It is designed to load a single segment of code or data to a single block of memory. A PGX file begins with a small header (eight bytes), and the remainder of the file is the data to load:

* The first three bytes are the format signature: the ASCII characters "PGX".
* The fourth byte is the version and target CPU code. Currently, only format 0 is used. There are two CPU codes supported: 1 = 65816, 2 = 68000.
* The next four bytes are the target address for loading the data (i.e. the address of the first byte of the data). The address is to be written in the 32-bit integer format native for the target processor. For the 65816, the address will be in little-endian format. For the 68000 processors, the address will be in big-endian format.
