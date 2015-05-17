#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include "types.h"

typedef struct _s_registers
{
    uint32 eax, ebx, ecx, edx;
    uint32 esi, edi;
    uint32 esp, ebp, eip;
    uint32 eflags, cr3;
} s_registers;

typedef struct _s_task
{
    s_registers regs;
    struct s_task *next;
} s_task;

void tasking_init(void);
void tasking_create(s_task *task, void(*func)(void), uint32 flags, uint32 *pagedir, s_task *next);
void tasking_preempt(void); /* Switch task frontend */
void tasking_switch(s_registers *old, s_registers *new); /* The ASM function which actually switches */


#endif
