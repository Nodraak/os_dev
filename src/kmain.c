
#include "types.h"
#include "screen.h"

void kmain(void)
{
    char str[14] = "hello world !";

    screen_write_str(0, 0, str, WHITE, BLACK);
    screen_move_cursor(1, 1);

    screen_write_char(2, 0, '0'+sizeof(uint8), WHITE, BLACK);
    screen_write_char(2, 1, '0'+sizeof(uint16), WHITE, BLACK);
    screen_write_char(2, 2, '0'+sizeof(uint32), WHITE, BLACK);
    screen_write_char(2, 3, '0'+sizeof(uint64), WHITE, BLACK);
}
