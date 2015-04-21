
#include "pic.h"
#include "printf.h"


void kmain(void)
{
    printf("Greetings from kmain() !\n");

    pic_irq_install_kbd();

    printf("\nOS loaded !\n\n> ");

    for (;;)
        ;
}
