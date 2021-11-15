#ifndef _ELF_H
#define _ELF_H

#define ET_NONE		 0	    // No file type
#define ET_REL		 1	    // Relocatable file
#define ET_EXEC		 2	    // Executable file
#define ET_DYN		 3	    // Shared object file
#define ET_CORE		 4	    // Core file
#define	ET_NUM		 5	    // Number of defined types 
#define EM_NONE		 0	    // No machine
#define EM_386		 3	    // Intel 80386
#define EM_68K		 4	    // Motorola m68k family
#define EM_MIPS		 8	    // MIPS R3000 big-endian
#define EM_MIPS_LE	10	    // MIPS R3000 little-endian
#define EM_PPC		20	    // PowerPC
#define EM_PPC64	21	    // PowerPC 64-bit
#define EM_ARM		40	    // ARM

#if defined(__powerpc__)
	#define CPU_ARCH	EM_PPC
#elif defined(__powerpc64__)
	#define CPU_ARCH	EM_PPC64
#elif defined(__arm__)
	#define CPU_ARCH	EM_ARM
#elif defined(__m68k__) || defined(__M68K__)
	#define CPU_ARCH	EM_68K
#else
	#define CPU_ARCH	EM_NONE
#endif

typedef enum {
    PT_NULL = 0,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
	PT_TLS
} progtype_t;

typedef enum {
	HDATA_NONE = 0,
	HDATA_LITTLE,
	HDATA_BIG
} endianness_t;

struct ident_t {
	unsigned char	magic[4];
	unsigned char	class;
	unsigned char	data;
	unsigned char	version;
	unsigned char	osabi;
	unsigned char	abiversion;
	unsigned char	padding[7];
};

typedef struct {
    struct ident_t	ident;
    unsigned short	type;
    unsigned short	machine;
    unsigned long	version;
    unsigned long	entry;
    unsigned long	progOffset;
    unsigned long	shoff;
    unsigned long	flags;
    unsigned short	ehsize;
    unsigned short	progSize;
    unsigned short	progNum;
    unsigned short	shentsize;
    unsigned short	shnum;
    unsigned short	shtrndx;
} elf32_header;

typedef struct {
    unsigned long	type;
    unsigned long	offset;
    unsigned long	virtAddr;
    unsigned long	physAddr;
    unsigned long	fileSize;
    unsigned long	memSize;
    unsigned long	flags;
    unsigned long	align;
} elf32_program_header;

#endif
