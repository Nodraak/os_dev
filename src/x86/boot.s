[BITS 32]

global loader

extern kinit
extern kmain

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

section .__mbHeader
    align 0x4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .text:

loader:
    cli
    call kinit
    sti
; KMAIN
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    call kmain
