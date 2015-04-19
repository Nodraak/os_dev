
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

void kmain(s_gdt *gdt)
{
    uint8 *ptr = NULL, i;

    char msg[] = "kmain\n";
    screen_write_str(msg);

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
