#ifndef PAGING_ATT_S_H_INCLUDED
#define PAGING_ATT_S_H_INCLUDED

#include "types.h"

void paging_load_and_enable_page_directory(uint32 *page_directory);
void paging_reload_page_directory(void);

#endif
