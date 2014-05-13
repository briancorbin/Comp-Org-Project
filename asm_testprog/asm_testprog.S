#include "registers.h"

string1: .asciiz "Testing... Testing 1 2 3..."

.globl __start

__start:
	li v0, 4
	la a0, string1
	syscall

quit:
	li v0, 10
	syscall 