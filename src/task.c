
#include "task.h"
#include "types.h"
#include "malloc.h"

s_task *runningTask;
s_task task1;
s_task task2;

uint32 task_flag;
uint32 *task_pagedir;

void func1(void)
{
    printf("[1] Hello + Switching ...\n");
    tasking_preempt();
    printf("[1] Middle !\n");
    tasking_preempt();
    printf("[1] Returned !\n");
}

void func2(void)
{
    printf("[2] Hello multitasking world !\n");
    tasking_preempt();
    printf("[2] Middle !\n");
    tasking_preempt();
    printf("[2] Returned !\n");
}

void tasking_init(void)
{
    /* Get EFLAGS and CR3 */
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(task_flag)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(task_pagedir)::"%eax");

    /* create a few tasks */
    tasking_create(&task1, func1, &task2);
    tasking_create(&task2, func2, &task1);

    runningTask = &task1;
    func1();
}

void tasking_create(s_task *task, void(*func)(void), s_task *next)
{
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = task_flag;
    task->regs.eip = (uint32)func;
    task->regs.cr3 = (uint32)task_pagedir;
    task->regs.esp = (uint32)malloc(1) + 0x1000;
    task->next = next;
}

void tasking_preempt(void)
{
    s_task *old = NULL, *new = NULL;

    old = runningTask;
    new = old->next;
    runningTask = new;
    tasking_switch(&old->regs, &new->regs);
}
