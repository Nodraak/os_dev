#ifndef KMAIN_H_INCLUDED
#define KMAIN_H_INCLUDED

#include "buffer.h"
#include "multiboot.h"

typedef struct _s_kdata
{
    s_buffer buffer_stdin;

    uint8 screen_x;
    uint8 screen_y;
    uint8 screen_fg;
    uint8 screen_bg;
    uint16 *screen_ptr;

    uint16 serial_port_com1;

    uint32 timer_tick;

    uint8 *paging_low_table_addr;
    uint8 *paging_low_pages_addr;
    uint32 paging_low_pages_nb;
} s_kdata;

extern s_kdata kdata;

void kinit(multiboot_info_t *mbi);
void kpannic(char *msg);
void kmain(void);

#endif
