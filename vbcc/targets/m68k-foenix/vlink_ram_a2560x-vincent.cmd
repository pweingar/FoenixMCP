VECTOR_START = 0x00000000;
VECTOR_LEN = 0x1000;
STORE_LEN   = 0x00010000;
STACK_LEN = 0x400;
SYSTEM_RAM_TOP = 0x400000;
OS_RAM_AREA = SYSTEM_RAM_TOP - 0x100000;

MEMORY
{
  vectors :   org = VECTOR_START             ,len = VECTOR_LEN
  system_ram: org = VECTOR_START+VECTOR_LEN  ,len = OS_RAM_AREA - VECTOR_LEN
  os_area:    org = OS_RAM_AREA              ,len = SYSTEM_RAM_TOP - OS_RAM_AREA
}

SECTIONS
{
  vectors : { *(VECTORS) } >vectors

  bss ALIGN(4) (NOLOAD) : {
      ___BSSSTART = .;
      *(BSS)

      . = ALIGN(4);
      RESERVE(STACK_LEN);
      ___USER_STACK = .;

      . = ALIGN(4);
      RESERVE(STACK_LEN);
      ___STACK = .;
      ___stack = .;

      . = ALIGN(4);
      ___heap = .;
      ___heapptr = .
      RESERVE(STORE_LEN);
      ___heapend = .;

      . = ALIGN(4);
      ___memory_start = .;
  } >os_area

  data ALIGN(0x04)   : { *(DATA)   } >os_area
  text   ALIGN(0x04) : { *(CODE)   } >os_area
  .dtors ALIGN(0x04) : { *(.dtors) } >os_area
  .ctors ALIGN(0x04) : { *(.ctors) } >os_area

  ___BSSSIZE = SIZEOF(bss);
  _RAM_TOP = ADDR(bss);

}
