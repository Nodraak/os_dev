#ifndef BITFIELD_H_INCLUDED
#define BITFIELD_H_INCLUDED

#include "types.h"

uint8 bitfield_get(uint32 bitfield, uint8 id);
void bitfield_set(uint32 *bitfield, uint8 id, uint8 value);

#endif
