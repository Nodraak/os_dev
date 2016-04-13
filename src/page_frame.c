
#include "bitfield.h"
#include "kmain.h"
#include "lib/malloc.h"
#include "lib/printf.h"
#include "lib/types.h"
#include "x86/multiboot.h"

#include "page_frame.h"


void page_frame_init(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap = NULL;
    uint32 mem_upper = 0, mem_lower = 0, page_frame_table_size = 0;
    uint8 *max_addr = NULL, *ptr;

    printf("Page frame :\n");
    printf("\tkstart : %p\n", kdata.kernel_start);
    printf("\tkend : %p\n", kdata.kernel_end);
    printf("\tsize : %d B\n", kdata.kernel_size);

    /*
        get max addr of memory
    */

    max_addr = (uint8*)0;
    mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
    while ((uint32)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->addr_high || mmap->len_high)
            kpanic("PAGE FRAME : 64 bits addr not implemented");

        if ((uint8*)mmap->addr_low > max_addr)
            max_addr = (uint8*)mmap->addr_low;

        mmap = (multiboot_memory_map_t*)((uint32)mmap + mmap->size + sizeof(uint32));
    }

    kdata.page_frame_nb = (uint32)max_addr / PAGE_SIZE;

    printf("\tpage_frame_nb : %u\n", kdata.page_frame_nb);

    if (kdata.page_frame_nb % 8)
        kpanic("PAGE FRAME : page_frame_nb %% 8 != 0 (incomming bitfield error)");

    page_frame_table_size = kdata.page_frame_nb/8;

    printf("\tpage_frame_table_size : %u B\n", page_frame_table_size);

    /*
        get size of mem_upper + mem_lower
    */

    if ((mbi->flags & (1 << 0)) == 0)
        kpanic("PAGE FRAME : mem_lower and mem_upper flags not set");

    mem_upper = mbi->mem_upper * 1024;
    mem_lower = mbi->mem_lower * 1024;

    printf("\tavailable mem_upper : %u B\n", mem_upper);
    printf("\tavailable mem_lower : %u B\n", mem_lower);

    /*
        allocate page_frame_table in mem_lower
    */

    mmap = page_frame_init_get_mmap_entry(mbi, mem_lower);
    kdata.page_frame_table_addr = (uint8*)mmap->addr_low;

    /*
        init page_frame_table :
            mark everything as used (to prevent errors if allocating weird reserved regions (for the bios or whatever))
            mark mem_upper as free (todo: we could mark mem_lower as free too)
            mark kernel data as used

    ram addr            0                                max
    whole physical ram  |---------------------------------|
    mem_upper                   |-------------------------|
    kernel data                     |---|
    */

    memset(kdata.page_frame_table_addr, page_frame_table_size, 0xFF);

    mmap = page_frame_init_get_mmap_entry(mbi, mem_upper);
    for (ptr = (uint8*)mmap->addr_low; (uint32)ptr-mmap->addr_low < mmap->len_low; ptr += PAGE_SIZE)
        page_frame_set_addr(ptr, 0);

    for (ptr = (uint8*)kdata.kernel_start; ptr < (uint8*)kdata.kernel_end; ptr += PAGE_SIZE)
        page_frame_set_addr(ptr, 1);

    for (
        ptr = (uint8*)kdata.symbol_string_table;
        ptr < (uint8*)kdata.symbol_string_table + kdata.symbol_string_table_size;
        ptr += PAGE_SIZE
    )
        page_frame_set_addr(ptr, 1);

    for (
        ptr = (uint8*)kdata.symbol_table;
        ptr < (uint8*)kdata.symbol_table + kdata.symbol_table_size;
        ptr += PAGE_SIZE
    )
        page_frame_set_addr(ptr, 1);

    printf("Page frame ok.\n");
}


multiboot_memory_map_t *page_frame_init_get_mmap_entry(multiboot_info_t *mbi, uint32 size)
{
    multiboot_memory_map_t *ret = NULL;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
    uint8 nb = 0;

    while ((uint32)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1 && mmap->len_low == size)
        {
            ret = mmap;
            nb ++;
        }

        mmap = (multiboot_memory_map_t*)((uint32)mmap + mmap->size + sizeof(uint32));
    }

    if (nb != 1)
        kpanic("PAGE FRAME : page_frame_init_get_mmap_entry() returned multiple results");

    return ret;
}


void page_frame_set_addr(uint8 *addr, bool data) /* todo: bool: make defines PAGE_FRAME_USED / PAGE_FRAME_FREE */
{
    page_frame_set_id((uint32)addr/PAGE_SIZE, data);
}


void page_frame_set_id(uint32 id, bool data)
{
    bitfield_set(&kdata.page_frame_table_addr[id/8], id%8, data);
}


uint32 page_frame_alloc_id(uint32 nb)
{
    uint32 i = 0;
    uint8 *ptr = NULL;

    if (nb == 0)
        kpanic("PAGE FRAME ALLOC : can't allocate zero page");

    if (nb != 1)
        kpanic("PAGE FRAME ALLOC : can't alloc more than one page"); /* todo */

    for (i = 0; i < kdata.page_frame_nb; ++i)
    {
        ptr = kdata.page_frame_table_addr;

        if (bitfield_get(ptr[i/8], i%8) == 0)
        {
            bitfield_set(&ptr[i/8], i%8, 1);
            return i;
        }
    }

    kpanic("PAGE FRAME ALLOC : no free page found");
    return 0; /* silent warning */
}


void *page_frame_alloc_addr(uint32 size)
{
    uint32 page_frame_nb, page_id = 0;

    if (size == 0)
        kpanic("PAGE FRAME ALLOC : can't allocate zero bytes");

    page_frame_nb = size / PAGE_SIZE;
    if (size % PAGE_SIZE != 0)
        page_frame_nb ++;

    page_id = page_frame_alloc_id(page_frame_nb);

    return (uint32*)(page_id * PAGE_SIZE);
}


void page_frame_free_id(uint32 page_id)
{
    uint8 *ptr = kdata.page_frame_table_addr;

    if (bitfield_get(ptr[page_id/8], page_id%8) == 0)
        kpanic("PAGE FRAME FREE : freeing a not allocated page");

    bitfield_set(&ptr[page_id/8], page_id%8, 0);
}


void page_frame_free_addr(void *page_addr)
{
    uint32 page_id = (uint32)page_addr / PAGE_SIZE;
    page_frame_free_id(page_id);
}
