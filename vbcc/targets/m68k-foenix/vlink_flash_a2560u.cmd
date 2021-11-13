FLASHSTART = 0x00E10000;
FLASHLEN = 0x00200000;
RAMSTART = 0x00001000;
RAMSIZE  = 0x00010000;
STACKLEN = 0x400;
VECTORSIZE = 0x400;
BINFILESTART = 0x00000000;
PAGESIZE = 0x10000;

MEMORY
{
  vectors : org = 0x000000, len = VECTORSIZE
  ram : org = RAMSTART + VECTORSIZE, len = RAMSIZE - VECTORSIZE
  flash: org = FLASHSTART, len = FLASHLEN
  binpage0: org = BINFILESTART, len = PAGESIZE
  binpages: org = BINFILESTART + PAGESIZE, len = FLASHLEN - PAGESIZE
}

SECTIONS
{
  vectors : { *(VECTORS) } >vectors AT>binpage0
  text : {*(CODE)} >flash AT>binpages
  .dtors : { *(.dtors) } >ram AT>binpage0
  .ctors : { *(.ctors) } >ram AT>binpage0
  rodata : {*(RODATA)} >flash AT>binpages
  data: {*(DATA)} >ram
  bss (NOLOAD): {*(BSS)} >ram

  ___heap = ADDR(bss) + SIZEOF(bss);
  ___heapend = RAMSTART + RAMSIZE - STACKLEN;

  ___BSSSTART = ADDR(bss);
  ___BSSSIZE  = SIZEOF(bss);

  ___STACK = RAMSTART + RAMSIZE;
}
