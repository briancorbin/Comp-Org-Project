#include "registers.h"

.data

mem1: .space 16

.globl __start

__start:
	li v0, 1
	la s0, mem1

	addi s1, zero, 255

	sb s1, 13(s0)
	lb s2, 13(s0)

	addi a0, s2, 0
	syscall

quit:
	li v0, 10
	syscall