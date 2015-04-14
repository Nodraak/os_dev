
#include "types.h"
#include "screen.h"

/*
    Bit     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
    Content |         ASCII         |    FG   |    BG   |
*/
void screen_write_char(uint8 j, uint8 i, char c, uint8 fg, uint8 bg)
{
    char *screen = (char*)(SCREEN_ADDR + 2*j*SCREEN_WIDTH + 2*i);
    screen[0] = c;
    screen[1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void screen_write_str(uint8 j, uint8 i, char *s, uint8 fg, uint8 bg)
{
    int k;

    for (k = 0; s[k] != NULL; ++k)
        screen_write_char(j, i+k, s[k], fg, bg);
}

void outb(unsigned short port, unsigned char data);

void screen_move_cursor(uint8 j, uint8 i)
{
    uint16 pos = j*SCREEN_WIDTH + i;

    outb(0x3D4, 14);
    outb(0x3D5, ((pos >> 8) & 0x00FF));
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0x00FF);
}
