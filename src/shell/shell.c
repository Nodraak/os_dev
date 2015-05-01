
#include "shell.h"
#include "types.h"
#include "buffer.h"
#include "printf.h"

extern s_buffer buffer_system_stdin;

void shell(void)
{
    uint8 quit = 0;
    s_buffer buffer_shell;

    buffer_init(&buffer_shell);
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
            /* todo : shell_execute_cmd(cmd) */

            printf("\n\"%s\" : command not found.\n", cmd);
            printf("> "); /* todo : prompt ps1 */
        }

        /* todo sleep */
    }
}
