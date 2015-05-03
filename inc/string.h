#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include "types.h"

uint32 str_len(char *s);
void str_move(char *dest, char *src, uint32 n);
char *str_dup(char *s);
int32 str_cmp(char *s1, char *s2);

#endif
