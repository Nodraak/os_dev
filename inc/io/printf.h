#ifndef PRINTF_H_INCLUDED
#define PRINTF_H_INCLUDED

#include "types.h"

#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, ARG) (AP = (char*)&ARG + __va_rounded_size(ARG))
#define va_arg(AP, TYPE) (AP += __va_rounded_size(TYPE), *((TYPE *)((AP) - __va_rounded_size(TYPE))))

typedef char* va_list;

void printf(char *format, ...) __attribute__ ((format (printf, 1, 2)));

void sprintf(char *format, char *ap, char *dest, uint32 buf_size);
uint32 sprintf_str(char *dest, char *src);
uint32 sprintf_unsigned_number(char *s, uint32 n, uint32 base, uint8 group_by);
uint32 sprintf_uint(char *s, uint32 n);
uint32 sprintf_int(char *s, int32 n);
uint32 sprintf_hex(char *s, uint32 n);
uint32 sprintf_binary(char *s, uint32 n);
int get_pad_width(int32 n, uint8 base, uint8 group_by);

uint32 m_pow(uint32 n, uint32 p);

#define isdigit(c)  (((c) >= '0') && ((c) <= '9'))
// todo math
#define abs(n)      ((n) > 0 ? (n) : -(n))

#endif
