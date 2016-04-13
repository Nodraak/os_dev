
#include "debug.h"
#include "kmain.h"
#include "lib/malloc.h"
#include "lib/printf.h"
#include "lib/read_elf.c.h"
#include "lib/types.h"
#include "page_frame.h"
#include "paging.c.h"
#include "x86/paging.s.h"

#include "task.h"


s_task tasks[TASK_MAX];
s_task *task_running;

uint32 task_flag;
uint32 *task_pagedir;


void func1(void)
{
    while (1)
    {
        printf("in task 1.1\n");
        printf("in task 1.2\n");
        tasking_switch();
    }
}

void func2(void)
{
    while (1)
    {
        printf("in task 2.1\n");
        tasking_switch();
        printf("in task 2.2\n");
    }
}

void func3(void)
{
    while (1)
    {
        printf("in task 3.1\n");
        tasking_switch();
        printf("in task 3.2\n");
    }
}

void tasking_init(void)
{
    /* Get EFLAGS and CR3 */
    task_pagedir = paging_read_cr3();
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(task_flag)::"%eax");

    printf("pagedir=%p flags=%d\n", task_pagedir, task_flag);

    /* create a few tasks */
    tasking_create(&tasks[0], func1, &tasks[1]);
    tasking_create(&tasks[1], func2, &tasks[2]);
    tasking_create(&tasks[2], func3, &tasks[0]);

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
    task->regs.ebp = (uint32)malloc(PAGE_SIZE) + PAGE_SIZE/2; // todo check that
    task->regs.esp = task->regs.ebp;
    printf("create task, task.esp=%x=%d\n", task->regs.esp, task->regs.esp);
    task->next = next;

    void *addr = (void*)(task->regs.eip/PAGE_SIZE*PAGE_SIZE);
    paging_map_frame_virtual_to_phys(addr, addr); // todo check that
    addr = (void*)(task->regs.ebp/PAGE_SIZE*PAGE_SIZE);
    paging_map_frame_virtual_to_phys(addr, addr); // todo check that
}
