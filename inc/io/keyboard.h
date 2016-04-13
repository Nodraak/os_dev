#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "lib/types.h"

#define KEY_BACKSPACE       0x0E

void kb_int_handler_install(void);
void kb_int_handler(void);
uint32 kb_convert(uint32 code);

#endif
