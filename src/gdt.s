[BITS 32]

global gdt_init
global LINEAR_CODE_SEL
global LINEAR_DATA_SEL

; GDT : stop using bootloader GDT, and load new GDT
gdt_init:
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

section .data

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
