Directory structure
	asm_testprog		Assembly-only test program (produces asm_testprog.elf). Modify as needed to test your simulator, but will not be graded.
	c_testprog			C+assembly test program (produces c_testprog.elf). Modify as needed to test your simulator, but will not be graded
	sim					The simulator itself, you will need to fill in the missing parts.
	
Basic workflow
	Make changes to test code in c_testprog/ or asm_testprog/ as needed
	Make changes to simulator code in sim as needed
	Run "make" in the appropriate test program directory to compile your test code to a MIPS executable.
	Go to the sim directory and run "make" to compile your simulator
	Run "./sim ../test_program_dir/input_file.elf" (using the appropriate input file name) to launch the simulation.
