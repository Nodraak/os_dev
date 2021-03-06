#ifndef PAGING_S_H_INCLUDED
#define PAGING_S_H_INCLUDED

#include "lib/types.h"

uint32 paging_read_cr0(void);
void paging_write_cr0(uint32 *flags);
uint32 *paging_read_cr2(void);
uint32 *paging_read_cr3(void);
void paging_write_cr3(uint32 *page_directory);
void paging_reload_page_directory(void);

#endif
