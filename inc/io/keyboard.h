#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "types.h"


void kb_int_handler_install(void);
void kb_int_handler(void);
uint32 kb_convert(uint32 code);

#endif
