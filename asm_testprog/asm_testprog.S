#include "registers.h"

string1: .asciiz "Testing... Testing 1 2 3..."

.globl __start

__start:
	li v0, 5
	syscall

	move s0, v0

	li v0, 1
	move a0, s0
	syscall

quit:
	li v0, 10
	syscall 