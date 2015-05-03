
#include "bitfield.h"
#include "types.h"

uint8 bitfield_get(uint32 bitfield, uint8 id)
{
    return (bitfield & (1 << id)) != 0; /* for bool */
}

void bitfield_set(uint32 *bitfield, uint8 id, uint8 value)
{
    *bitfield = (*bitfield & ~(1 << id)) | (value << id);
}
