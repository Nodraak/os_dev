#ifndef PAGING_H_INCLUDED
#define PAGING_H_INCLUDED

#include "types.h"

#define PF_FLAG_BASE        ((1 << 2) | (1 << 1))           /* access everybody + read / write */
#define PF_FLAG_PRESENT     (PF_FLAG_BASE | (1 << 0))

void paging_map_frame_virtual_to_phys(uint32 *page_directory, void *virt, void *phys, bool reload);
void paging_init(void);

#endif
