; This is assembly part

section .text
default rel
global arima_diff_avx2

arima_diff_avx2:

vpxor ymm3, ymm3, ymm3 ;Initialize y-registers

xor r11,r11

sub:
cmp rcx, 8
jle remainder_loop


	vmovups  ymm1, [rdx+r11+4]
	vmovups ymm2, [rdx+r11]
	vsubps ymm0, ymm1, ymm2 

	vmovups [r8+r11], ymm0

	sub rcx, 8
	add r11, 4*8
	jmp sub

remainder_loop:
    cmp rcx, 0
    jle end                    ; Exit if no elements left

    movss xmm1, [rdx+r11+4]     ; Load next float
    movss xmm2, [rdx+r11]       ; Load current float
    subss xmm1, xmm2            ; Compute difference

    movss [r8+r11], xmm1        ; Store difference

    sub rcx, 1
    add r11, 4                  ; Move to next element
    jmp remainder_loop


end:
ret