#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#include "types.h"

extern uint16 serial_port_com1;

void serial_init(void);

int serial_received(void);
char serial_read(void);

int serial_can_write(void);
void serial_write_char(char c);
void serial_write_str(char *s);

#endif
