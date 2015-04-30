
#include "serial.h"
#include "io.h"

void serial_init(void)
{
    outb(SERIAL_PORT_COM1 + 1, 0x00);    // Disable all interrupts
    outb(SERIAL_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT_COM1 + 1, 0x00);    //                  (hi byte)
    outb(SERIAL_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received(void)
{
    return inb(SERIAL_PORT_COM1 + 5) & 1;
}

char serial_read(void)
{
    while (serial_received() == 0)
        ;

    return inb(SERIAL_PORT_COM1);
}

int serial_can_write(void)
{
   return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial_write_char(char c)
{
    while (serial_can_write() == 0)
        ;

    outb(SERIAL_PORT_COM1, c);
}

void serial_write_str(char *s)
{
    uint32 i;

    for (i = 0; s[i] != '\0'; ++i)
        serial_write_char(s[i]);
}
