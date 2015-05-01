
#include "paging_low.h"
#include "types.h"
#include "multiboot.h"
#include "printf.h"

uint8 *paging_low_table_addr;
uint8 *paging_low_pages_addr;
uint32 paging_low_pages_nb;

void kpannic(char *msg);

void paging_low_init(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap = NULL;
    uint32 mem_upper, mem_lower, i;
    uint8 ok;

    printf("Paging :\n");

    /* are mem_lower and mem_upper valid ? */
    if (!(mbi->flags & (1 << 0)))
        kpannic("PAGING : mem_lower and mem_upper flag not set");

    mem_upper = mbi->mem_upper * 1024;
    mem_lower = mbi->mem_lower * 1024;

    printf("\tmem_upper=%u\n", mem_upper);
    printf("\tmem_lower=%u\n", mem_lower);
    printf("\tPAGE_SIZE=%u\n", PAGE_SIZE);

    /* paging_low_pages_nb */
    paging_low_pages_nb = mem_upper / PAGE_SIZE;
    printf("\tpaging_low_pages_nb=%u\n", paging_low_pages_nb);
    printf("\tmem_lower=%u\n", mem_lower);

    printf("\tSpace needed for page table=%u\n", paging_low_pages_nb/8);
    if (paging_low_pages_nb/8 > mem_lower)
        kpannic("PAGING : no space for page table");

    /* paging_low_table_addr */
    ok = 0;
    mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
    while ((uint32)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1 && mmap->len_low == mem_lower)
        {
            paging_low_table_addr = (uint8*)mmap->addr_low;
            ok ++;
        }

        mmap = (multiboot_memory_map_t*)((uint32)mmap + mmap->size + sizeof(uint32));
    }
    if (ok != 1)
        kpannic("PAGING : paging_low_table_addr not found");

    /* paging_low_pages_addr */
    ok = 0;
    mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
    while ((uint32)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1 && mmap->len_low == mem_upper)
        {
            paging_low_pages_addr = (uint8*)mmap->addr_low;
            ok ++;
        }

        mmap = (multiboot_memory_map_t*)((uint32)mmap + mmap->size + sizeof(uint32));
    }
    if (ok != 1)
        kpannic("PAGING : paging_low_pages_addr not found");

    printf("\tpaging_low_pages_addr=%u\n", (uint32)paging_low_pages_addr);
    printf("\tRAM available=%u\n", PAGE_SIZE*paging_low_pages_nb);

    /* init page table : set to unused */
    for (i = 0; i < paging_low_pages_nb/8; ++i)
        paging_low_table_addr[i] = 0;
}
