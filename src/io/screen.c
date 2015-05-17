
#include "screen.h"
#include "types.h"
#include "serial.h"
#include "io.h"
#include "kmain.h"


void screen_init(void)
{
    uint8 i, j;

    screen_move_cursor(0, 0);
    kdata.screen_fg = COLOR_BLACK;
    kdata.screen_bg = COLOR_LIGHT_GREY;
    kdata.screen_ptr = (uint16 *)SCREEN_ADDR;

    for (j = 0; j < SCREEN_HEIGHT-1; ++j)
    {
        for (i = 0; i < SCREEN_WIDTH; ++i)
            _screen_write_char(j, i, ' ', kdata.screen_fg, kdata.screen_bg);
    }

    for (i = 0; i < 16; ++i)
        _screen_write_char(0, i, ' ', i, kdata.screen_fg);
    screen_write_str("\nScreen configured\n");
}

/*
    Bit     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
    Content |         ASCII         |    FG   |    BG   |
*/
uint16 _screen_make_char(char c, uint8 fg, uint8 bg)
{
    return ((fg & 0x0F) << 12) | ((bg & 0x0F) << 8) | (c & 0xFF);
}

void _screen_write_char(uint8 j, uint8 i, char c, uint8 fg, uint8 bg)
{
    kdata.screen_ptr[j*SCREEN_WIDTH + i] = _screen_make_char(c, fg, bg);
}

void _screen_scroll_up(void)
{
    uint8 i, j;

    for (j = 0; j < SCREEN_HEIGHT-1; ++j)
    {
        for (i = 0; i < SCREEN_WIDTH; ++i)
            kdata.screen_ptr[j*SCREEN_WIDTH + i] = kdata.screen_ptr[(j+1)*SCREEN_WIDTH + i];
    }

    for (i = 0; i < SCREEN_WIDTH; ++i)
        _screen_write_char(SCREEN_HEIGHT-1, i, ' ', COLOR_BLACK, COLOR_LIGHT_GREY);
}

void screen_move_cursor(uint8 j, uint8 i)
{
    uint16 pos = j*SCREEN_WIDTH + i;

    outb(0x3D4, 14);
    outb(0x3D5, ((pos >> 8) & 0x00FF));
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0x00FF);

    kdata.screen_y = j, kdata.screen_x = i;
}

uint16 screen_get_cursor(void)
{
    return ((kdata.screen_y << 8) | kdata.screen_x);
}

void screen_write_char(char c)
{
    if (c == '\n')
    {
        kdata.screen_x = 0;
        kdata.screen_y ++;
    }
    else if (c == '\t')
    {
        kdata.screen_x += 4 - (kdata.screen_x % 4);
    }
    else if (c == '\r')
    {
        kdata.screen_x = 0;
    }
    else
    {
        _screen_write_char(kdata.screen_y, kdata.screen_x, c, kdata.screen_fg, kdata.screen_bg);

        kdata.screen_x ++;

        if (kdata.screen_x == SCREEN_WIDTH)
        {
            kdata.screen_x = 0;
            kdata.screen_y ++;
        }
    }

    if (kdata.screen_y == SCREEN_HEIGHT)
    {
        _screen_scroll_up();
        kdata.screen_y --;
    }

    screen_move_cursor(kdata.screen_y, kdata.screen_x);

    serial_write_char(c);
}

void screen_write_str(char *s)
{
    uint32 i;

    for (i = 0; s[i] != '\0'; ++i)
        screen_write_char(s[i]);
}
