
#include "string.h"
#include "types.h"


uint32 str_len(char *s)
{
    uint32 len = 0;

    while (s[len] != '\0')
        len ++;
    len ++; /* for '\0' */

    return len;
}

/*
    The  memmove()  function copies n bytes from memory area src to memory area
    dest.  The memory areas may overlap: copying  takes  place  as  though  the
    bytes  in src are first copied into a temporary array that does not overlap
    src or dest, and the bytes are then copied  from  the  temporary  array  to
    dest.
*/
void str_move(char *dest, char *src, uint32 n)
{
    char *buf = NULL;
    uint32 len, i;

    /* buffer */
    len = n + 1; /* for '\0' */
    buf = __builtin_alloca(len);

    /* copy src to buf */
    for (i = 0; i < len; ++i)
        buf[i] = src[i];

    /* copy buf to dest */
    for (i = 0; i < len; ++i)
        dest[i] = buf[i];

    /* todo free buf */
}

/*
    The  memcpy()  function  copies n bytes from memory area src to memory area
   dest.  The memory areas must not overlap.  Use  memmove(3)  if  the  memory
   areas do overlap.
*/
/* todo memcpy */

char *str_dup(char *s)
{
    char *ret = NULL;
    uint32 len = 0;

    /* buffer */
    len = str_len(s) + 1; /* for '\0' */
    ret = __builtin_alloca(len);

    /* copy */
    str_move(ret, s, len);

    return ret;
}
