/**
	@file
	@author Andrew D. Zonenberg
	@brief Register definitions for MIPS assembly
 */

#ifndef registers_h
#define registers_h

#ifdef __LANGUAGE_ASSEMBLY__

//CPU core registers
#define zero  				$0
#define at     				$1	/* assembler temporary */
#define v0					$2
#define v1					$3
#define a0					$4
#define a1					$5
#define a2					$6
#define a3					$7
#define t0					$8
#define t1					$9
#define t2					$10
#define t3					$11
#define t4					$12
#define t5					$13
#define t6					$14
#define t7					$15
#define t8					$24
#define t9					$25
#define s0					$16
#define s1					$17
#define s2					$18
#define s3					$19
#define s4					$20
#define s5					$21
#define s6					$22
#define s7					$23
#define s8					$30	/* s8 and fp are same register */
#define fp					$30
#define k0 					$26	/* reserved for kernel */
#define k1					$27
#define gp					$28
#define sp					$29
#define ra					$31

#endif

#endif
