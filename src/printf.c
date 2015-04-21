
#include "printf.h"
#include "types.h"
#include "screen.h"


void printf(char *format, ...)
{
    char *cur;
    char buf[BUFFER_SIZE];
    uint32 i = 0;
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
                    buf[i] = '%';
                    i ++;
                    break;
                case 'c':
                    buf[i] = va_arg(ap, char);
                    i ++;
                    break;
                case 'd':
                    i += printf_int(&buf[i], va_arg(ap, int));
                    break;
                case 's':
                    i += printf_str(&buf[i], va_arg(ap, char*));
                    break;
                case 'x':
                    i += printf_hex(&buf[i], va_arg(ap, int));
                    break;

                default:
                    i += printf_str(&buf[i], "<error for mod ");
                    buf[i] = *cur;
                    i++;
                    i += printf_str(&buf[i], ">");
                    break;
            }
        }
        else if (*cur == '\\')
        {
            cur ++;
            buf[i] = *cur;
            i ++;
        }
        else
        {
            buf[i] = *cur;
            i ++;
        }
        cur ++;
    }

    buf[i] = '\0';

    screen_write_str(buf);
}

uint32 printf_str(char *dest, char *src)
{
    int i;

    for (i = 0; src[i] != NULL; ++i)
        dest[i] = src[i];

    return i;
}

uint32 printf_uint(char *s, uint32 n)
{
    return printf_unsigned_number(s, n, 10);
}

uint32 printf_unsigned_number(char *s, uint32 n, uint32 base)
{
    int8 nb_bits = 0, i;
    uint32 tmp = n, cur_char = 0;
    char digits[] = "0123456789abcdef";

    if (n == 0)
    {
        *s = '0';
        return 1;
    }

    while (tmp != 0)
    {
        tmp /= base;
        nb_bits ++;
    }

    for (i = nb_bits-1; i >= 0; --i)
    {
        uint8 cur = (n/m_pow(base, i)) % base;
        s[cur_char] = digits[cur];
        cur_char ++;
    }

    return cur_char;
}

uint32 printf_hex(char *s, uint32 n)
{
    s[0] = '0';
    s[1] = 'x';
    return 2 + printf_unsigned_number(s+2, n, 16);
}

uint32 printf_int(char *s, int32 n)
{
    if (n < 0)
    {
        *s = '-';
        return 1 + printf_uint(s+1, -n);
    }
    else
    {
        return printf_uint(s, n);
    }
}

uint32 m_pow(uint32 n, uint32 p)
{
    uint32 i = 0, ret = 1;

    if (p == 0)
        return 1;

    for (i = 0; i < p; ++i)
        ret *= n;

    return ret;
}
