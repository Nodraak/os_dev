
#include "serial.h"
#include "io.h"
#include "printf.h"
#include "kmain.h"


void serial_init(void)
{
    uint16 *ptr = (uint16*)0x0400;      /* bios defined addr for serial ports */

    printf("Serial ports :\n");
    printf("\tcom1 com2 com3 com4\n");
    printf("\t%x %x %x %x\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3));
    printf("Installing serial on com1=%x ...", kdata.serial_port_com1);

    kdata.serial_port_com1 = *ptr;
    outb(kdata.serial_port_com1 + 1, 0x00);   /* Disable all interrupts */
    outb(kdata.serial_port_com1 + 3, 0x80);   /* Enable DLAB (set baud rate divisor) */
    outb(kdata.serial_port_com1 + 0, 0x03);   /* Set divisor to 3 (lo byte) 38400 baud */
    outb(kdata.serial_port_com1 + 1, 0x00);   /*                  (hi byte) */
    outb(kdata.serial_port_com1 + 3, 0x03);   /* 8 bits, no parity, one stop bit */
    outb(kdata.serial_port_com1 + 2, 0xC7);   /* Enable FIFO, clear them, with 14-byte threshold */
    outb(kdata.serial_port_com1 + 4, 0x0B);   /* IRQs enabled, RTS/DSR set */

    printf(" ok\n");
}

int serial_received(void)
{
    return inb(kdata.serial_port_com1 + 5) & 1;
}

char serial_read(void)
{
    while (serial_received() == 0)
        ;

    return inb(kdata.serial_port_com1);
}

int serial_can_write(void)
{
   return inb(kdata.serial_port_com1 + 5) & 0x20;
}

void serial_write_char(char c)
{
    if (kdata.serial_port_com1 == 0) /* todo : if not serial_is_initialized */
        return;

    while (serial_can_write() == 0)
        ;

    outb(kdata.serial_port_com1, c);
}

void serial_write_str(char *s)
{
    uint32 i;

    for (i = 0; s[i] != '\0'; ++i)
        serial_write_char(s[i]);
}
