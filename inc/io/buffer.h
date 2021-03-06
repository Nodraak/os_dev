#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "lib/types.h"

#define BUFFER_SIZE     1024
#define EOF             (-1)

typedef struct _s_buffer
{
    char data[BUFFER_SIZE];
    uint32 len;
} s_buffer;

void buffer_init(s_buffer *b);
void buffer_push_char(s_buffer *b, char c);
char buffer_pop_char(s_buffer *b);
void buffer_pop_str(s_buffer *b, char *buffer);
void buffer_backspace_char(s_buffer *b);
void buffer_copy_str(s_buffer *b, char *s);

#endif
