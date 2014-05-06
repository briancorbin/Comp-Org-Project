/**
	@file
	@author Andrew D. Zonenberg
	@brief The core of the simulator
 */
#include "sim.h"

/**
	@brief Read logic for instruction fetch and load instructions
	
	Address must be aligned
 */
uint32_t FetchWordFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}
		
		//Align check
		uint32_t offset = address - memory->vaddr;
		if(offset & 3)
		{
			printf("SEGFAULT: address %08x is not aligned\n", address);
			exit(1);	
		}
		
		return memory->data[offset/4];
	}
	
	//Didn't find anything! Give up
	printf("SEGFAULT: attempted to read word from nonexistent virtual address %08x\n", address);
	exit(1);
}

/**
	@brief Write logic for store instructions.
	
	Stores an entire 32-bit word. sh/sb instructions will need to do a read-modify-write structure
 */
void StoreWordToVirtualMemory(uint32_t address, uint32_t value, struct virtual_mem_region* memory)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}
		
		//Align check
		uint32_t offset = address - memory->vaddr;
		if(offset & 3)
		{
			printf("SEGFAULT: address %08x is not aligned\n", address);
			exit(1);	
		}
		
		memory->data[offset/4] = value;
		return;
	}
	
	//Didn't find anything! Give up
	printf("SEGFAULT: attempted to write word to nonexistent virtual address %08x\n", address);
	exit(1);
}

/**
	@brief Runs the actual simulation
 */
void RunSimulator(struct virtual_mem_region* memory, struct context* ctx)
{
	printf("Starting simulation...\n");
	
	union mips_instruction inst;
	while(1)
	{
		inst.word = FetchWordFromVirtualMemory(ctx->pc, memory);
		if(!SimulateInstruction(&inst, memory, ctx))
			break;		
	}
}

/**
	@brief Simulates a single instruction
	
	Return 0 to exit the program (for syscall/invalid instruction) and 1 to keep going
 */
int SimulateInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{

 //    OP_BGEZ     = 0x01, //rt = 00001
 //    OP_BGEZAL   = 0x01, //rt = 10001
 //    OP_BLTZ     = 0x01, //rt = 00000
 //    OP_BLTZAL   = 0x01, //rt = 10000
 //    OP_J        = 0x02,
 //    OP_JAL      = 0x03,
 //    OP_BEQ      = 0x04,
 //    OP_BNE      = 0x05,
 //    OP_BLEZ     = 0x06,
 //    OP_BGTZ     = 0x07,
 //    OP_ADDI     = 0x08,
 //    OP_ADDIU    = 0x09,
 //    OP_SLTI     = 0x0a,
 //    OP_SLTIU    = 0x0b,
 //    OP_ANDI     = 0x0c,
 //    OP_ORI      = 0x0d,
 //    OP_XORI     = 0x0e,
 //    OP_LUI      = 0x0f,
 //    OP_LB       = 0x20,
 //    OP_LW       = 0x23,
 //    OP_SB       = 0x28,
	// OP_SW		= 0x2b

	//TODO: Switch on opcode, if R-type instruction call SimulateRTypeInstruction()
	//otherwise it's I/J type
	switch(inst->itype.opcode)
	{
		case OP_RTYPE:
			SimulateRtypeInstruction(inst, memory, ctx);
			break;
	}
	//Go on to next instruction by default
	//Need to change this for branches
	ctx->pc += 4;
	
	return 1;
}

int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	//TODO: switch on func, if syscall call SimulateSyscall()
	//else process instruction normally
	
	return 1;
}

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx)
{
	return 1;
}
