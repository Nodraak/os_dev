
#include "types.h"
#include "screen.h"

void kmain(void)
{
    char str[14] = "hello world !";

    screen_write_str(str);

    screen_write_char('0'+sizeof(uint8));
    screen_write_char('0'+sizeof(uint16));
    screen_write_char('0'+sizeof(uint32));

    screen_write_int(-42);
}
