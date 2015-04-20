
#include "types.h"
#include "io.h"
#include "int_s.h"
#include "int_c.h"
#include "screen.h"
#include "keyboard.h"


void kmain(void)
{
    char msg[] = "kmain\n";
    screen_write_str(msg);

    s_vector v;

    char msg1[] = "Installing keyboard interrupt handler ...\n";
    screen_write_str(msg1);

    v.eip = (uint32)kb_irq_handler;
    v.access_byte = 0x8E; /* present, ring 0, '386 interrupt gate */
    setvect(&v, 0x21);
    pic_irq_enable(IRQ_ID_KEYBOARD);

    char msg3[] = "Type some text\n";
    screen_write_str(msg3);

    for (;;)
        ;
}
