#include "registers.h"
.globl __start

__start:
	li v0, 1

	addi a0, zero, 10
	syscall

	addiu a0, zero, 8
	syscall

	slti a0, a0, 10
	syscall

	sltiu a0, a0, 0
	syscall

quit:
	li v0, 10
	syscall