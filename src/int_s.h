#ifndef INT_S_H_INCLUDED
#define INT_S_H_INCLUDED

#include "types.h"

typedef struct _s_vector
{
    uint32 access_byte;
    uint32 eip;
} __attribute__((packed)) s_vector;

void getvect(s_vector *v, uint32 vect_num);
void setvect(s_vector *v, uint32 vect_num);

/* the layout of this structure must match the order of registers
pushed and popped by the exception handlers in KSTART.ASM */
typedef struct _s_regs
{
/* pushed by pusha */
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
/* pushed separately */
    uint32 ds, es, fs, gs;
    uint32 which_int, err_code;
/* pushed by exception. Exception may also push err_code.
user_esp and user_ss are pushed only if a privilege change occurs. */
    uint32 eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed)) s_regs;

#endif
