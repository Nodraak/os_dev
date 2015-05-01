
#include "pic.h"
#include "printf.h"
#include "buffer.h"
#include "screen.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "shell.h"

s_buffer buffer_system_stdin;
s_buffer buffer_system_stdout;
s_buffer buffer_system_serial;

#include "multiboot.h"

void kinit(void)
{
    screen_init();
    serial_init();
    printf("Installing GDT and IDT ...");
    gdt_init();
    idt_init();
    printf(" ok\n");
    pic_remap();
    pic_irq_install_kbd();

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

void kmain(multiboot_info_t *mbi)
{
    printf("\nGreetings from kmain() !\n");

    uint16 *ptr = (uint16*)0x0400;
    printf("Serial ports :\n");
    printf("\tcom1 com2 com3 com4\n");
    printf("\t%x %x %x %x\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3));

    printf("mem_lower = %uKB, mem_upper = %uKB\n", mbi->mem_lower, mbi->mem_upper);

    uint32 RAM_SIZE;
    uint32 PAGE_SIZE;
    uint32 NB_PAGES;

    RAM_SIZE           = (mbi->mem_upper - mbi->mem_lower)*1024;
    PAGE_SIZE          = 4*1024;
    NB_PAGES           = RAM_SIZE/PAGE_SIZE;

    printf("ram_size     %d\n", RAM_SIZE);
    printf("page_size    %d\n", PAGE_SIZE);
    printf("nb_page      %d\n", NB_PAGES);

    printf("\nOS loaded !\n");

    for (;;)
    {
        printf("New shell spawning ...\n");
        shell();
    }
}
