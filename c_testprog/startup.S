/**
	@file
	@author Andrew D. Zonenberg
	@brief C startup code for test program
 */

#include "../asm_testprog/registers.h"

.set noreorder
.globl __start
__start:

	jal		main
	nop

quit:
	li		v0, 10				//syscall 10: quit program
	syscall
	nop

.globl do_syscall
do_syscall:
	or		v0, a2, zero		//copy third arg to syscall number
	syscall
	nop
	
	jr		ra
	nop
