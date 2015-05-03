
#include "malloc.h"
#include "types.h"
#include "paging_low.h"
#include "kmain.h"

void *malloc(uint32 size)
{
    uint32 page_id = 0;

    page_id = paging_alloc_pages(size/PAGE_SIZE);

    return (uint32*)(kdata.paging_low_pages_addr + page_id*PAGE_SIZE);
}

void free(void *ptr)
{
    uint32 page_id = ((uint8*)ptr-kdata.paging_low_pages_addr) / PAGE_SIZE;
    paging_free_page(page_id);
}
