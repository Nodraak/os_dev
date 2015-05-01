
#include "printf.h"
#include "types.h"
#include "screen.h"
#include "buffer.h"


void printf(char *format, ...)
{
    va_list args;
    char buffer[1024];

    va_start(args, format);
    sprintf(format, args, buffer);

    screen_write_str(buffer);
}


void sprintf(char *format, char *ap, char *buf)
{
    char *cur = format;
    uint32 i = 0;

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
                    i += sprintf_int(&buf[i], va_arg(ap, int32));
                    break;
                case 'u':
                    i += sprintf_uint(&buf[i], va_arg(ap, uint32));
                    break;
                case 's':
                    i += sprintf_str(&buf[i], va_arg(ap, char*));
                    break;
                case 'x':
                    i += sprintf_hex(&buf[i], va_arg(ap, int32));
                    break;
                case 'p':
                    i += sprintf_hex(&buf[i], va_arg(ap, uint32)); /* sizeof(void*) == sizeof(int) */
                    break;
                case 'b':
                    i += sprintf_binary(&buf[i], va_arg(ap, uint32));
                    break;

                default:
                    i += sprintf_str(&buf[i], "<error sprintf ");
                    buf[i] = *cur;
                    i++;
                    i += sprintf_str(&buf[i], ">");
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
}

uint32 sprintf_str(char *dest, char *src)
{
    uint32 i;

    for (i = 0; src[i] != '\0'; ++i)
        dest[i] = src[i];

    dest[i] = '\0';
    return i;
}

uint32 sprintf_unsigned_number(char *s, uint32 n, uint32 base)
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

        if (i != 0 && i % 3 == 0)
        {
            s[cur_char] = ' ';
            cur_char ++;
        }
    }

    s[cur_char] = '\0';
    return cur_char;
}

uint32 sprintf_uint(char *s, uint32 n)
{
    return sprintf_unsigned_number(s, n, 10);
}

uint32 sprintf_int(char *s, int32 n)
{
    if (n < 0)
    {
        s[0] = '-';
        return 1 + sprintf_uint(s+1, -n);
    }
    else
    {
        return sprintf_uint(s, n);
    }
}

uint32 sprintf_hex(char *s, uint32 n)
{
    s[0] = '0';
    s[1] = 'x';
    return 2 + sprintf_unsigned_number(s+2, n, 16);
}

uint32 sprintf_binary(char *s, uint32 n)
{
    return sprintf_unsigned_number(s, n, 2);
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
