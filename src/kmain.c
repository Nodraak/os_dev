
#include "types.h"
#include "kscreen.h"

void foo(void)
{
    char str[14] = "hello world !";

    screen_write_str(str);
    screen_write_char('\n');
    screen_write_int((int)&str);
    screen_write_char('\n');

    screen_write_char('0'+sizeof(uint8));
    screen_write_char('0'+sizeof(uint16));
    screen_write_char('0'+sizeof(uint32));
    screen_write_char('\n');

    int i;
    for (i = 0; i < 100; ++i)
    {
        screen_write_int(i);
        screen_write_char('\n');
    }
}

void kmain(void)
{
    screen_init();

    foo();

    for (;;)
        ;
}
