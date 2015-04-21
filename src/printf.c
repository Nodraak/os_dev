
#include "printf.h"
#include "screen.h"

void printf(char *format, ...)
{
    char *cur;
    va_list ap;

    va_start(ap, format);

    cur = format;
    while (*cur != '\0')
    {
        if (*cur == '%')
        {
            cur ++;
            switch (*cur)
            {
                case '%':
                    screen_write_char('%');
                    break;
                case 'c':
                    screen_write_char(va_arg(ap, char));
                    break;
                case 'd':
                    screen_write_int(va_arg(ap, int));
                    break;
                case 's':
                    screen_write_str(va_arg(ap, char*));
                    break;
                case 'x':
                    screen_write_uhex(va_arg(ap, int));
                    break;

                default:
                    screen_write_str("<error for mod ");
                    screen_write_char(*cur);
                    screen_write_str(">");
                    break;
            }
        }
        else if (*cur == '\\')
        {
            cur ++;
            screen_write_char(*cur);
        }
        else
        {
            screen_write_char(*cur);
        }
        cur ++;
    }
}
