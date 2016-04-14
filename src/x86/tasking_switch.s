[BITS 32]

extern task_running

global tasking_read_flags
global tasking_switch

tasking_read_flags:
    pushf
    pop eax
    ret

; Regs saved/restored (cf task.h): eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3
tasking_switch:
    push ebp
    mov ebp, esp

    ; save all regs to the old thread data struct
    push eax ; save eax
    mov eax, [task_running]
    mov [eax+4], ebx
    mov [eax+8], ecx
    mov [eax+12], edx
    mov [eax+16], esi
    mov [eax+20], edi
    mov [eax+24], ebp ; esp
    mov ebx, [ebp] ; ebp
    mov [eax+28], ebx
    mov ebx, [ebp+4] ; eip
    mov [eax+32], ebx
    pushf ; flags
    pop ebx
    mov [eax+36], ebx
    mov ebx, cr3 ; cr3
    mov [eax+40], ebx
    pop ebx ; get eax
    mov [eax+0], ebx

    ; get the next task
    mov eax, [task_running]
    mov eax, [eax+44]
    mov [task_running], eax

    ; load all regs from the new thread data struct
    mov eax, [task_running]
    mov ecx, [eax+8]
    mov edx, [eax+12]
    mov esi, [eax+16]
    mov edi, [eax+20]
    mov esp, [eax+24] ; esp
    mov ebp, [eax+28] ; ebp
    mov ebx, [eax+36] ; eflags - changes stack
    push ebx
    popf
    mov ebx, [eax+40] ; cr3
    mov cr3, ebx
    mov ebx, [eax+32] ; eip - require intact stack
    mov [esp-4], ebx
    ; eax and ebx
    mov ebx, [eax+4]
    mov eax, [eax+0]

    jmp [esp-4]
