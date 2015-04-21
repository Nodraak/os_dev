
#include "types.h"
#include "io.h"
#include "int_s.h"
#include "int_c.h"
#include "screen.h"
#include "keyboard.h"
#include "printf.h"


void kmain(void)
{
    s_vector v;
    printf("Greetings from kmain() !\n");

    screen_write_str("Installing keyboard interrupt handler ...\n");

    v.eip = (uint32)kb_irq_handler;
    v.access_byte = 0x8E; /* present, ring 0, '386 interrupt gate */
    setvect(&v, 0x21);
    pic_irq_enable(IRQ_ID_KEYBOARD);

    printf("\nOS loaded !\n\n> ");

    for (;;)
        ;
}
