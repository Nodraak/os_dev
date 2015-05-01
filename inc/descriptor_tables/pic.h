#ifndef PIC_C_H_INCLUDED
#define PIC_C_H_INCLUDED

#include "types.h"
#include "idt.h" /* s_regs */

#define PIC_MASTER_COMMAND  0x20
#define PIC_MASTER_DATA     0x21
#define PIC_SLAVE_COMMAND   0xA0
#define PIC_SLAVE_DATA      0xA1

#define PIC_ACK             0x20

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
    IRQ     Description
    0   Programmable Interrupt Timer Interrupt
    1   Keyboard Interrupt
    2   Cascade (used internally by the two PICs. never raised)
    3   COM2 (if enabled)
    4   COM1 (if enabled)
    5   LPT2 (if enabled)
    6   Floppy Disk
    7   LPT1 / Unreliable "spurious" interrupt (usually)
    8   CMOS real-time clock (if enabled)
    9   Free for peripherals / legacy SCSI / NIC
    10  Free for peripherals / SCSI / NIC
    11  Free for peripherals / SCSI / NIC
    12  PS2 Mouse
    13  FPU / Coprocessor / Inter-processor
    14  Primary ATA Hard Disk
    15  Secondary ATA Hard Disk

    enable IRQ 0 (timer) and IRQ 1 (keyboard) (0x21 master / 0xA1 slave)
*/

#define IRQ_ACCESS_BYTE     0x8E /* present, ring 0, '386 interrupt gate */
#define IRQ_ID_KEYBOARD     0x01
#define IRQ_VECT_KEYBOARD   0x21

void pic_interrupt_handler(s_regs *regs);
void pic_irq_install_kbd(void);

void pic_io_wait(void);
void pic_remap(void);
void pic_ack(uchar irq);
void pic_irq_enable(uint8 irq);
void pic_irq_disable(uint8 irq);

#endif
