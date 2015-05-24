#ifndef PAGING_H_INCLUDED
#define PAGING_H_INCLUDED

#include "types.h"

#define PF_FLAG_BASE        ((1 << 2) | (1 << 1))           /* access everybody + read / write */
#define PF_FLAG_PRESENT     (1 << 0)

void paging_map_frame_virtual_to_phys(void *virt, void *phys);
void paging_init(void);

void _paging_test_addr(uint32 *ptr1, uint32 *ptr2);

#endif
