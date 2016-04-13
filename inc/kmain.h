#ifndef KMAIN_H_INCLUDED
#define KMAIN_H_INCLUDED

#include "io/buffer.h"
#include "lib/read_elf.c.h"
#include "x86/multiboot.h"

typedef struct _s_kdata
{
    uint8 *kernel_start;
    uint8 *kernel_end;
    uint32 kernel_size;

    char *symbol_string_table;
    uint32 symbol_string_table_size;
    elf_symbol *symbol_table;
    uint32 symbol_table_size;

    uint8 screen_x;
    uint8 screen_y;
    uint8 screen_fg;
    uint8 screen_bg;
    uint16 *screen_ptr;

    uint16 serial_port_com1;

    uint8 *page_frame_table_addr;
    uint32 page_frame_nb;
    uint32 *page_directory;

    uint32 timer_tick;
    s_buffer buffer_stdin;
} s_kdata;

extern s_kdata kdata;

void kinit(multiboot_info_t *mbi);
void kpanic(char *msg);
void kmain(void);

void _kpanic(char *msg, const char *file, int line, const char *func);
#define kpanic(msg) _kpanic(msg, __FILE__, __LINE__, __FUNCTION__)

#endif
