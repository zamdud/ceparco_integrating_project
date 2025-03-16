; This is assembly part

section .text
default rel
global arima_diff_avx2

arima_diff_avx2:

vpxor ymm3, ymm3, ymm3 ;Initialize y-registers
xor r11,r11 ;Ensures r11 is 0 for the offset

sub:
    cmp rcx, 8 ;compares the size to 8
    jle remainder_loop ;if the size is less than or equal to 8, go to remainder_loop

	vmovups  ymm1, [rdx+r11+4]  ; Moves 8 elements with an offset of one element in advance into ymm1
	vmovups ymm2, [rdx+r11]     ; Moves 8 elements into ymm2
	vsubps ymm0, ymm1, ymm2     ; Subtracts the elements of ymm1 to ymm2 with respect to their index and stores it into ymm0

	vmovups [r8+r11], ymm0      ; Stores the resulting array back into the destination array in c

	sub rcx, 8                  ; Subtracts 8 elements to the size count
	add r11, 4*8                ; Adds an offset for the next elements to be processed
	jmp sub                     ; Jumps back to the sub label

remainder_loop:
    cmp rcx, 0                  
    jle end                     ; Exit if there are no elements left

    movss xmm1, [rdx+r11+4]     ; Load next float
    movss xmm2, [rdx+r11]       ; Load current float
    subss xmm1, xmm2            ; Compute difference

    movss [r8+r11], xmm1        ; Store difference

    sub rcx, 1                  ; subtracts 1 elements to the size count
    add r11, 4                  ; Move to next element
    jmp remainder_loop

end:
    ret