
#include "paging.h"
#include "types.h"
#include "multiboot.h"
#include "printf.h"
#include "bitfield.h"
#include "kmain.h"


void page_frame_init(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap = NULL;
    uint32 mem_upper = 0, mem_lower = 0, i = 0;
    uint8 nb = 0;

    printf("Paging :\n");
    printf("\tkstart : %p\n", kdata.kernel_start);
    printf("\tkend : %p\n", kdata.kernel_end);
    printf("\tsize : %d B\n", (uint32)(kdata.kernel_end-kdata.kernel_start));

    /* are mem_lower and mem_upper valid ? */
    if ((mbi->flags & (1 << 0)) == 0)
        kpannic("PAGING : mem_lower and mem_upper flag not set");

    mem_upper = mbi->mem_upper * 1024;
    mem_lower = mbi->mem_lower * 1024;

    printf("\tavailable mem_upper : %u B\n", mem_upper);
    printf("\tavailable mem_lower : %u B\n", mem_lower);

    /* paging_frame_pages_nb */
    kdata.paging_frame_pages_nb = ((mem_upper/PAGE_SIZE)/8)*8; /* round to 8 to have full byte bitfield */

    printf("\tpaging_frame_pages_nb : %u\n", kdata.paging_frame_pages_nb);
    printf("\tmemory used by the frame_table : %u B\n", kdata.paging_frame_pages_nb/8);

    if (kdata.paging_frame_pages_nb/8 > mem_lower)
        kpannic("PAGING : no space for page table");

    /* paging_frame_table_addr -> mem_lower */
    nb = 0;
    mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
    while ((uint32)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1 && mmap->len_low == mem_lower)
        {
            kdata.paging_frame_table_addr = (uint8*)mmap->addr_low;
            nb ++;
        }

        mmap = (multiboot_memory_map_t*)((uint32)mmap + mmap->size + sizeof(uint32));
    }
    if (nb != 1)
        kpannic("PAGING : paging_frame_table_addr not found");

    /* paging_frame_pages_addr -> mem_upper */
    nb = 0;
    mmap = (multiboot_memory_map_t*)mbi->mmap_addr;
    while ((uint32)mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        if (mmap->type == 1 && mmap->len_low == mem_upper)
        {
            kdata.paging_frame_pages_addr = (uint8*)mmap->addr_low;
            nb ++;
        }

        mmap = (multiboot_memory_map_t*)((uint32)mmap + mmap->size + sizeof(uint32));
    }
    if (nb != 1)
        kpannic("PAGING : paging_frame_pages_addr not found");

    printf("\tpaging_low_pages_addr : %p\n", kdata.paging_frame_pages_addr);
    printf("\tRAM available : %u\n", PAGE_SIZE*kdata.paging_frame_pages_nb);

    /* init page table */
    for (i = 0; i < kdata.paging_frame_pages_nb/8; ++i)
        kdata.paging_frame_pages_addr[i] = 0;

    uint8 *ptr;
    for (ptr = kdata.kernel_start; ptr < kdata.kernel_end; ptr += PAGE_SIZE)
        page_frame_alloc_pages_addr(1);

    printf("Paging ok.\n");
}

uint32 page_frame_alloc_pages_id(uint32 nb)
{
    uint32 i = 0, *ptr = NULL;

    if (nb == 0)
        kpannic("PAGING ALLOC : can't allocate zero page");

    if (nb != 1)
        kpannic("PAGING ALLOC : can't alloc more than one page");

    for (i = 0; i < kdata.paging_frame_pages_nb; ++i)
    {
        ptr = (uint32*)kdata.paging_frame_table_addr;

        if (bitfield_get(ptr[i/8], i%8) == 0)
        {
            bitfield_set(&ptr[i/8], i%8, 1);
            return i;
        }
    }

    kpannic("PAGING ALLOC : no free page found");
    return 0; /* silent warning */
}

void *page_frame_alloc_pages_addr(uint32 size)
{
    uint32 nb_frame, page_id = 0;

    if (size == 0)
    {
        printf("WARNING : allocating zero bytes ...\n");
        return 0;
    }

    nb_frame = size / PAGE_SIZE;
    if (size % PAGE_SIZE)
        nb_frame ++;

    page_id = page_frame_alloc_pages_id(nb_frame);

    return (kdata.paging_frame_pages_addr + page_id*PAGE_SIZE);
}

void page_frame_free_page_id(uint32 page_id)
{
    uint32 *ptr = (uint32*)kdata.paging_frame_table_addr;

    if (bitfield_get(ptr[page_id/8], page_id%8) == 0 && page_id != 0)
        kpannic("PAGING FREE : page not allocated");

    bitfield_set(&ptr[page_id/8], page_id%8, 0);
}

void page_frame_free_page_addr(void *page_addr)
{
    uint32 page_id = ((uint8*)page_addr-kdata.paging_frame_pages_addr) / PAGE_SIZE;
    page_frame_free_page_id(page_id);
}
