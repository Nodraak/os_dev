.text

.globl paging_load_and_enable_page_directory
.globl paging_reload_page_directory

paging_load_and_enable_page_directory:
    ;# load page dir addr in cr3
    mov 4(%esp), %eax
    mov %eax, %cr3
    ;# set flag to enable paging
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    ;# return
    ret

paging_reload_page_directory:
    mov %cr3, %eax
    mov %eax, %cr3
    ret
