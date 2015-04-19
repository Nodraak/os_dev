
#include "types.h"
#include "kscreen.h"

typedef struct
{
    uint32 access_byte;
    uint32 eip;
} __attribute__((packed)) vector_t;

void getvect(vector_t *v, uint32 vect_num);
void setvect(vector_t *v, uint32 vect_num);

/* the layout of this structure must match the order of registers
pushed and popped by the exception handlers in KSTART.ASM */
typedef struct
{
/* pushed by pusha */
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
/* pushed separately */
    uint32 ds, es, fs, gs;
    uint32 which_int, err_code;
/* pushed by exception. Exception may also push err_code.
user_esp and user_ss are pushed only if a privilege change occurs. */
    uint32 eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed)) regs_t;

void interrupt_handler(regs_t *regs)
{
    char msg1[] = "\thello from interrupt_handler\n";
    screen_write_str(msg1);

    char msg2[] = "\tinterrupt ";
    screen_write_str(msg2);

    screen_write_uhex(regs->which_int);

    screen_write_char('\n');
}


void outb(uint32 port, uint32 data);
uint32 inb(uint32 port);

#define PIC_MASTER_COMMAND  0x20
#define PIC_MASTER_DATA     0x21
#define PIC_SLAVE_COMMAND   0xA0
#define PIC_SLAVE_DATA      0xA1

#define PIC_ACK             0x20

static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}
/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4       0x01        /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08        /* Level triggered (edge) mode */
#define ICW1_INIT       0x10        /* Initialization - required! */

#define ICW4_8086       0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C        /* Buffered mode/master */
#define ICW4_SFNM       0x10        /* Special fully nested (not) */

/*
arguments:
    offset1 - vector offset for master PIC
        vectors on the master become offset1..offset1+7
    offset2 - same for slave PIC: offset2..offset2+7
*/
void kpic_remap(void)
{
    char msg[] = "kpic_remap\n";
    screen_write_str(msg);

    outb(PIC_SLAVE_COMMAND, ICW1_INIT+ICW1_ICW4);       // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC_MASTER_COMMAND, ICW1_INIT+ICW1_ICW4);
    io_wait();
    outb(PIC_MASTER_DATA, PIC_MASTER_COMMAND);          // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC_SLAVE_DATA, PIC_SLAVE_COMMAND);            // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC_MASTER_DATA, 4);                           // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC_SLAVE_DATA, 2);                            // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(PIC_MASTER_DATA, ICW4_8086);
    io_wait();
    outb(PIC_SLAVE_DATA, ICW4_8086);
    io_wait();

    /* disable all IRQs */
    outb(PIC_MASTER_DATA, 0xFF);
    outb(PIC_SLAVE_DATA, 0xFF);
}

void pic_ack(uchar irq)
{
    if (irq >= 8)
        outb(PIC_SLAVE_COMMAND, PIC_ACK);

    outb(PIC_MASTER_COMMAND, PIC_ACK);
}

#define IRQ_ID_KEYBOARD     0x01

void irq_handler_keyboard(void)
{
    unsigned scan_code, temp;

/* you MUST read port 60h to clear the keyboard interrupt */
    scan_code = inb(0x60);
    screen_write_uhex(scan_code);
    screen_write_char('\n');
    /*
    temp = convert(scan_code);
    if (temp != 0)
        screen_write_char(temp);*/

    pic_ack(IRQ_KEYBOARD);
}

void kmain(void)
{
    char msg[] = "kmain\n";
    screen_write_str(msg);

    vector_t v;

    char msg1[] = "Installing keyboard interrupt handler ...\n";
    screen_write_str(msg1);

    v.eip = (uint32)irq_handler_keyboard;
    v.access_byte = 0x8E; /* present, ring 0, '386 interrupt gate */
    setvect(&v, 0x21);
    outb(0x21, ~(1 << IRQ_KEYBOARD)); /* enable IRQ 1 (keyboard) (0x21 master) */

    char msg3[] = "Type some text\n";
    screen_write_str(msg3);

    for (;;)
        ;
}
