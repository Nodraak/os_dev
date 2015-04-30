
#include "pic.h"
#include "printf.h"
#include "buffer.h"
#include "screen.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"

s_buffer buffer_system;
s_buffer buffer_shell;

void shell(void)
{
    uint8 quit = 0;

    while (!quit)
    {
        int16 ret = buffer_pop_char(&buffer_system); /* todo getch */

        if (ret != EOF && ret != '\n')
        {
            buffer_push_char(&buffer_shell, ret); /* todo local buffer */
            printf("%c", ret); /* todo : if echo */
//            printf("%x-%c\n", scancode, c);
        }
        else if (ret == '\n')
        {
            char *cmd = buffer_pop_str(&buffer_shell);
    //        shell_execute_cmd(cmd)

            printf("\n\"%s\" : command not found.\n", cmd);
            printf("> "); /* todo : prompt ps1 */
        }

        // todo sleep
    }
}


void kinit(void)
{
    screen_init();
    printf("Installing GDT and IDT ...");
    gdt_init();
    idt_init();
    printf(" ok\n");
    pic_remap();
    serial_init();
    pic_irq_install_kbd();
}

void kmain(void)
{
    printf("Greetings from kmain() !\n");

    uint16 *ptr = (uint16*)0x0400;
    printf("com1 %x\n", *ptr);
    printf("com2 %x\n", *(ptr+1));
    printf("com3 %x\n", *(ptr+2));
    printf("com4 %x\n", *(ptr+3));

    printf("\nOS loaded !\n\n> ");

    for (;;)
    {
        shell();
        printf("New shell spawning ...\n");
    }
}
