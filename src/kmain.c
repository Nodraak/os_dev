
#include "pic.h"
#include "printf.h"
#include "buffer.h"
#include "screen.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"

s_buffer buffer_system_stdin;
s_buffer buffer_system_stdout;
s_buffer buffer_system_serial;
s_buffer buffer_shell;

void shell(void)
{
    uint8 quit = 0;

    printf("> ");

    while (!quit)
    {
        int16 ret = buffer_pop_char(&buffer_system_stdin); /* todo getch */

        if (ret != EOF && ret != '\n')
        {
            buffer_push_char(&buffer_shell, ret);
            printf("%c", ret); /* todo : if echo */
        }
        else if (ret == '\n')
        {
            char cmd[1024];

            buffer_pop_str(&buffer_shell, cmd);
    //        shell_execute_cmd(cmd)

            printf("\n\"%s\" : command not found.\n", cmd);
            printf("> "); /* todo : prompt ps1 */
        }

        // todo sleep
    }
}

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
    buffer_init(&buffer_shell);
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
