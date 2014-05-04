CFLAGS=-mips1 -mno-abicalls -fno-pic -nostdlib -nodefaultlibs -nostartfiles -mno-mips16 -O -fno-delayed-branch
all:
	mipsel-linux-gnu-gcc -c startup.S -o startup.o $(CFLAGS)
	mipsel-linux-gnu-gcc -c main.c -o main.o $(CFLAGS)
	mipsel-linux-gnu-gcc main.o startup.o -o c_testprog.elf $(CFLAGS)
