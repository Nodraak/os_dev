#ifndef KMAIN_H_INCLUDED
#define KMAIN_H_INCLUDED

#include "buffer.h"
#include "multiboot.h"

typedef struct _s_kdata
{
    s_buffer buffer_stdin;
} s_kdata;

extern s_kdata kdata;

void kinit(multiboot_info_t *mbi);
void kpannic(char *msg);
void kmain(void);

#endif
