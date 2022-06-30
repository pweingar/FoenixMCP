# Utility: srec_remap

A simple Python3 script to move blocks of data in a Motorola SREC HEX file.

## Usage ##

```
python srec_remap.py {mapping file} {source srec file}
```

The new SREC file with the data moved to the new addresses will be output to the `STDOUT`.

## Mapping Files ##

A mapping file is a simple text file. A line in the text file can be one of two things:
1. A comment, which begins with a `#`. All comment lines are ignored.
2. A mapping definition, which consists of three addresses written in hexidecimal, and separated by commas. The first address is the starting address of the first byte in a block of memory in the source SREC file that needs to be moved. The second is the address of the last byte in the block to move. The third address is the starting address the first byte of the block should have in the new SREC file.

## Operation ##

The script will read the provided mapping files to figure out where the blocks of data need to be moved. Then, it will open and start to read the provided SREC file. Each line in the SREC file will then be checked against the mappings provided:

* If the address of the record falls within original starting and ending addresses of a block defined in the mapping file, the address will be changed to be in the same relative location in the new block of memory. The checksum for the line will be recalculated, and the line will be send to `STDOUT`.

* If the address of the record does not fall within any of the blocks of memory to be moved, the line will be emitted as-is.

* Finally, if the line is of type 0, the line will be emitted as-is.

## Example ##

Given the mapping file:

```
# Example mapping (move data in bank 1 to bank 2, and bank 2 to bank 3):
010000,01FFFF,020000
020000,02FFFF,030000
```

And the SREC file (the checksums are dummy values):

```
S0050000414243FF
S2080000005152535455FF
S2080100006162636465FF
S2080100087172737475FF
S2080201008182838485FF
```

The resulting SREC file will look like (again, the checksums here are dummy values):

```
S0050000414243FF
S2080000005152535455FF
S2080200006162636465FF
S2080200087172737475FF
S2080301008182838485FF
```
