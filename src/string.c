
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

/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2.  */
int32 str_cmp(char *s1, char *s2)
{
    while (*s1 == *s2)
    {
        if (*s1 == '\0')
            return *s1 - *s2;

        s1 ++, s2 ++;
    }

    return *s1 - *s2;
}

/* Fills memory pointed by *ptr*, setting the 32 bits wide *data* *count* times
*/
void str_memset32(uint32 *ptr, uint32 count, uint32 data)
{
    uint32 i = 0;

    for (i = 0; i < count; ++i)
        ptr[i] = data;
}
