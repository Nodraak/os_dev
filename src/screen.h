#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   25
#define SCREEN_ADDR     0x000B8000
#define NULL            0

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHT_GREY      7
#define DARK_GREY       8
#define LIGHT_BLUE      9
#define LIGHT_GREEN     10
#define LIGHT_CYAN      11
#define LIGHT_RED       12
#define LIGHT_MAGENTA   13
#define LIGHT_BROWN     14
#define WHITE           15

void outb(uint32 port, uint32 data);
void _screen_write_char(uint8 j, uint8 i, char c, uint8 fg, uint8 bg);
void _screen_move_cursor(uint8 j, uint8 i);

void screen_write_char(char c);
void screen_write_str(char *s);
void screen_write_uint(uint32 n);
void screen_write_int(int32 n);

uint32 m_pow(uint32 n, uint32 p);

#endif