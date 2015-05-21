
#include "kmain.h"

#include "multiboot.h"
#include "printf.h"
#include "screen.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "paging.h"
#include "timer.h"
#include "buffer.h"
#include "shell.h"
#include "task.h"

s_kdata kdata;


extern uint8 ld_kernel_start;
extern uint8 ld_kernel_end;

void kinit(multiboot_info_t *mbi)
{
    kdata.kernel_start = &ld_kernel_start;
    kdata.kernel_end = &ld_kernel_end;

    /* basic */
    screen_init();
    serial_init();

    /* interrupts */
    printf("Installing GDT and IDT ...");
    gdt_init();
    idt_init();
    printf(" ok\n");
    pic_remap();
    kb_int_handler_install();

    /* Memory */
    page_frame_init(mbi);

    /* misc */
    timer_init();
    buffer_init(&kdata.buffer_stdin);

    tasking_init();
}

void kpannic(char *msg)
{
    printf("\n**********************\n");
    printf("=> Kernel pannic !! <=\n");
    printf("Reason : %s\n", msg);
    printf("**********************\n");

    for (;;)
        ;
}

void kmain(void)
{
    printf("OS loaded !\nGreetings from kmain() !\n");

    for (;;)
    {
        printf("Spawning new shell ...\n");
        shell_init();
        shell();
    }
}
