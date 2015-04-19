[BITS 32]

global loader

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

; GDT : stop using bootloader GDT, and load new GDT
    lgdt  [gdt_ptr]
    ; Reload data segment registers
    mov ax, LINEAR_DATA_SEL ; new data selector
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    ; Reload CS register (code selector)
    jmp LINEAR_CODE_SEL:flush_cs
flush_cs:

    sti
; KMAIN
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    push gdt_ptr
    call kmain
    jmp $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .data

;
; GDT
;

gdt_start:
; NULL descriptor
    dw 0            ; limit 15:0
    dw 0            ; base 15:0
    db 0            ; base 23:16
    db 0            ; type
    db 0            ; limit 19:16, flags
    db 0            ; base 31:24
; code
LINEAR_CODE_SEL equ $-gdt_start
    dw 0xFFFF
    dw 0
    db 0
    db 0x9A             ; present,ring 0,code,non-conforming,readable
    db 0xCF             ; page-granular (4 gig limit), 32-bit
    db 0
; data
LINEAR_DATA_SEL equ $-gdt_start
    dw 0xFFFF
    dw 0
    db 0
    db 0x92             ; present, ring 0, data, expand-up, writable
    db 0xCF             ; page-granular (4 gig limit), 32-bit
    db 0
gdt_end:

; struct gdt
; {
;     uint16 size;
;     uint32 address;
; } __attribute__((packed))
gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd gdt_start

