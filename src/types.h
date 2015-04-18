#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

typedef unsigned char uchar;

typedef char int8;
typedef short int int16;
typedef int int32;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

typedef struct _s_gdt
{
    uint16 size;
    uint32 address;
} __attribute__((packed)) s_gdt;

#endif
