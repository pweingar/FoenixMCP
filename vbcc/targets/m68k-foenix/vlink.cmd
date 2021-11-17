RAMSTART = 0x00010000;
RAMSIZE  = 0x00030000;
STACKLEN = 0x400;
HEAPSTART = 0x00100000;
HEAPSIZE = 0x00020000;

MEMORY
{
  vec : org = 0, len = 0x400
  ram : org = RAMSTART, len = RAMSIZE - STACKLEN
}

SECTIONS
{
  vectors : { *(VECTORS) } >vec
  text ALIGN(0x02) : { *(CODE) } >ram
  .dtors ALIGN(0x02) : { *(.dtors) } >ram
  .ctors ALIGN(0x02) : { *(.ctors) } >ram
  rodata : { *(RODATA) } >ram
  data : {*(DATA) } >ram
  bss (NOLOAD): {*(BSS)} >ram

  ___heap = HEAPSTART;
  ___heapend = HEAPSTART + HEAPSIZE;
  ___BSSSTART = ADDR(bss);
  ___BSSSIZE  = SIZEOF(bss);

  ___STACK = RAMSTART + RAMSIZE;
}
