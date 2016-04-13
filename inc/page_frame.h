#ifndef PAGE_FRAME_H_INCLUDED
#define PAGE_FRAME_H_INCLUDED

#include "lib/types.h"
#include "x86/multiboot.h"

#define PAGE_SIZE 0x1000 /* 4 KiB */

void page_frame_init(multiboot_info_t *mbi);
multiboot_memory_map_t *page_frame_init_get_mmap_entry(multiboot_info_t *mbi, uint32 size);

void page_frame_set_addr(uint8 *addr, bool data);
void page_frame_set_id(uint32 id, bool data);

uint32 page_frame_alloc_id(uint32 nb);
void *page_frame_alloc_addr(uint32 size);

void page_frame_free_id(uint32 page_id);
void page_frame_free_addr(void *page_addr);

#endif
