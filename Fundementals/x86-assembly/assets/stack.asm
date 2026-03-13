section .text
global _start

_start:
    mov rax, 10
    mov rbx, 20

    push rax      ; push 10 onto stack
    push rbx      ; push 20 onto stack

    pop rcx       ; rcx = 20
    pop rdx       ; rdx = 10

    mov rax, 60
    xor rdi, rdi
    syscall