
#include "buffer.h"
#include "types.h"
#include "string.h"


void buffer_init(s_buffer *b)
{
    b->len = 0;
    b->data[b->len] = '\0';
}

void buffer_push_char(s_buffer *b, char c)
{
    b->data[b->len] = c;
    b->len ++;
    b->data[b->len] = '\0';
}

char buffer_pop_char(s_buffer *b)
{
    char ret;
    uint32 i;

    if (b->len == 0)
        return EOF;

    ret = b->data[0];
    b->len --;

    for (i = 0; i < b->len; ++i)
        b->data[i] = b->data[i+1];

    return ret;
}

char *buffer_pop_str(s_buffer *b)
{
    uint32 len = 0, i;
    char *ret = NULL;

    /* ret str */
    while (b->data[len] != '\n' && b->data[len] != '\0')
        len ++;
    str_move(ret, b->data, len);
    ret[len] = '\0';

    /* move b->data to erase poped data */
    for (; i < b->len-1; ++i)
        b->data[i] = b->data[i+1];

    b->len -= len;

    return ret;
}
