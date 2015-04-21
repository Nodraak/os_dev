#ifndef PRINTF_H_INCLUDED
#define PRINTF_H_INCLUDED

void printf(char *format, ...) __attribute__ ((format (printf, 1, 2)));

typedef char* va_list;

#define __va_rounded_size(TYPE) (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(AP, ARG) (AP = (char*)&ARG + __va_rounded_size(ARG))
#define va_arg(AP, TYPE) (AP = ((char *)(AP) + __va_rounded_size(TYPE)), *((TYPE *)((char *)(AP) - __va_rounded_size(TYPE))))

#endif
