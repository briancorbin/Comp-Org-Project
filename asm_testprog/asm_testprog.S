#include "registers.h"

.globl __start

__start:
	li v0, 1

	addi s0, zero, 12
	move a0, s0
	syscall
	jal jump1
	syscall
	addi s0, zero, 15

jump1:
	addi s0, s0, 9
	move a0, s0
	syscall
	jr ra

quit:
	li v0, 10
	syscall