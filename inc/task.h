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
} s_task;

void tasking_init(void);
void tasking_create(s_task *task, void(*func)(void), s_task *next);
void tasking_preempt(void); /* Switch task frontend */
void tasking_switch(s_registers *old, s_registers *new); /* The ASM function which actually switches */


#endif
