#include "registers.h"
.globl __start

__start:
	li v0, 1

	addi s0, zero, 15
	addi s1, zero, 1

	srlv s2, s0, s1 // 1111 >> 1 = 111 (7)
	addi a0, s2, 0
	syscall

quit:
	li v0, 10
	syscall