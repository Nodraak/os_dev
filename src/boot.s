[BITS 32]

global loader

extern gdt_init
extern idt_init
extern pic_remap
extern kmain
extern screen_init

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER

KERNEL_STACK_SIZE equ 4096

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .bss
    align 4

kernel_stack:
    resb KERNEL_STACK_SIZE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text:
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    cli
    call screen_init
    call gdt_init
    call idt_init
    call pic_remap
    sti
; KMAIN
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    call kmain
    jmp $
