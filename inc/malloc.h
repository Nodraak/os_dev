#ifndef MALLOC_H_INCLUDED
#define MALLOC_H_INCLUDED

#include "types.h"

void *malloc(uint32 size);
void free(void *ptr);

#endif
