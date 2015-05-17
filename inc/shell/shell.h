#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED

typedef struct _s_cmd
{
    char name[1024];
    void(*cmd)(void);
} s_cmd;

void shell_cmd_hostname(void);
void shell_cmd_add(char *name, void(*cmd)(void));

void shell_init(void);
void shell_execute_cmd(char *cmd);
void shell(void);

#endif
