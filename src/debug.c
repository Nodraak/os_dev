
#include "lib/printf.h"
#include "lib/read_elf.c.h"

#include "debug.h"


void print_regs(s_registers *regs)
{
    printf("print regs\n");
    printf("\t%s=%x=%d\n", "eax", regs->eax, regs->eax);
    printf("\t%s=%x=%d\n", "ebx", regs->ebx, regs->ebx);
    printf("\t%s=%x=%d\n", "ecx", regs->ecx, regs->ecx);
    printf("\t%s=%x=%d\n", "edx", regs->edx, regs->edx);
    printf("\t%s=%x=%d\n", "esi", regs->esi, regs->esi);
    printf("\t%s=%x=%d\n", "edi", regs->edi, regs->edi);
    printf("\t%s=%x=%d\n", "esp", regs->esp, regs->esp);
    printf("\t%s=%x=%d\n", "ebp", regs->ebp, regs->ebp);
    printf("\t%s=%x=%d\n", "eip", regs->eip, regs->eip);
    printf("\t%s=%x=%d\n", "eflags", regs->eflags, regs->eflags);
    printf("\t%s=%x=%d\n", "cr3", regs->cr3, regs->cr3);
}


void _print_stack_trace(void *ebp, void *eip)
{
    int i = 0;
    char *ptr = NULL;

    printf("Stack trace\n");
    printf("\t%p %s\n", eip, resolve_symbol(eip));

    while (ebp)
    {
        eip = (void*)((uint32*)ebp)[1];
        ptr = resolve_symbol(eip);
        if (ptr == NULL)
            break;
        printf("\t%p %s\n", eip, ptr);
        ebp = (void*)*(uint32*)ebp;
    }
    printf("\t(End of stack)\n");
}
