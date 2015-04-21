#ifndef PRINTF_H_INCLUDED
#define PRINTF_H_INCLUDED

#include "types.h"

#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, ARG) (AP = (char*)&ARG + __va_rounded_size(ARG))
#define va_arg(AP, TYPE) (AP = ((char *)(AP) + __va_rounded_size(TYPE)), *((TYPE *)((char *)(AP) - __va_rounded_size(TYPE))))

typedef char* va_list;

void printf(char *format, ...) __attribute__ ((format (printf, 1, 2)));

uint32 printf_str(char *dest, char *src);
uint32 printf_uint(char *s, uint32 n);
uint32 printf_unsigned_number(char *s, uint32 n, uint32 base);
uint32 printf_hex(char *s, uint32 n);
uint32 printf_int(char *s, int32 n);

uint32 m_pow(uint32 n, uint32 p);

#endif
