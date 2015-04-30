#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include "types.h"

void outb(uint32 port, uint32 data);
uint32 inb(uint32 port);

#endif
