
#include "screen.h"
#include "types.h"
#include "serial.h"

uint8 screen_x, screen_y, screen_fg, screen_bg;
uint16 *screen_buffer;

void outb(uint32 port, uint32 data);

void screen_init(void)
{
    uint8 i, j;

    screen_x = 0;
    screen_y = 0;
    screen_fg = COLOR_BLACK;
    screen_bg = COLOR_LIGHT_GREY;
    screen_buffer = (uint16 *)SCREEN_ADDR;

    for (j = 0; j < SCREEN_HEIGHT-1; ++j)
    {
        for (i = 0; i < SCREEN_WIDTH; ++i)
            _screen_write_char(j, i, ' ', screen_fg, screen_bg);
    }

    screen_move_cursor(screen_x, screen_y);

    screen_write_str("Screen configured\n");
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
    screen_buffer[j*SCREEN_WIDTH + i] = _screen_make_char(c, fg, bg);
}

void _screen_scroll_up(void)
{
    uint8 i, j;

    for (j = 0; j < SCREEN_HEIGHT-1; ++j)
    {
        for (i = 0; i < SCREEN_WIDTH; ++i)
            screen_buffer[j*SCREEN_WIDTH + i] = screen_buffer[(j+1)*SCREEN_WIDTH + i];
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

    screen_y = j, screen_x = i;
}

uint16 screen_get_cursor(void)
{
    return ((screen_y << 8) | screen_x);
}

void screen_write_char(char c)
{
    if (c == '\n')
    {
        screen_x = 0;
        screen_y ++;
    }
    else if (c == '\t')
    {
        screen_x += 4-screen_x%4;
    }
    else if (c == '\r')
    {
        screen_x = 0;
    }
    else
    {
        _screen_write_char(screen_y, screen_x, c, screen_fg, screen_bg);

        screen_x ++;

        if (screen_x == SCREEN_WIDTH)
        {
            screen_x = 0;
            screen_y ++;
        }
    }

    if (screen_y == SCREEN_HEIGHT)
    {
        _screen_scroll_up();
        screen_y --;
    }

    screen_move_cursor(screen_y, screen_x);

    serial_write_char(c);
}

void screen_write_str(char *s)
{
    uint32 i;

    for (i = 0; s[i] != '\0'; ++i)
        screen_write_char(s[i]);
}
