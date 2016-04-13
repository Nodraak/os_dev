#ifndef READ_ELF_C_H_INCLUDED
#define READ_ELF_C_H_INCLUDED

#include "lib/types.h"
#include "x86/multiboot.h"

/* The ELF file header.  This appears at the start of every ELF file.  */

typedef struct _elf_header
{
    uint32 magic;           /* Magic number and other info */
    uint8 class;            /* 32 or 64 bits */
    uint8 data;             /* Data encoding */
    uint8 other[10];        /* Other info */
    uint16 type;            /* Object file type */
    uint16 machine;         /* Architecture */
    uint32 version;         /* Object file version */
    uint32 entry;           /* Entry point virtual address */
    uint32 phoff;           /* Program header table file offset */
    uint32 shoff;           /* Section header table file offset */
    uint32 flags;           /* Processor-specific flags */
    uint16 ehsize;          /* ELF header size in bytes */
    uint16 phentsize;       /* Program header table entry size */
    uint16 phnum;           /* Program header table entry count */
    uint16 shentsize;       /* Section header table entry size */
    uint16 shnum;           /* Section header table entry count */
    uint16 shstrndx;        /* Section header string table index */
} __attribute__((packed)) elf_header;

#define ELF_MAGIC           0x464C457F

#define ELF_CLASS_32        1   /* 32-bit objects */
#define ELF_CLASS_64        2   /* 64-bit objects */

#define ELF_DATA_2LSB       1   /* 2's complement, little endian */
#define ELF_DATA_2MSB       2   /* 2's complement, big endian */

#define ELF_TYPE_NONE       0   /* No file type */
#define ELF_TYPE_REL        1   /* Relocatable file */
#define ELF_TYPE_EXEC       2   /* Executable file */
#define ELF_TYPE_DYN        3   /* Shared object file */
#define ELF_TYPE_CORE       4   /* Core file */

#define ELF_MACHINE_386     3   /* Intel 80386 */
#define ELF_MACHINE_X86_64  62  /* AMD x86-64 architecture */


/* Section header.  */

typedef struct _elf_section_header {
    uint32 name;
    uint32 type;
    uint32 flags;
    uint32 addr;
    uint32 offset;
    uint32 size;
    uint32 link;
    uint32 info;
    uint32 addralign;
    uint32 entsize;
} __attribute__((packed)) elf_section_header;

#define ELF_SH_TYPE_NULL            0       /* Section header table entry unused */
#define ELF_SH_TYPE_PROGBITS        1       /* Program data */
#define ELF_SH_TYPE_SYMTAB          2       /* Symbol table */
#define ELF_SH_TYPE_STRTAB          3       /* String table */
#define ELF_SH_TYPE_RELA            4       /* Relocation entries with addends */
#define ELF_SH_TYPE_HASH            5       /* Symbol hash table */
#define ELF_SH_TYPE_DYNAMIC         6       /* Dynamic linking information */
#define ELF_SH_TYPE_NOTE            7       /* Notes */
#define ELF_SH_TYPE_NOBITS          8       /* Program space with no data (bss) */
#define ELF_SH_TYPE_REL             9       /* Relocation entries, no addends */
#define ELF_SH_TYPE_SHLIB           10      /* Reserved */
#define ELF_SH_TYPE_DYNSYM          11      /* Dynamic linker symbol table */
#define ELF_SH_TYPE_INIT_ARRAY      14      /* Array of constructors */
#define ELF_SH_TYPE_FINI_ARRAY      15      /* Array of destructors */
#define ELF_SH_TYPE_PREINIT_ARRAY   16      /* Array of pre-constructors */
#define ELF_SH_TYPE_GROUP           17      /* Section group */

/* Symbol table entry.  */

typedef struct _elf_symbol
{
  uint32 name;      /* Symbol name (string tbl index) */
  uint32 value;     /* Symbol value */
  uint32 size;      /* Symbol size */
  uint8 info;       /* Symbol type and binding */
  uint8 other;      /* Symbol visibility */
  uint16 shndx;     /* Section index */
} __attribute__((packed)) elf_symbol;

#define EFL_SYMBOL_SIZE sizeof(elf_symbol)


/* Program segment header.  */

typedef struct _elf_program_header {
    uint32 type;
    uint32 offset;
    uint32 vaddr;
    uint32 paddr;
    uint32 filesz;
    uint32 memsz;
    uint32 flags;
    uint32 align;
} __attribute__((packed)) elf_program_header;

/* Prototypes. */

void elf_print_stats(multiboot_elf_section_header_table_t mbi_eshdr);
char *resolve_symbol(void *addr);

#endif
