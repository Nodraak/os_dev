
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
    int i = 0;
    for (i = 0; i < 3; ++i)
    {
        printf("in task 1.1 - %d\n", i);
        print_stack_trace();
        tasking_switch();
        printf("in task 1.2 - %d\n", i);
        print_stack_trace();
    }
}

void func2(void)
{
    while (1)
    {
        printf("in task 2.1\n");
        print_stack_trace();
        tasking_switch();
        printf("in task 2.2\n");
        print_stack_trace();
    }
}

void func3(void)
{
    while (1)
    {
        printf("in task 3.1\n");
        print_stack_trace();
        tasking_switch();
        printf("in task 3.2\n");
        print_stack_trace();
    }
}

void tasking_init(void)
{
    /* Get EFLAGS and CR3 */
    task_pagedir = paging_read_cr3();
    task_flag = tasking_read_flags();

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
    printf("[Tasking] Create task with callback=%p\n", func);

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

    task->next = next;

    void *addr = (void*)(task->regs.eip/PAGE_SIZE*PAGE_SIZE);
    paging_map_frame_virtual_to_phys(addr, addr);
}
