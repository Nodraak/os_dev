
#include "lib/types.h"

#include "bitfield.h"


uint8 bitfield_get(uint8 bitfield, uint8 id)
{
    return (bitfield & (1 << id)) != 0; /* for bool */
}

void bitfield_set(uint8 *bitfield, uint8 id, bool value)
{
    *bitfield = (*bitfield & ~(1 << id)) | (value << id);
}
