#ifndef PAGING_H_INCLUDED
#define PAGING_H_INCLUDED

#include "types.h"

#define PF_FLAG_BASE        ((1 << 2) | (1 << 1))           /* access everybody + read / write */
#define PF_FLAG_PRESENT     (1 << 0)

/*
typedef struct page {
   unsigned int present    : 1;   // Page present in memory
   unsigned int rw         : 1;   // Read-only if clear, readwrite if set
   unsigned int user       : 1;   // Supervisor level only if clear
   unsigned int accessed   : 1;   // Has the page been accessed since last refresh?
   unsigned int dirty      : 1;   // Has the page been written to since last refresh?
   unsigned int unused     : 7;   // Amalgamation of unused and reserved bits
   unsigned int frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
   page_t pages[1024];
} page_table_t;

typedef struct page_directory {
   page_table_t *tables[1024];
   unsigned int tablesPhysical[1024];
   unsigned int physicalAddr;
} page_directory_t;
*/

void paging_map_frame_virtual_to_phys(void *virt, void *phys);
void paging_init(void);

void _paging_test_addr(char *ptr1, char *ptr2);

#endif
