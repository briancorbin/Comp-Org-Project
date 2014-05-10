#include "registers.h"
.globl __start

__start:
	li v0, 1
	addi a0, zero, 10
	syscall

quit:
	li v0, 10
	syscall