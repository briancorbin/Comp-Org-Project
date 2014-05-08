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

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx)
{
	return 1;
}

void simBGEZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	switch (inst->itype.rt) {
		case 
	}
}

void simJ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simJAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simBEQ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simBNE(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simBLEZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simBGTZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simADDI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + inst->itype.imm;
}

void simADDIU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSLTI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSLTIU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simANDI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] & inst->itype.imm;
}

void simORI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] | inst->itype.imm;
}

void simXORI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] ^ inst->itype.imm;
}

void simLUI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simLB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simLW(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSW(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSLL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]<<inst->rtype.shamt;
}

void simSRL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]>>inst->rtype.shamt;
}

void simSRA(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSLLV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]<<ctx->regs[inst->rtype.rs];
}

void simSRLV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]>>ctx->regs[inst->rtype.rs];
}

void simJR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simMFHI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simMFLO(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simMULT(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] * ctx->regs[inst->rtype.rt];
}

void simMULTU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simDIV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] / ctx->regs[inst->rtype.rt];
}

void simDIVU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simADD(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
}

void simADDU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
}

void simSUB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
}

void simSUBU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
}

void simAND(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] & ctx->regs[inst->rtype.rt];
}

void simOR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] | ctx->regs[inst->rtype.rt];
}

void simXOR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] ^ ctx->regs[inst->rtype.rt];
}

void simSLT(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

void simSLTU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
}

/**
	@brief Simulates a single instruction
	
	Return 0 to exit the program (for syscall/invalid instruction) and 1 to keep going
 */
int SimulateInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	//TODO: Switch on opcode, if R-type instruction call SimulateRTypeInstruction()
	//otherwise it's I/J type
	switch(inst->itype.opcode)
	{
		case OP_RTYPE:
			SimulateRtypeInstruction(inst, memory, ctx);
			break;
		case OP_BGEZ:

			break;
		case OP_J:

			break;
		case OP_JAL:

			break;
		case OP_BEQ:

			break;
		case OP_BNE:

			break;
		case OP_BLEZ:

			break;
		case OP_BGTZ:

			break;
		case OP_ADDI:
            simADDI(ints, memory, ctx);

			break;
		case OP_ADDIU:

			break;
		case OP_SLTI:

			break;
		case OP_ANDI:

			break;
		case OP_ORI:

			break;
		case OP_XORI:

			break;
		case OP_LUI:

			break;
		case OP_LB:

			break;
		case OP_LW:

			break;
		case OP_SB:

			break;
		case OP_SW:

			break;
		default:
			printf("Invalid or unsupported instruction\n");
			return 0;
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
	switch (inst->rtype.func) {
		case FUNC_SLL:

			break;
		case FUNC_SRL:

			break;
		case FUNC_SRA:

			break;
		case FUNC_SLLV:

			break;
		case FUNC_SRLV:

			break;
		case FUNC_JR:

			break;
		case FUNC_SYSCALL:
			//SimulateSyscall(, memory, context)
			break;
		case FUNC_MFHI:

			break;
		case FUNC_MFLO:

			break;
		case FUNC_MULT:

			break;
		case FUNC_MULTU:

			break;
		case FUNC_DIV:

			break;
		case FUNC_DIVU:

			break;
		case FUNC_ADD:

			break;
		case FUNC_ADDU:

			break;
		case FUNC_SUB:

			break;
		case FUNC_SUBU:

			break;
		case FUNC_AND:

			break;
		case FUNC_OR:

			break;
		case FUNC_XOR:

			break;
		case FUNC_SLT:

			break;
		case FUNC_SLTU:

			break;
	}
	return 1;
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
