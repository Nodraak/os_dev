
#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   25
#define SCREEN_ADDR     0x000B8000
#define NULL            0

/*
    Bit     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
    Content |         ASCII         |    FG   |    BG   |
*/

/*
    Black 0
    Blue 1
    Green 2
    Cyan 3
    Red 4
    Magenta 5
    Brown 6
    Light grey 7
    Dark grey 8
    Light blue 9
    Light green 10
    Light cyan 11
    Light red 12
    Light magenta 13
    Light brown 14
    White 15
*/

#define BLACK   0
#define WHITE   15

void screen_write_char(unsigned int i, unsigned int j, char c, unsigned char fg, unsigned char bg)
{
    char *screen = (char*)(0x000B8000 + 2*j*SCREEN_WIDTH + 2*i);
    screen[0] = c;
    screen[1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void screen_write_str(unsigned int i, unsigned int j, char *str, unsigned char fg, unsigned char bg)
{
    int k;

    for (k = 0; str[k] != NULL; ++k)
        screen_write_char(i+k, j, str[k], fg, bg);
}

void kmain(void)
{
    char str[14] = "hello world !";

    screen_write_str(0, 0, str, WHITE, BLACK);
}
