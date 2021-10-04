# SRECPGZ

This project is a simple C program to convert Motorola SREC hex files to Foenix PGZ executable binaries.

## Usage:

The program takes one option and two required path parameters:
1. The `--large` switch if present generates a 32-bit PGZ file. If not, the old 24-bit PGZ format will be used.
1. The first path is the input SREC file
2. The second path is the output binary file.

```
srecpgz [--large] <input srec file> <output bin file>
```

## PGZ Format

The output format will depend on whether or not the `--large` option was used. By default, 24-bit address and count fields are used.
If the `--large` option is enabled, 32-bit address and count fields are used.

* The first byte is the header or signature byte. For 24-bit PGZ, an uppercase 'Z' is used. For 32-bit PGZ, a lowercase 'z' is used.
* After the signature can come any number of blocks. Each block has two mandatory fields, and an optional data field:
    1. The first field is the address of the block in memory. The address is specified in 24-bit or 32-bit, little endian format.
    1. The second field is the size of the data block. The size is specified in 24-bit 32-bit, little endian format.
    1. If the size (_n_) is non-zero, immediately after the size field are _n_ bytes of data. These are the data bytes to be loaded into that address block of memory.
* Each block immediately follows the one before it. So if a block has a size of zero, the next block starts immediately after the last size byte. If the block has a non-zero size, the next block starts immediately after the data field.
* A block with a size of 0 and no data field specifies the starting address for the executable (the address field specifies the starting address). At least one starting address block must be contained in the PGZ file for it to be executable. If more than one starting address block is present, the last starting address block is taken to be the correct starting address.
