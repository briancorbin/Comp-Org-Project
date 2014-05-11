#include "registers.h"
.globl __start

__start:
	li v0, 1

	add s0, zero, 10
	add s1, zero, 3

quit:
	li v0, 10
	syscall