/**
	@file
	@author Andrew D. Zonenberg
	@brief Test program
 */

#include "registers.h"

.globl __start
__start:
	//Simulator loads $sp with a valid address before starting program

quit:
	li		v0, 10				//syscall 10: quit program
	syscall
