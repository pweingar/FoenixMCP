## FAT32 Volume Boot Record Format

Windows 10 checks for the following (https://github.com/microsoft/Windows-driver-samples/blob/16022c593787aef040b8a37ef697316e07f1322e/filesys/fastfat/fsctrl.c#L2546):

* [offset=0x000, length=3] First byte must be: 0xE9, 0xEB, 0x49
* [offset=0x00B, length=2] BytesPerSector must be: 128, 256, 512, 1024, 2048, or 4096
* [offset=0x00D, length=1] SectorsPerCluster must be: 1, 2, 4, 8, 16, 32, 64, or 128
* [offset=0x00E, length=2] ReservedSectors must be > 0
* [offset=0x010, length=1] NumberOfFATs must be > 0
* Either [offset=0x013, length=2] Sectors or [offset=0x020, length=4] LargeSectors must be > 0
* [offset=0x016, length=2] SectorsPerFat > 0 or ([offset=0x024, length=4] LargeSectorsPerFAT > 0 and [offset=0x02A, length=2] FsVersion = 0)
* [offset=0x015, length=1] MediaType must be: 0xF0, 0xF8, 0xF9, 0xFB, 0xFD, 0xFE, 0xFF
* Either [offset=0x016, length=2] SectorPerFAT = 0 or RootEntries > 0
* Either [offset=0x016, length=2] SectorPerFAT > 0 or MirrorDisabled

Important Fields:
* 0x000 - 0x002 : Boot jump code {0xEB 0xFE 0x00}
* 0x003 - 0x00A : OEM Name
* 0x00B - 0x05A : BPB and Extended BPB
* 0x05B         : CPU #
* 0x05C - 0x05D : Adjustment spot for checksum (?)
* 0x05E - 0x05F : Unused
* 0x060 - 0x1FC : Boot sector code area
* 0x1FD         : Physical drive number (?)
* 0x1FE - 0x1FF : Signature: 0x55 0xAA

For Foenix/MCP: 16-bit big-endian checksum of entire sector must equal 0x2560

Checksum: `sum = ((sum & 1) << 15) + (sum >> 1) + *w++;`

### Boot Process from Floppy and SDC

1. Load VBR into memory at 0x00000400
2. Compute checksum and verify it comes to 0x2560
3. Check that 0x05B has the right CPU ID
4. Call to 0x00000460 as a subroutine

## Master Boot Record Format

* 0x000 - 0x1BD : Boot code area (approximate end point)
* 0x1BE - 0x1FD : Partition data
* 0x1FE - 0x1FF : Signature: 0x55 0xAA

For Foenix/MCP: 16-bit big-endian checksum of entire sector must equal 0x2560

### Boot Process for PATA drive

1. Load MBR into memory at 0x00000400
2. Compute checksum and verify it comes to 0x2560
3. Check that 0x05B has the right CPU ID
4. Call to 0x00000400 as a subroutine
