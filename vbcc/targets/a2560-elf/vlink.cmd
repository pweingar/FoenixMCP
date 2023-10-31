RAMSTART = 0x00020000;
RAMSIZE  = 0x00030000;
STACKLEN = 0x400;

MEMORY
{
 ram : org = RAMSTART, len = RAMSIZE - STACKLEN
}

SECTIONS
{
  text : {*(CODE)} >ram
  .dtors : { *(.dtors) } > ram
  .ctors : { *(.ctors) } > ram
  rodata : {*(RODATA)} >ram
  data: {*(DATA)} >ram
  bss (NOLOAD): {*(BSS)} >ram

  ___heap = ADDR(bss) + SIZEOF(bss);
  ___heapend = RAMSTART + RAMSIZE - STACKLEN;


  ___BSSSTART = ADDR(bss);
  ___BSSSIZE  = SIZEOF(bss);

  ___STACK = RAMSTART + RAMSIZE;
}

