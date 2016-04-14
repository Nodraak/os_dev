
extern _print_stack_trace

global magic_breakpoint
global print_stack_trace

magic_breakpoint:
    xchg bx, bx
    ret

print_stack_trace:
    push ebp
    mov ebp, esp

    mov eax, [ebp+4]
    push eax
    mov eax, [ebp]
    push eax
    call _print_stack_trace
    add esp, 0x8

    leave ; leave == mov esp, ebp + pop ebp
    ret
