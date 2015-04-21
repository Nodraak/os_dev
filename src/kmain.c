
#include "types.h"
#include "io.h"
#include "int_s.h"
#include "int_c.h"
#include "screen.h"
#include "printf.h"


void kmain(void)
{
    printf("Greetings from kmain() !\n");

    irq_install_kbd();

    printf("\nOS loaded !\n\n> ");

    for (;;)
        ;
}
