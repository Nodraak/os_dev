
#include "types.h"
#include "kscreen.h"

void foo(void)
{
    char str[] = "hello world !";
    int i;

    screen_write_str(str);
    screen_write_char('\n');
    screen_write_int((int)&str);
    screen_write_char('\n');

    screen_write_char('0'+sizeof(uint8));
    screen_write_char('0'+sizeof(uint16));
    screen_write_char('0'+sizeof(uint32));
    screen_write_char('\n');

    for (i = 0; i < 10; ++i)
    {
        screen_write_int(i);
        screen_write_char('\n');
    }
}

typedef struct
{
    uint32 access_byte;
    uint32 eip;
} __attribute__((packed)) vector_t;

void getvect(vector_t *v, uint32 vect_num);
void setvect(vector_t *v, uint32 vect_num);

/* the layout of this structure must match the order of registers
pushed and popped by the exception handlers in KSTART.ASM */
typedef struct
{
/* pushed by pusha */
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
/* pushed separately */
    uint32 ds, es, fs, gs;
    uint32 which_int, err_code;
/* pushed by exception. Exception may also push err_code.
user_esp and user_ss are pushed only if a privilege change occurs. */
    uint32 eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed)) regs_t;

void interrupt_handler(regs_t *regs)
{
    char msg1[] = "\thello from interrupt_handler\n";
    screen_write_str(msg1);

    char msg2[] = "\tinterrupt ";
    screen_write_str(msg2);

    screen_write_uhex(regs->which_int);

    screen_write_char('\n');
}

#define INT_ID 0x20

void kmain(s_gdt *gdt)
{
    uint8 *ptr = NULL, i;

    char msg[] = "kmain\n";
    screen_write_str(msg);

    vector_t v;

    char msg2[] = "Installing new int handler\n";
    screen_write_str(msg2);

    v.eip = (uint32)interrupt_handler;
    v.access_byte = 0x8E; /* present, ring 0, '386 interrupt gate */
    setvect(&v, INT_ID);

    char msg3[] = "Trying new int ...\n";
    screen_write_str(msg3);
    __asm__ __volatile__("int %0" : : "i"(INT_ID));

    char msg4[] = "ok ! \\o/\n";
    screen_write_str(msg4);

    screen_write_uint(gdt->size);
    screen_write_char('\n');
    screen_write_uint(gdt->address);
    screen_write_char('\n');

    screen_write_char('\n');
    ptr = (uint8*)gdt;
    ptr -= 32;

    for (i = 0; i < 4; ++i)
    {
        screen_write_uhex(ptr[i*8+0]*256 + ptr[i*8+1]);
        screen_write_char(' ');
        screen_write_uhex(ptr[i*8+2]*256 + ptr[i*8+3]);
        screen_write_char(' ');
        screen_write_uhex(ptr[i*8+4]);
        screen_write_char(' ');
        screen_write_uhex(ptr[i*8+5]);
        screen_write_char(' ');
        screen_write_uhex(ptr[i*8+6]);
        screen_write_char(' ');
        screen_write_uhex(ptr[i*8+7]);

        screen_write_char('\n');
    }
    screen_write_char('\n');

    foo();

    for (;;)
        ;
}
