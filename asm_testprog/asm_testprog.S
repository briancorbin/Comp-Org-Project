#include "registers.h"

string1: .asciiz "Testing... Testing 1 2 3..."
string2: .space 100

.globl __start

__start:
	li v0, 8
	la a0, string2
	addi a1, zero, 12
	syscall

	li v0, 4
	la a0, string2
	syscall

quit:
	li v0, 10
	syscall 