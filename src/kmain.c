
#include "pic.h"
#include "printf.h"
#include "buffer.h"
#include "screen.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "shell.h"
#include "paging_low.h"

s_buffer buffer_system_stdin;
s_buffer buffer_system_stdout;
s_buffer buffer_system_serial;

#include "multiboot.h"

void kinit(multiboot_info_t *mbi)
{
    screen_init();
    serial_init();
    printf("Installing GDT and IDT ...");
    gdt_init();
    idt_init();
    printf(" ok\n");
    pic_remap();
    pic_irq_install_kbd();
    paging_low_init(mbi);

    buffer_init(&buffer_system_stdin);
    buffer_init(&buffer_system_stdout);
    buffer_init(&buffer_system_serial);
}

void kpannic(char *msg)
{
    printf("\n**********************\n");
    printf("=> Kernel pannic !! <=\n");
    printf("Reason : %s\n", msg);
    printf("\n**********************\n");

    for (;;)
        ;
}

void kmain(void)
{
    printf("\nGreetings from kmain() !\n");

    /* done */
    printf("\nOS loaded !\n");

    for (;;)
    {
        printf("New shell spawning ...\n");
        shell();
    }
}
