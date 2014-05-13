#include "registers.h"

.globl __start

__start:
	li v0, 1

	addi s0, zero, 12
	move a0, s0
	syscall
	bgez s0, jump1
	addi s0, zero, 9

jump1:
	addi s0, s0, 9
	move a0, s0
	syscall

quit:
	li v0, 10
	syscall 