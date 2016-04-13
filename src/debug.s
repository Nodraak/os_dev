
extern _print_stack_trace

global magic_breakpoint
global print_stack_trace

magic_breakpoint:
    xchg bx, bx
    ret

print_stack_trace:
    push ebp
    call _print_stack_trace
    add esp, 4
    ret
