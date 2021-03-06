
#include "descriptor_tables/gdt.h"
#include "descriptor_tables/idt.h"
#include "descriptor_tables/pic.h"
#include "io/buffer.h"
#include "io/keyboard.h"
#include "io/screen.h"
#include "io/serial.h"
#include "lib/printf.h"
#include "lib/read_elf.c.h"
#include "page_frame.h"
#include "paging.c.h"
#include "shell/shell.h"
#include "task.h"
#include "timer.h"
#include "x86/multiboot.h"

#include "kmain.h"


s_kdata kdata;


extern uint8 ld_kernel_start;
extern uint8 ld_kernel_end;

void kinit(multiboot_info_t *mbi)
{
    kdata.kernel_start = &ld_kernel_start;
    kdata.kernel_end = &ld_kernel_end;
    kdata.kernel_size = (uint32)(kdata.kernel_end-kdata.kernel_start);

    /* basic */
    serial_init();
    screen_init();

    if (!(mbi->flags & (1 << 5)))
        kpanic("no symbol data in os image");
    elf_print_stats(mbi->_.elf_sec);

    /* interrupts */
    printf("Installing GDT and IDT ...");
    gdt_init();
    idt_init();
    printf(" ok\n");
    pic_remap();
    kb_int_handler_install();

    int_handler_install(handler, 0xE); // Page Fault

    /* memory */
    page_frame_init(mbi);
    paging_init();

    /* misc */
    timer_init();
    buffer_init(&kdata.buffer_stdin);

    tasking_init();

    printf("OS loaded !\n\n");
}

void _kpanic(char *msg, const char *file, int line, const char *func)
{
    printf("\n");
    printf("****************************************\n");
    printf("Kernel panic !!\n");
    printf("At %s() in %s:%d\n", func, file, line);
    printf("Reason: %s\n", msg);
    printf("****************************************\n");

    for (;;)
        ;
}

void kmain(void)
{
    printf("Greetings from kmain() !\n");

    for (;;)
    {
        printf("Spawning new shell ...\n");
        shell_init();
        shell();
    }
}
