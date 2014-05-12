#include "registers.h"
.globl __start

__start:
	li v0, 1

	addi s0, zero, 15
	addi s1, zero, 1
	j skip
	add s0, s1, s0
skip:
	syscall

quit:
	li v0, 10
	syscall