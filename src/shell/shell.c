
#include "io/buffer.h"
#include "io/keyboard.h"
#include "kmain.h"
#include "lib/printf.h"
#include "lib/string.h"
#include "lib/types.h"

#include "shell/shell.h"


s_cmd cmd_data[3];
uint8 cmd_nb;

void shell_cmd_help(void)
{
    uint8 i;

    printf("Available commands :\n");

    for (i = 0; i < cmd_nb; ++i)
        printf("\t%s\n", cmd_data[i].name);
}

void shell_cmd_hostname(void)
{
    printf("os_dev\n");
}

void shell_cmd_add(char *name, void(*cmd)(void))
{
    str_move(cmd_data[cmd_nb].name, name, str_len(name));
    cmd_data[cmd_nb].cmd = cmd;

    cmd_nb ++;
}

void shell_init(void)
{
    shell_cmd_add("help", shell_cmd_help);
    shell_cmd_add("hostname", shell_cmd_hostname);
}

void shell_execute_cmd(char *cmd)
{
    uint8 i;

    if (cmd[0] == '\0')
        return;

    for (i = 0; i < cmd_nb; ++i)
    {
        if (str_cmp(cmd, cmd_data[i].name) == 0)
        {
            cmd_data[i].cmd();
            return;
        }
    }

    if (i == cmd_nb)
        printf("\"%s\" : command not found, try \"help\".\n", cmd);
}

void shell(void)
{
    s_buffer buffer_shell;
    char ps1[1024] = "> ";

    buffer_init(&buffer_shell);
    printf("%s", ps1);

    for (;;)
    {
        int16 ret = buffer_pop_char(&kdata.buffer_stdin);

        if (ret != EOF)
        {
            if (ret == KEY_BACKSPACE)
            {
                char tmp[1024];
                buffer_backspace_char(&buffer_shell);
                buffer_copy_str(&buffer_shell, tmp);
                printf("\r%s%s ", ps1, tmp);
                printf("\r%s%s", ps1, tmp);
            }
            else
            {
                buffer_push_char(&buffer_shell, ret);
                printf("%c", ret);

                if (ret == '\n')
                {
                    char cmd[1024];

                    buffer_pop_str(&buffer_shell, cmd);
                    shell_execute_cmd(cmd);
                    printf("%s", ps1);
                }
            }
        }

        /* todo sleep */
    }
}
