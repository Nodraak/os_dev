
#include "malloc.h"
#include "types.h"
#include "paging.h"
#include "kmain.h"

void *malloc(uint32 size)
{
    return page_frame_alloc_pages_addr(size);
}

void free(void *ptr)
{
    page_frame_free_page_addr(ptr);
}
