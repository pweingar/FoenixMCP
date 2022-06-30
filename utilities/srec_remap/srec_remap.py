
#
# Remap the addresses of an SREC file
#
# The user can define address ranges that should be remapped
#
# For instance: $E00000 to $EFFFFF -> $000000 to $0FFFFF...
#
# * If a line in an SREC file has an address of $E10000, it
#   would be moved to $010000.
# * If a line has an address of $0F0000, however, it would
#   be left alone
#

import sys
import re

mapping_pattern = re.compile("^([0-9a-fA-F]+)\,([0-9a-fA-F]+)\,([0-9a-fA-F]+)")
srec_pattern = re.compile("^S(\d)([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F]*)([0-9a-fA-F][0-9a-fA-F])")

memory_mappings = []

class SRecLine:
    "A line from an SREC file."

    def __init__(self, line):
        "Initialize an SREC from a line in the file."
        self.type = -1
        self.count = 0
        self.address = 0
        self.data = ""
        self.checksum = 0

        matches = srec_pattern.search(line)
        if matches:
            self.type = int(matches.group(1), 10)
            self.count = int(matches.group(2), 16)
            address_data = matches.group(3)
            self.checksum = int(matches.group(4), 16)

            if self.type in [3, 7]:
                "32-bit addresses"
                self.address = int(address_data[0:7],16)
                self.data = address_data[8:]

            elif self.type in [2, 6, 8]:
                "24-bit addresses"
                self.address = int(address_data[0:5], 16)
                self.data = address_data[6:]

            else:
                "16-bit addresses"
                self.address = int(address_data[0:3], 16)
                self.data = address_data[4:]

    def is_ok(self):
        "Return True if we have read actual data"
        return self.type > -1

    def in_range(self, address_range):
        "Return true if the line is within the range of addresses provided."
        return (self.address in address_range)

    def compute_checksum(self):
        "Recalculate the checksum for the line"
        chk = self.count & 0xff

        address = self.address
        while address > 0:
            chk = chk + (address & 0xff)
            address = (0x00ffffff00 & address) >> 8

        data = self.data
        while len(data) > 0:
            byte_str = data[0:2]
            data = data[2:]
            chk = chk + int(byte_str, 16)

        self.checksum = (~chk & 0xff)

    def remap(self, address_range, address_new):
        "Change the address of the line to match the new starting point of the address range."
        if self.type != 0:
            self.address = self.address - (address_range[0] - address_new)
            self.compute_checksum()

    def to_string(self):
        "Format the line as a string."
        if self.type in [3, 7]:
            "32-bit addresses"
            return "S{0:1d}{1:02X}{2:08X}{3}{4:02X}".format(self.type, self.count, self.address, self.data, self.checksum)
        elif self.type in [2, 6, 8]:
            "24-bit addresses"
            return "S{0:1d}{1:02X}{2:06X}{3}{4:02X}".format(self.type, self.count, self.address, self.data, self.checksum)
        else:
            "16-bit addresses"
            return "S{0:1d}{1:02X}{2:04X}{3}{4:02X}".format(self.type, self.count, self.address, self.data, self.checksum)

class Mapping:
    "Class to track address ranges that need to be mapped to new locations."
    def __init__(self, orig_start, orig_end, new_start):
        self.orig_start = orig_start
        self.orig_end = orig_end
        self.new_start = new_start

    def orig_range(self):
        "Return the original range"
        return range(self.orig_start, self.orig_end)

    def new_start(self):
        "Return the new starting address"
        return self.new_start

if __name__ == "__main__":

    if len(sys.argv) != 3:
        print("Usage: python srec_remap.py <range mapping file> <SREC file>", file=sys.stderr);
        print("       New SREC printed to STDOUT\n", file=sys.stderr);
        exit(1)

    # First: read all the address range mappings
    with open(sys.argv[1], "r") as ranges_in:
        while True:
            line = ranges_in.readline()
            if not line:
                break

            mapping_matches = mapping_pattern.search(line)
            if mapping_matches:
                orig_start = int(mapping_matches.group(1), 16)
                orig_end = int(mapping_matches.group(2), 16)
                new_start = int(mapping_matches.group(3), 16)

                new_mapping = Mapping(orig_start, orig_end, new_start)
                memory_mappings.append(new_mapping)

    # Next: read through the SREC file and process each line
    with open(sys.argv[2], "r") as srec_in:
        while True:
            line = srec_in.readline()
            if not line:
                break

            record = SRecLine(line)
            if record.is_ok():
                for mapping in memory_mappings:
                    if record.in_range(mapping.orig_range()):
                        record.remap(mapping.orig_range(), mapping.new_start)
                        record.compute_checksum()
                        break

                print(record.to_string())
