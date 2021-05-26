;void draw_bitmap(unsigned char* bitmap, double a, double b, double c, double s);
;rdi - bitmap pointer;
;xmm0 - double a;
;xmm1 - double b;
;xmm2 - double c;
;xmm3 - double s;
section .text
global draw_bitmap

draw_bitmap:
    ;pushing saved registers
    push r12
    push r13
    push r14
    push r15
    push rbx
    movsx rbx, dword[rdi+18]    ;move width of bmp to rbx
    movsx rcx, dword[rdi+22]    ;move height of bmp to rcx
draw_axis:
    mov r8, rbx                 ;move width to r8 to decrement
    mov r9, rcx                 ;move height to r8 to decrement
draw_x:
    mov rdx, 256                ;move y to rdx (512/2)

    ;move width to r11 and calculate index of a pixel
    mov r11, rbx
    lea r11, [r11+r11*2]
    add r11, 3
    and r11, 0xFFFFFFFFFFFFFFFC
    imul r11, rdx

    mov r10, r8                ;move x to r10
    lea r10, [r10+r10*2]
    add r11, r10
    add r11, rdi
    add r11, 54

    ;color pixel
    mov byte[r11], 0xA9
    mov byte[r11+1], 0xA9
    mov byte[r11+2], 0xA9
    dec r8
    jnbe draw_x

draw_y:
    mov rdx, 256    ;move x to rdx(512/2)

    ;move width to r11 and calculate index of a pixel
    mov r11, rbx
    lea r11, [r11+r11*2]
    add r11, 3
    and r11, 0xFFFFFFFFFFFFFFFC
    imul r11, r9

    mov r10, rdx
    lea r10, [r10+r10*2]
    add r11, r10
    add r11, rdi
    add r11, 54

    ;color pixel
    mov byte[r11], 0xA9
    mov byte[r11+1], 0xA9
    mov byte[r11+2], 0xA9

    dec r9
    jnbe draw_y

draw_parabola:
    ;calculate starting point;
    mov r11, __float64__(-0.5)
    movq xmm4, r11
    mulsd xmm4, xmm1    ;xmm4 = -b/2
    divsd xmm4, xmm0    ;xmm4 = -b/2a
    movsd xmm5, xmm4    ;xmm5 = xmm4 - move to save as axis of symmetric
    ;xmm4 - axis of symmetric
    ;xmm5 - first x
    movsd xmm6, xmm5
    mulsd xmm6, xmm6    ;xmm6 = x^2
    mulsd xmm6, xmm0    ;xmm6 = a*x^2
    movsd xmm7, xmm1    ;move b to xmm7
    mulsd xmm7, xmm5    ;xmm7 = x*b
    addsd xmm6, xmm7    ;xmm6 = a*x^2+x*b
    addsd xmm6, xmm2    ;xmm6 = a*x^2+x*b+c = y
    ;xmm6 - first y
    movsd xmm7, xmm4
    addsd xmm7, xmm7    ;xmm7 = 2*axis of symmetric

drawing_loop:
    xor r11, r11    ;r11 = is_colored flag

    ;Coloring the right pixel
    xor rax, rax    ;rax = is_colored = 0;
    mov r8, rbx     ;r8 = width
    mov r9, rcx     ;r9 = height

    cvtsd2si r14, xmm5 ;r14 = (int)x
    cvtsd2si r15, xmm6 ;r15 = (int)y

    ;add offset to x and y
    add r14, 256
    add r15, 256

    ;if x / y > width / height do not color
    cmp r14, r8
    ja end1
    cmp r15, r9
    ja end1

    lea r8, [r8+r8*2]
    add r8, 3
    and r8, 0xFFFFFFFFFFFFFFFC
    imul r8, r15

    lea r14, [r14 + r14*2]
    add r8, r14
    add r8, rdi
    add r8, 54

    mov word[r8], 0x00
    mov byte[r8+2], 0x00
    mov rax, 1
end1:
    or r11, rax

    ;Color left pixel
    xor rax, rax    ;rax = is_colored = 0;
    mov r8, rbx     ;r8 = width
    mov r9, rcx     ;r9 = height

    cvtsd2si r14, xmm5 ;r14 = (int)x
    cvtsd2si r15, xmm6 ;r15 = (int)y

    cvtsd2si r12, xmm7 ;r12 = (int)axis of symmetric

    ;calculate symmetric point
    neg r14
    add r14, r12

    ;add offset to x and y
    add r14, 256
    add r15, 256

    ;if x / y > width / height do not color
    cmp r14, r8
    ja end2
    cmp r15, r9
    ja end2

    lea r8, [r8+r8*2]
    add r8, 3
    and r8, 0xFFFFFFFFFFFFFFFC
    imul r8, r15

    lea r14, [r14 + r14*2]
    add r8, r14
    add r8, rdi
    add r8, 54

    mov word[r8], 0x00
    mov byte[r8+2], 0x00
    mov rax, 1
end2:
    or r11, rax
    test r11, r11
    jz exit2

    ;update x
    movsd xmm12, xmm5   ;move old x to xmm12
    movsd xmm8, xmm0    ;xmm8 = a
    addsd xmm8, xmm8    ;xmm8 = 2a
    mulsd xmm12, xmm8    ;xmm12 = 2*a*x
    addsd xmm12, xmm1    ;xmm12 = 2*a*x+b
    mulsd xmm12, xmm12   ;xmm12 = (2*a*x+b)^2

    mov r13, __float64__(1.0)
    movq xmm9, r13

    addsd xmm12, xmm9    ;xmm12 = (2*a*x+b)^2 + 1
    sqrtsd xmm12, xmm12   ;xmm12 = sqrt((2*a*x+b)^2 + 1)
    movsd xmm10, xmm3   ;xmm10 = s
    divsd xmm10, xmm12   ;xmm10 = s / sqrt((2*a*x+b)^2 + 1)
    addsd xmm5, xmm10   ;xmm5 = new x

    ;update y

    movsd xmm6, xmm5
    mulsd xmm6, xmm6    ;xmm6 = x^2
    mulsd xmm6, xmm0    ;xmm6 = a*x^2
    movsd xmm11, xmm1    ;move b to xmm11
    mulsd xmm11, xmm5    ;xmm11 = x*b
    addsd xmm6, xmm11    ;xmm6 = a*x^2+x*b
    addsd xmm6, xmm2    ;xmm6 = a*x^2+x*b+c = new y

    jmp drawing_loop
exit2:
    ;restore saved registers
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12
    ret
