#ifndef PAGING_LOW_H_INCLUDED
#define PAGING_LOW_H_INCLUDED

#include "types.h"
#include "multiboot.h"

#define PAGE_SIZE (4*1024)

void paging_low_init(multiboot_info_t *mbi);
uint32 paging_alloc_pages(uint32 nb);
void paging_free_page(uint32 page_id);

#endif
