
#include "printf.h"
#include "types.h"
#include "screen.h"
#include "buffer.h"
#include "kmain.h"
#include "string.h"


void printf(char *format, ...)
{
    va_list args;
    char buffer[1024];

    va_start(args, format);
    sprintf(format, args, buffer, 1024);

    screen_write_str(buffer);
}


void sprintf(char *format, char *ap, char *buf, uint32 buf_size)
{
    char *cur = format;
    uint32 i = 0;
    int to_pad = 0, pad_reverse = 0;

    char *stmp = NULL;
    int32 itmp = 0;
    uint32 uitmp = 0;

    while ((*cur != '\0') && (i < buf_size-1))
    {
        if (*cur == '%')
        {
            to_pad = 0;
            cur ++;
mod_switch:
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
                    itmp = va_arg(ap, int32);
                    to_pad -= get_pad_width(abs(itmp), 10, 3);

                    if (pad_reverse)
                    {
                        i += sprintf_int(&buf[i], itmp);
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                    }
                    else
                    {
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                        i += sprintf_int(&buf[i], itmp);
                    }
                    break;
                case 'u':
                    uitmp = va_arg(ap, uint32);
                    to_pad -= get_pad_width(uitmp, 10, 3);

                    if (pad_reverse)
                    {
                        i += sprintf_uint(&buf[i], uitmp);
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                    }
                    else
                    {
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                        i += sprintf_uint(&buf[i], uitmp);
                    }
                    break;
                case 's':
                    stmp = va_arg(ap, char*);
                    to_pad -= str_len(stmp);
                    if (pad_reverse)
                    {
                        i += sprintf_str(&buf[i], stmp);
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                    }
                    else
                    {
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                        i += sprintf_str(&buf[i], stmp);
                    }
                    break;
                case 'x':
                    itmp = va_arg(ap, int32);
                    to_pad -= get_pad_width(abs(itmp), 16, 0);

                    if (pad_reverse)
                    {
                        i += sprintf_hex(&buf[i], itmp);
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                    }
                    else
                    {
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                        i += sprintf_hex(&buf[i], itmp);
                    }
                    break;
                case 'p':
                    uitmp = va_arg(ap, uint32);
                    to_pad -= get_pad_width(uitmp, 16, 0);

                    if (pad_reverse)
                    {
                        i += sprintf_hex(&buf[i], uitmp);
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                    }
                    else
                    {
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                        i += sprintf_hex(&buf[i], uitmp);
                    }
                    break;
                case 'b':
                    uitmp = va_arg(ap, uint32);
                    to_pad -= get_pad_width(uitmp, 10, 4);

                    if (pad_reverse)
                    {
                        i += sprintf_binary(&buf[i], uitmp);
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                    }
                    else
                    {
                        while (to_pad-- > 0)
                            buf[i++] = ' ';
                        i += sprintf_binary(&buf[i], uitmp);
                    }
                    break;

                case '-':
                    pad_reverse = 1;
                    cur ++;
                    goto mod_switch;

                default:
                    if (isdigit(*cur))
                    {
                        while (isdigit(*cur))
                        {
                            to_pad *= 10;
                            to_pad += (*cur) - '0';
                            cur ++;
                        }
                        goto mod_switch;
                    }

                    buf[i] = '?';
                    i++;
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

uint32 sprintf_unsigned_number(char *s, uint32 n, uint32 base, uint8 group_by)
{
    int8 nb_bits = 0, i;
    uint32 tmp = n, cur_char = 0;
    char digits[] = "0123456789ABCDEF";

    if (base == 0 || base > 16)
        kpanic("SPRINTF : invalid base");

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

        if (group_by)
        {
            if (i != 0 && i % group_by == 0)
            {
                s[cur_char] = ' ';
                cur_char ++;
            }
        }
    }

    s[cur_char] = '\0';
    return cur_char;
}

uint32 sprintf_uint(char *s, uint32 n)
{
    return sprintf_unsigned_number(s, n, 10, 3);
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
    return 2 + sprintf_unsigned_number(s+2, n, 16, 0);
}

uint32 sprintf_binary(char *s, uint32 n)
{
    return sprintf_unsigned_number(s, n, 2, 4);
}


// todo math
uint32 m_pow(uint32 n, uint32 p)
{
    uint32 i = 0, ret = 1;

    if (p == 0)
        return 1;

    for (i = 0; i < p; ++i)
        ret *= n;

    return ret;
}

int get_pad_width(int32 n, uint8 base, uint8 group_by)
{
    uint8 ret = 0;

    if (n == 0)
        return 1;

    while (n > 0)
        n /= base, ret ++;

    if (group_by)
        return ret + (ret-1)/group_by;
    else
        return ret;
}
