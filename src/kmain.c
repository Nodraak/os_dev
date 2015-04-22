
#include "pic.h"
#include "printf.h"
#include "buffer.h"

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


void kmain(void)
{
    printf("Greetings from kmain() !\n");

    pic_irq_install_kbd();

    printf("\nOS loaded !\n\n> ");

    for (;;)
    {
        shell();
        printf("New shell spawning ...\n");
    }
}
