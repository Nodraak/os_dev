#ifndef PAGING_LOW_H_INCLUDED
#define PAGING_LOW_H_INCLUDED

#include "types.h"
#include "multiboot.h"

#define PAGE_SIZE 0x1000 /* 4 KiB */

void page_frame_init(multiboot_info_t *mbi);
uint32 page_frame_alloc_pages_id(uint32 nb);
void *page_frame_alloc_pages_addr(uint32 size);
void page_frame_free_page_id(uint32 page_id);
void page_frame_free_page_addr(void *page_addr);

#endif
