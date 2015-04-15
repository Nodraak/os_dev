[BITS 32]

global loader
global outb
extern kmain

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER

KERNEL_STACK_SIZE equ 4096

section .bss
    align 4

kernel_stack:
    resb KERNEL_STACK_SIZE

section .text:
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov eax, 0xCAFEBABE
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    call kmain

.loop:
    jmp .loop

; outb - send a byte to an I/O port
; stack :
;   [esp + 8] the data byte
;   [esp + 4] the I/O port
;   [esp] return address
outb:
    mov al, [esp + 8]   ; move the data to be sent into the al register
    mov dx, [esp + 4]   ; move the address of the I/O port into the dx register
    out dx, al          ; send the data to the I/O port
    ret
