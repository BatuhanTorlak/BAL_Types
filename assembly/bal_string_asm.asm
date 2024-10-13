[BITS 64]
section .text
global BalMemMove
BalMemMove:
.g0:
    sub r8, 16
    jc .g1
    movdqu xmm0, [rcx+r8]
    movdqu [rdx+r8], xmm0
    jmp .g0
.g1:
    add r8, 16
.g2:
    test r8, r8
    jz .ge
    dec r8
    mov al, [rcx+r8]
    mov [rdx+r8], al
    jmp .g2
.ge:
    ret