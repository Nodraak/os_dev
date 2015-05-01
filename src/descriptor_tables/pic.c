
#include "pic.h"
#include "types.h"
#include "printf.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"

void pic_interrupt_handler(s_regs *regs)
{
    printf("-> Received interrupt %x\n", regs->which_int);
}

void pic_irq_install_kbd(void)
{
    s_vector v;
    printf("Installing keyboard interrupt handler ...");

    v.eip = (uint32)kb_irq_handler;
    v.access_byte = 0x8E; /* present, ring 0, '386 interrupt gate */
    setvect(&v, 0x21);
    pic_irq_enable(IRQ_ID_KEYBOARD);

    printf(" ok\n");
}

void pic_io_wait(void)
{
    /* TODO This is probably fragile. */
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
        outb(PIC_SLAVE_COMMAND, PIC_ACK);

    outb(PIC_MASTER_COMMAND, PIC_ACK);
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
