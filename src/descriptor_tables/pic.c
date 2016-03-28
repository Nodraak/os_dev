
#include "pic.h"
#include "types.h"
#include "printf.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"

void pic_io_wait(void)
{
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

/*
    reinitialize the PIC controllers, giving them specified vector offsets
    rather than 8h and 70h, as configured by default
*/
void pic_remap(void)
{
    printf("Remaping pic ...");

    outb(PIC_SLAVE_COMMAND, ICW1_INIT+ICW1_ICW4);   /* starts the initialization sequence (in cascade mode) */
    pic_io_wait();
    outb(PIC_MASTER_COMMAND, ICW1_INIT+ICW1_ICW4);
    pic_io_wait();
    outb(PIC_MASTER_DATA, PIC_MASTER_COMMAND);      /* ICW2: Master PIC vector offset */
    pic_io_wait();
    outb(PIC_SLAVE_DATA, PIC_SLAVE_COMMAND);        /* ICW2: Slave PIC vector offset */
    pic_io_wait();
    outb(PIC_MASTER_DATA, 4);                       /* ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) */
    pic_io_wait();
    outb(PIC_SLAVE_DATA, 2);                        /* ICW3: tell Slave PIC its cascade identity (0000 0010) */
    pic_io_wait();

    outb(PIC_MASTER_DATA, ICW4_8086);
    pic_io_wait();
    outb(PIC_SLAVE_DATA, ICW4_8086);
    pic_io_wait();

    /* disable all IRQs */
    outb(PIC_MASTER_DATA, 0xFF);
    outb(PIC_SLAVE_DATA, 0xFF);

    printf(" ok\n");
}

void pic_ack(uchar irq)
{
    if (irq >= 8)
        outb(PIC_SLAVE_COMMAND, PIC_ACK_DATA);

    outb(PIC_MASTER_COMMAND, PIC_ACK_DATA);
}

void pic_irq_enable(uint8 irq)
{
    if (irq < 8)
    {
        uint8 mask = inb(PIC_MASTER_DATA);
        mask &= ~(1 << irq);
        outb(PIC_MASTER_DATA, mask);
    }
    else
    {
        uint8 mask = inb(PIC_SLAVE_DATA);
        irq -= 8;
        mask &= ~(1 << irq);
        outb(PIC_SLAVE_DATA, mask);
    }
}

void pic_irq_disable(uint8 irq)
{
    if (irq < 8)
    {
        uint8 mask = inb(PIC_MASTER_DATA);
        mask |= ~(1 << irq);
        outb(PIC_MASTER_DATA, mask);
    }
    else
    {
        uint8 mask = inb(PIC_SLAVE_DATA);
        irq -= 8;
        mask |= ~(1 << irq);
        outb(PIC_SLAVE_DATA, mask);
    }
}

void int_handler_install(void(*callback)(void), uint32 irq_vect)
{
    s_vector v;

    v.eip = (uint32)callback;
    v.access_byte = IRQ_ACCESS_BYTE;
    setvect(&v, irq_vect);
}

void pic_int_handler_install(void(*callback)(void), uint32 irq_vect, uint32 irq_id)
{
    int_handler_install(callback, irq_vect);
    pic_irq_enable(irq_id);
}
