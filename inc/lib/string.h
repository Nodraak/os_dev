#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include "lib/types.h"

uint32 str_len(char *s);
void str_move(char *dest, char *src, uint32 n);
char *str_dup(char *s);
int32 str_cmp(char *s1, char *s2);
void str_memset32(uint32 *ptr, uint32 count, uint32 data);

#endif
