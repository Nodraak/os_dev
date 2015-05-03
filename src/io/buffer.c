
#include "buffer.h"
#include "types.h"


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

    for (i = 0; i < b->len; ++i)
        b->data[i] = b->data[i+1];

    b->len --;

    return ret;
}

void buffer_pop_str(s_buffer *b, char *buffer)
{
    uint32 i = 0;

    if (b->len == 0)
    {
        buffer[0] = '\0';
        return;
    }

    for (;;)
    {
        char cur = buffer_pop_char(b);
        if (cur == EOF || cur == '\0' || cur == '\n')
            break;

        buffer[i] = cur;
        i ++;
    }

    buffer[i] = '\0';
}
