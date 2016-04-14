#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include "lib/types.h"

#define TASK_MAX    5

typedef struct _s_registers
{
    uint32 eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} __attribute__((packed)) s_registers;

typedef struct _s_task
{
    s_registers regs;
    struct _s_task *next;
} __attribute__((packed)) s_task;

void tasking_init(void);
void tasking_create(s_task *task, void(*func)(void), s_task *next);
uint32 tasking_read_flags(void); // asm
void tasking_switch(void); // asm


#endif
