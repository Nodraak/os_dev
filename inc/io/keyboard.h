#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "types.h"


void kb_irq_handler(void);
uint32 kb_convert(uint32 code);

#endif
