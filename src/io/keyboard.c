
#include "keyboard.h"
#include "io.h"
#include "printf.h"
#include "pic.h"
#include "buffer.h"
#include "kmain.h"

void kb_irq_handler(void)
{
    uint32 scancode, c;

    scancode = inb(0x60);

    if ((scancode & 0x80) == 0) /* key down */
    {
        c = kb_convert(scancode & 0x7F);

        if (c != 0)
            buffer_push_char(&kdata.buffer_stdin, c);
    }

    pic_ack(IRQ_ID_KEYBOARD);
}


uint32 kb_convert(uint32 code)
{
    uint32 ret;
    uint32 kbd_map[] = {
/* 0x00 */
        0,      0,      '1',    '2',    '3',    '4',    '5',    '6',
        '7',    '8',    '9',    '0',    '-',    '=',    0,      '\t',
/* 0x10 */
        'q',    'w',    'e',    'r',    't',    'y',    'u',    'i',
        'o',    'p',    '[',    ']',    '\n',   0,      'a',    's',
/* 0x20 */
        'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';',
        '\'',   '`',    0,      '\\',   'z',    'x',    'c',    'v',
/* 0x30 */
        'b',    'n',    'm',    ',',    '.',    '/',    0,      0,
    };

    /* ignore invalid scan codes */
    if (code >= sizeof(kbd_map) / sizeof(kbd_map[0]))
        goto key_invalid;

    ret = kbd_map[code];

    if (ret == 0)
        goto key_invalid;

    return ret;

    key_invalid:
        printf("<%x-%d>", code, code);
        return 0;
}
