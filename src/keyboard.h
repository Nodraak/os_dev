#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "types.h"

#define IRQ_ID_KEYBOARD     0x01

void kb_irq_handler(void);
uint32 kb_convert(uint32 code);

#endif
