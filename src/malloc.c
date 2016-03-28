
#include "malloc.h"
#include "types.h"
#include "page_frame.h"
#include "kmain.h"
#include "paging.h"


void *malloc(uint32 size)
{
    void *ptr = page_frame_alloc_addr(size);
    paging_map_frame_virtual_to_phys(ptr, ptr);
    return ptr;
}

void free(void *ptr)
{
    page_frame_free_addr(ptr);
}

void memset(void *ptr, uint32 len, uint8 data)
{
    uint8 *tmp = (uint8*)ptr;
    uint32 i;

    for (i = 0; i < len; ++i)
        tmp[i] = data;
}
