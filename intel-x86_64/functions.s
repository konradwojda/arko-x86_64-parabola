
;xmm0 - x
;xmm1 - s
;xmm2 - a
;xmm3 - b
;Calculating the next x for put a dot in
;xmm0 - return
section .text

global x


x:
    addsd xmm2, xmm2    ;xmm1 = 2a
    mulsd xmm2, xmm0    ;xmm1 = 2ax
    addsd xmm2, xmm3    ;xmm1 = 2ax+b
    mulsd xmm2, xmm2    ;xmm1 = (2ax+b)^2

    mov rax, __float64__(1.0)
    movq xmm3, rax

    addsd xmm2, xmm3    ;xmm1 = (2ax+b)^2 + 1
    sqrtsd xmm2, xmm2   ;xmm1 = sqrt((2ax+b)^2 + 1))
    divsd xmm1, xmm2    ;xmm3 = s / xmm1
    addsd xmm0, xmm1    ;xmm0 = new x !
    ret

;xmm0 - a
;xmm1 - b
;xmm2 - c
;xmm3 - x
;Calculating the valuve of function
;xmm0 - return
global y

y:
    mulsd xmm0, xmm3
    mulsd xmm0, xmm3
    mulsd xmm1, xmm3
    addsd xmm0, xmm1
    addsd xmm0, xmm2
    ret

;rdi - bitmap pointer
;rsi - x
;rdx - y
;Set pixel of x,y to black
;Returns 1 if operation ended succesfully,
;Returns 0 if not
global set_pixel

set_pixel:
    xor rax, rax
    movsx rbx, dword[rdi+18]   ;move width of img to rbx
    movsx rcx, dword[rdi+22]    ;move height of img to rcx
    cmp rsi, rbx
    ja exit
    cmp rdx, rcx
    ja exit

    lea rbx, [rbx + rbx*2]  ;rbx *= 3
    add rbx, 3
    and rbx, 0xFFFFFFFFFFFFFFFC
    imul rbx, rdx

    lea rsi, [rsi + rsi*2]
    add rbx, rsi
    add rbx, rdi
    add rbx, 54

    mov word[rbx], 0x00
    mov byte[rbx+2], 0x00
    mov rax, 1
exit:
    ret