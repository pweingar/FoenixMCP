FLASHSTART = 0x00E00000;
FLASHLEN = 0x00200000;
RAMSTART = 0x00001000;
RAMSIZE  = 0x003ff000;
STACKLEN = 0x400;
VECTORSIZE = 0x400;
BINFILESTART = 0x00000000;

MEMORY
{
  vectors : org = 0x000000, len = VECTORSIZE
  ram : org = RAMSTART + VECTORSIZE, len = RAMSIZE - STACKLEN
  flash: org = FLASHSTART, len = FLASHLEN
  filevecs: org = BINFILESTART, len = VECTORSIZE
  filecode: org = BINFILESTART + VECTORSIZE, len = FLASHLEN - VECTORSIZE
}

SECTIONS
{
  vectors : { *(VECTORS) } >vectors AT>filevecs
  text : {*(CODE)} >flash AT>filecode
  .dtors : { *(.dtors) } >flash AT>filecode
  .ctors : { *(.ctors) } >flash AT>filecode
  rodata : {*(RODATA)} >flash AT>filecode
  data: {*(DATA)} >ram
  bss (NOLOAD): {*(BSS)} >ram

  ___heap = ADDR(bss) + SIZEOF(bss);
  ___heapend = RAMSTART + RAMSIZE - STACKLEN;


  ___BSSSTART = ADDR(bss);
  ___BSSSIZE  = SIZEOF(bss);

  ___STACK = RAMSTART + RAMSIZE;
}
