
#include "kmain.h"
#include "lib/printf.h"
#include "lib/types.h"
#include "x86/multiboot.h"

#include "lib/read_elf.c.h"


char *section_header_type_to_str[] = {
    "NULL",
    "PROGBITS",
    "SYMTAB",
    "STRTAB",
    "RELA",
    "HASH",
    "DYNAMIC",
    "NOTE",
    "NOBITS",
    "REL",
    "SHLIB",
    "DYNSYM",
    "INIT_ARRAY",
    "FINI_ARRAY",
    "PREINIT_ARRAY",
    "GROUP",
};

char *symbol_table_type_to_str[] = {
    "NOTYPE",
    "OBJECT",
    "FUNC",
    "SECTION",
    "FILE",
};

#define ELF_ST_INFO_TO_TYPE(i) ((i) & 0xF)


void elf_print_stats(multiboot_elf_section_header_table_t mbi_eshdr)
{
    elf_section_header *eshdr = (elf_section_header*)mbi_eshdr.addr;
    uint32 section_count = mbi_eshdr.num;
    uint32 section_string_table_index = mbi_eshdr.shndx;

    uint32 i = 0;
    uint8 symbol_string_table_index = 0, symbol_table_index = 0;
    char *string_table = NULL;
    elf_symbol *esyhdr = NULL;

    /* section header */

    string_table = (char*)eshdr[section_string_table_index].addr;

    printf("\n==> Section header\n");
    printf("  i name                    type             addr      size\n");
    for (i = 0; i < section_count; ++i)
    {
        printf(" %2d", i);
        printf(" %-20s", &string_table[eshdr[i].name]);
        printf(" %6x - %-10s", eshdr[i].type, section_header_type_to_str[eshdr[i].type]);
        printf(" %6x", eshdr[i].addr);
        printf(" %6x\n", eshdr[i].size);

        if (eshdr[i].type == ELF_SH_TYPE_SYMTAB)
        {
            if (symbol_table_index != 0)
                kpanic("two symbol_table found");
            symbol_table_index = i;
        }

        if ((eshdr[i].type == ELF_SH_TYPE_STRTAB) && (i != section_string_table_index))
        {
            if (symbol_string_table_index != 0)
                kpanic("two symbol_string_table found");
            symbol_string_table_index = i;

        }
    }

    /* symbol table */

    printf("\n==> Symbol table\n");

    if (eshdr[symbol_table_index].size % EFL_SYMBOL_SIZE != 0)
        kpanic("unexpected symbol_table size");

    esyhdr = (elf_symbol*)eshdr[symbol_table_index].addr;
    string_table = (char*)eshdr[symbol_string_table_index].addr;
    printf("   i name                               value   size   type   section index\n");
    for (i = 0; i < eshdr[symbol_table_index].size/EFL_SYMBOL_SIZE; ++i)
    {
        printf(" %3d", i);
        printf(" %-32s", &string_table[esyhdr[i].name]);
        printf(" %8x", esyhdr[i].value);
        printf(" %6d", esyhdr[i].size);
        printf(" %-10s", symbol_table_type_to_str[ELF_ST_INFO_TO_TYPE(esyhdr[i].info)]);
        printf(" %6d\n", esyhdr[i].shndx);
    }

    /* save pointers and stuff in kdata for easy reuse */

    kdata.symbol_string_table = (char*)eshdr[symbol_string_table_index].addr;
    kdata.symbol_string_table_size = eshdr[symbol_string_table_index].size;
    kdata.symbol_table = (elf_symbol*)eshdr[symbol_table_index].addr;
    kdata.symbol_table_size = eshdr[symbol_table_index].size;
}


char *resolve_symbol(void *addr)
{
    char *ret = NULL;
    uint32 i = 0, closest_symbol = 0;
    elf_symbol *esyhdr = NULL;

    if (addr == 0x0)
        return "<null>";

    esyhdr = (elf_symbol*)kdata.symbol_table;

    for (i = 0; i < kdata.symbol_table_size/EFL_SYMBOL_SIZE; ++i)
    {
        if ((ELF_ST_INFO_TO_TYPE(esyhdr[i].info) == 2) \
            && (closest_symbol < esyhdr[i].value) && (esyhdr[i].value <= (uint32)addr)
        )
        {
            closest_symbol = esyhdr[i].value;
            ret = &kdata.symbol_string_table[esyhdr[i].name];
        }
    }

    return ret;
}
