
#include "malloc.h"
#include "types.h"
#include "paging_low.h"

void kpannic(char *msg);

void *malloc(uint32 size)
{
    uint32 page_id = 0;

    if (size > PAGE_SIZE)
        kpannic("MALLOC : can't allocate more than PAGE_SIZE");

    page_id = paging_alloc_pages(1); // size/PAGE_SIZE

    printf("malloc ...\n");

    return (uint32*)(paging_low_pages_addr + page_id*PAGE_SIZE);
}

void free(void *ptr)
{
    uint32 page_id = ((uint8*)ptr-paging_low_pages_addr) / PAGE_SIZE;
    paging_free_page(page_id);
}
