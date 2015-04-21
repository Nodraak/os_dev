#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include "types.h"

#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   25
#define SCREEN_ADDR     0xB8000
#define NULL            0

#define COLOR_BLACK           0
#define COLOR_BLUE            1
#define COLOR_GREEN           2
#define COLOR_CYAN            3
#define COLOR_RED             4
#define COLOR_MAGENTA         5
#define COLOR_BROWN           6
#define COLOR_LIGHT_GREY      7
#define COLOR_DARK_GREY       8
#define COLOR_LIGHT_BLUE      9
#define COLOR_LIGHT_GREEN     10
#define COLOR_LIGHT_CYAN      11
#define COLOR_LIGHT_RED       12
#define COLOR_LIGHT_MAGENTA   13
#define COLOR_LIGHT_BROWN     14
#define COLOR_WHITE           15

void screen_init(void);

uint16 _screen_make_char(char c, uint8 fg, uint8 bg);
void _screen_write_char(uint8 j, uint8 i, char c, uint8 fg, uint8 bg);
void _screen_move_cursor(uint8 j, uint8 i);
void _screen_scroll_up(void);

void screen_write_char(char c);
void screen_write_str(char *s);

#endif
