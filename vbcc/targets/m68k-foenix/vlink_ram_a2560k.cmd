BINFILE_START = 0x00000000;
FLASH_START = 0xFFC10000;
VECTOR_START = 0x00000000;
STORE_START = 0x00180000;
KERNEL_START = 0x001C0000;

STACK_LEN = 0x400;
VECTOR_LEN = 0x400;
STORE_LEN = 0x00030000;
KERNEL_LEN = 0x00040000;
FLASH_LEN = 0x00200000;
PAGE_SIZE = 0x00010000;

MEMORY
{
  binpage0: org = BINFILE_START, len = PAGE_SIZE
  binpages: org = BINFILE_START + PAGE_SIZE, len = FLASH_LEN - PAGE_SIZE
  vectors : org = VECTOR_START, len = VECTOR_LEN
  kernel: org = KERNEL_START, len = KERNEL_LEN
  storeage: org = STORE_START, len = STORE_LEN
}

SECTIONS
{
  vectors : { *(VECTORS) } >vectors
  bss (NOLOAD) : { *(BSS) } >storeage
  text ALIGN(0x02) : { *(CODE) } >kernel
  .dtors ALIGN(0x02) : { *(.dtors) } >kernel
  .ctors ALIGN(0x02) : { *(.ctors) } >kernel
  data ALIGN(0x02) : { *(DATA) } >kernel

  ___heap = ADDR(bss) + SIZEOF(bss);
  ___heapend = STORE_START + STORE_LEN - STACK_LEN;

  ___BSSSTART = ADDR(bss);
  ___BSSSIZE  = SIZEOF(bss);

  ___USER_STACK = 0x00010000;
  ___STACK = STORE_START + STORE_LEN;

  ___kernel_vma_start = ADDR(text);
  ___kernel_lma_start = LOADADDR(text);
  ___kernel_lma_end = ___kernel_lma_start + SIZEOF(text);
}
