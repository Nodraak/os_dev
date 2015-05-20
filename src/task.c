
#include "task.h"
#include "types.h"
#include "malloc.h"
#include "printf.h"

s_task tasks[TASK_MAX];
s_task *task_running;

uint32 task_flag;
uint32 *task_pagedir;

void debug(void)
{
    printf("-> from %d to %d\n", task_running-(&tasks[0])+1, task_running->next-(&tasks[0])+1);
    printf("\n");
}

void func(int i)
{
    printf("[%d.1]\n", i);
    tasking_preempt();
    printf("[%d.2]\n", i);
    //tasking_preempt();
    //printf("[%d.3]\n", i);
}

void func1(void)
{
    func(1);
}

void func2(void)
{
    func(2);
}

void func3(void)
{
    func(3);
}

void tasking_init(void)
{
    /* Get EFLAGS and CR3 */
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(task_pagedir)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(task_flag)::"%eax");

    printf("pagedir=%p flags=%d\n", task_pagedir, task_flag);

    /* create a few tasks */
    tasking_create(&tasks[0], func1, &tasks[1]);
    tasking_create(&tasks[1], func2, &tasks[0]);
    //tasking_create(&tasks[2], func3, &tasks[0]);

    task_running = &tasks[0];
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
    s_task *old = NULL;

    debug();

    old = task_running;
    task_running = task_running->next;
    tasking_switch(&old->regs, &task_running->regs);
}
