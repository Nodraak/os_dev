
#include "task.h"
#include "types.h"
#include "malloc.h"

s_task *runningTask;
s_task task1;
s_task task2;

void func1(void)
{
    printf("[1] Hello + Switching ...\n");
    tasking_preempt();
    printf("[1] Returned !\n");
}

void func2(void)
{
    printf("[2] Hello multitasking world !\n");
    tasking_preempt();
    printf("[2] Returned !\n");
}


void tasking_init(void)
{
    /* Get EFLAGS and CR3 */
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(task1.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(task1.regs.eflags)::"%eax");

    tasking_create(&task2, func2, task1.regs.eflags, (uint32*)task1.regs.cr3, NULL);
    task1.next = &task2;
    task2.next = &task1;

    runningTask = &task1;
}

void tasking_create(s_task *task, void(*func)(void), uint32 flags, uint32 *pagedir, s_task *next)
{
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (uint32)func;
    task->regs.cr3 = (uint32)pagedir;
    task->regs.esp = (uint32)malloc(1) + 0x1000;
    task->next = next;
}

void tasking_preempt(void)
{
    s_task *last = runningTask;
    runningTask = runningTask->next;
    tasking_switch(&last->regs, &runningTask->regs);
}
