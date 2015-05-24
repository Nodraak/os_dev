[BITS 32]

global paging_read_cr0
global paging_write_cr0
global paging_read_cr3
global paging_write_cr3
global paging_reload_page_directory

; cr0 : flags. The big 31 enable paging
; cr3 : address of page directory

paging_read_cr0:
    mov eax, cr0
    ret

paging_write_cr0:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr0,  eax
    pop ebp
    ret

paging_read_cr3:
    mov eax, cr3
    ret

paging_write_cr3:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
    ret

paging_reload_page_directory:
    mov eax, cr3
    mov cr3, eax
    ret
