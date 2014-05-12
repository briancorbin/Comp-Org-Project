#include "registers.h"

.data

mem1: .space 16

.globl __start

__start:
	li v0, 1
	la s0, mem1

	addi s1, zero, 255
	sb s1, 0(s0)
	addi s1, zero, 121
	sb s1, 1(s0)
	addi s1, zero, 12
	sb s1, 2(s0)
	addi s1, zero, 79
	sb s1, 3(s0)

	lb s2, 0(s0)
	addi a0, s2, 0
	syscall

	lb s2, 1(s0)
	addi a0, s2, 0
	syscall

	lb s2, 2(s0)
	addi a0, s2, 0
	syscall

	lb s2, 3(s0)
	addi a0, s2, 0
	syscall

quit:
	li v0, 10
	syscall