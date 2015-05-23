#ifndef BITFIELD_H_INCLUDED
#define BITFIELD_H_INCLUDED

#include "types.h"

uint8 bitfield_get(uint8 bitfield, uint8 id);
void bitfield_set(uint8 *bitfield, uint8 id, bool value);

#endif
