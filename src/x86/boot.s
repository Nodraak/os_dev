[BITS 32]

global loader

extern kinit
extern kmain

MBH_MAGIC_NUMBER equ 0x1BADB002
MBH_FLAGS equ 0x00000003
MBH_CHECKSUM equ -(MBH_MAGIC_NUMBER + MBH_FLAGS)

KERNEL_STACK_SIZE equ 4096

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .bss
    align 4

kernel_stack:
    resb KERNEL_STACK_SIZE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .__mbHeader
    align 0x4
    dd MBH_MAGIC_NUMBER
    dd MBH_FLAGS
    dd MBH_CHECKSUM

section .text:

loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    push ebx    ; Push the pointer to the Multiboot information structure
    cli
    call kinit
    sti
; KMAIN
    call kmain
    jmp $
