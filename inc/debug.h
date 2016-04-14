#ifndef UTILS_S_H_INCLUDED
#define UTILS_S_H_INCLUDED

#include "task.h"

// .s
void magic_breakpoint(void);
void print_stack_trace(void);

// .c
void print_regs(s_registers *regs);
void _print_stack_trace(void *ebp, void *eip);

#endif
