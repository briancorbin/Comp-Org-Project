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
	//TODO: Switch on opcode, if R-type instruction call SimulateRTypeInstruction()
	//otherwise it's I/J type

	ctx->regs[zero] = 0;

	switch(inst->itype.opcode)
	{
		case OP_RTYPE:
			SimulateRtypeInstruction(inst, memory, ctx);
			break;
		case OP_BGEZ: //this will deal with BGEZ, BGEZAL, BLTZ, BLTZAL
            SimulateBswitch(inst, memory, ctx);
			break;
		case OP_J:
            simJ(inst, memory, ctx);
			break;
		case OP_JAL:
            simJAL(inst, memory, ctx);
			break;
		case OP_BEQ:
            simBEQ(inst, memory, ctx);
			break;
		case OP_BNE:
            simBNE(inst, memory, ctx);
			break;
		case OP_BLEZ:
            simBLEZ(inst, memory, ctx);
			break;
		case OP_BGTZ:
            simBGTZ(inst, memory, ctx);
			break;
		case OP_ADDI:
            simADDI(inst, memory, ctx);
			break;
		case OP_ADDIU:
            simADDIU(inst, memory, ctx);
			break;
		case OP_SLTI:
            simSLTI(inst, memory, ctx);
			break;
		case OP_SLTIU:
            simSLTIU(inst, memory, ctx);
			break;
		case OP_ANDI:
            simANDI(inst, memory, ctx);
			break;
		case OP_ORI:
            simORI(inst, memory, ctx);
			break;
		case OP_XORI:
            simXORI(inst, memory, ctx);
			break;
		case OP_LUI:
            simLUI(inst, memory, ctx);
			break;
		case OP_LB:
            simLB(inst, memory, ctx);
			break;
		case OP_LW:
            simLW(inst, memory, ctx);
			break;
		case OP_SB:
            simSB(inst, memory, ctx);
			break;
		case OP_SW:
            simSW(inst, memory, ctx);
			break;
		default:
			printf("Invalid or unsupported instruction opcode\n");
			return 0;
	}
	
	return 1;
}

int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	//TODO: switch on func, if syscall call SimulateSyscall()
	//else process instruction normally
	switch (inst->rtype.func) {
		case FUNC_SLL:
            simSLL(inst, memory, ctx);
			break;
		case FUNC_SRL:
            simSRL(inst, memory, ctx);
			break;
		case FUNC_SRA:
            simSRA(inst, memory, ctx);
			break;
		case FUNC_SLLV:
            simSLLV(inst, memory, ctx);
			break;
		case FUNC_SRLV:
            simSRLV(inst, memory, ctx);
			break;
		case FUNC_JR:
            simJR(inst, memory, ctx);
			break;
		case FUNC_SYSCALL:
			SimulateSyscall(ctx->regs[v0], memory, ctx);
			break;
		case FUNC_MFHI:
            simMFHI(inst, memory, ctx);
			break;
		case FUNC_MFLO:
            simMFLO(inst, memory, ctx);
			break;
		case FUNC_MULT:
            simMULT(inst, memory, ctx);
			break;
		case FUNC_MULTU:
            simMULTU(inst, memory, ctx);
			break;
		case FUNC_DIV:
            simDIV(inst, memory, ctx);
			break;
		case FUNC_DIVU:
            simDIVU(inst, memory, ctx);
			break;
		case FUNC_ADD:
            simADD(inst, memory, ctx);
			break;
		case FUNC_ADDU:
            simADDU(inst, memory, ctx);
			break;
		case FUNC_SUB:
            simSUB(inst, memory, ctx);
			break;
		case FUNC_SUBU:
            simSUBU(inst, memory, ctx);
			break;
		case FUNC_AND:
            simAND(inst, memory, ctx);
			break;
		case FUNC_OR:
            simOR(inst, memory, ctx);
			break;
		case FUNC_XOR:
            simXOR(inst, memory, ctx);
			break;
		case FUNC_SLT:
            simSLT(inst, memory, ctx);
			break;
		case FUNC_SLTU:
            simSLTU(inst, memory, ctx);
			break;
		default:
			printf("Invalid or unsupported instruction func code\n");
			return 0;
	}
	return 1;
}

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx)
{
	switch (callnum) {
		case 1: //print integer
			printf("%d\n", ctx->regs[a0]);
			break;
		case 4: //print string
			printf("%s", ctx->regs[a0]);
			break;
		case 5: //read integer
			scanf("%d", ctx->regs[v0]);
			break;
		case 8: //read string
			scanf("%s", ctx->regs[v0]);
			break;
		case 10: //exit (end of program)
			exit(1);
			break;
		default:
			break;
	}
    
    ctx->pc += 4;
	return 1;
}

int SimulateBswitch(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	switch (inst->itype.rt) {
		case 0x01:
			simBGEZ(inst, memory, ctx);
			break;
		case 0x11:
			simBGEZAL(inst, memory, ctx);
			break;
		case 0x00:
			simBLTZ(inst, memory, ctx);
			break;
		case 0x10:
			simBLTZAL(inst, memory, ctx);
			break;
		default:
			return 0;
	}
	return 1;
}

void simBGEZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if (ctx->regs[inst->itype.rs] >= 0)
		ctx->pc = (inst->itype.imm << 2);
    else
        ctx->pc += 4;
}

void simBGEZAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if (ctx->regs[inst->itype.rs] >= 0) {
		ctx->regs[ra] = ctx->pc + 8;
		ctx->pc = (inst->itype.imm << 2);
	}
    else
        ctx->pc += 4;
}

void simBLTZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if (ctx->regs[inst->itype.rs] < 0)
		ctx->pc = (inst->itype.imm << 2);
    else
        ctx->pc += 4;
}

void simBLTZAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if (ctx->regs[inst->itype.rs] < 0) {
		ctx->regs[ra] = ctx->pc + 8;
		ctx->pc = (inst->itype.imm << 2);
	}
    else
        ctx->pc += 4;
}

void simJ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	ctx->pc = (ctx->pc & 0xf0000000) | (inst->jtype.addr << 2);
}

void simJAL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	ctx->regs[ra] = ctx->pc + 8;
	ctx->pc = (ctx->pc & 0xf0000000) | (inst->jtype.addr << 2);
}

void simBEQ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if(ctx->regs[inst->itype.rs] == ctx->regs[inst->itype.rt])
 		ctx->pc = (ctx->pc + (inst->itype.imm << 2));
    else
        ctx->pc += 4;
}

void simBNE(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if(ctx->regs[inst->itype.rs] != ctx->regs[inst->itype.rt])
 		ctx->pc = (ctx->pc + (inst->itype.imm << 2));
    else
        ctx->pc += 4;
}

void simBLEZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if (ctx->regs[inst->itype.rs] <= 0)
		ctx->pc = (ctx->pc + (inst->itype.imm << 2));
    else
        ctx->pc += 4;
}

void simBGTZ(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	if (ctx->regs[inst->itype.rs] > 0)
		ctx->pc = (ctx->pc + (inst->itype.imm << 2));
    else
        ctx->pc += 4;
}

void simADDI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + inst->itype.imm;
    
    ctx->pc += 4;
}

void simADDIU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] + inst->itype.imm;
    
    ctx->pc += 4;
}

void simSLTI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    if(ctx->regs[inst->itype.rs] < inst->itype.imm)
        ctx->regs[inst->itype.rt] = 1;
    else
        ctx->regs[inst->itype.rt] = 0;
    
    ctx->pc += 4;
}

void simSLTIU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    if(ctx->regs[inst->itype.rs] < inst->itype.imm)
        ctx->regs[inst->itype.rt] = 1;
    else
        ctx->regs[inst->itype.rt] = 0;
    
    ctx->pc += 4;
}

void simANDI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] & inst->itype.imm;
    ctx->pc += 4;
}

void simORI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] | inst->itype.imm;
    ctx->pc += 4;
}

void simXORI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] ^ inst->itype.imm;
    ctx->pc += 4;
}

void simLUI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = inst->itype.imm<<16;
    ctx->pc += 4;
}

void simLB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    if(inst->itype.imm % 4 == 0)
		ctx->regs[inst->itype.rt] = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, memory) & 0x000000ff;
	else if(inst->itype.imm % 4 == 1)
		ctx->regs[inst->itype.rt] = (FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm - 1, memory) & 0x0000ff00)>>8;
	else if(inst->itype.imm % 4 == 2)
		ctx->regs[inst->itype.rt] = (FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm - 2, memory) & 0x00ff0000)>>16;
	else
		ctx->regs[inst->itype.rt] = (FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm - 3, memory) & 0xff000000)>>24;
    
    ctx->pc += 4;
}

void simLW(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->itype.rt] = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, memory);
    ctx->pc += 4;
}

void simSB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    uint32_t tempAddressData;
    if(inst->itype.imm % 4 == 0)
    {
        tempAddressData = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, memory);
        tempAddressData = tempAddressData & 0xffffff00;
        tempAddressData = tempAddressData | (ctx->regs[inst->itype.rt] & 0x000000ff);
        StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, tempAddressData, memory);
    }
	else if(inst->itype.imm % 4 == 1)
    {
        tempAddressData = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm - 1, memory);
        tempAddressData = tempAddressData & 0xffff00ff;
        tempAddressData = tempAddressData | ((ctx->regs[inst->itype.rt]<<8) & 0x0000ff00);
        StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, tempAddressData, memory);
    }
	else if(inst->itype.imm % 4 == 2)
    {
        tempAddressData = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm - 2, memory);
        tempAddressData = tempAddressData & 0xff00ffff;
        tempAddressData = tempAddressData | ((ctx->regs[inst->itype.rt]<<16) & 0x00ff0000);
        StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, tempAddressData, memory);
    }
	else
    {
        tempAddressData = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm - 3, memory);
        tempAddressData = tempAddressData & 0x00ffffff;
        tempAddressData = tempAddressData | ((ctx->regs[inst->itype.rt]<<24) & 0xff000000);
        StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, tempAddressData, memory);
    }
    ctx->pc += 4;
}

void simSW(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, ctx->regs[inst->itype.rt], memory);
    ctx->pc += 4;
}

void simSLL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]<<inst->rtype.shamt;
    ctx->pc += 4;
}

void simSRL(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]>>inst->rtype.shamt;
    ctx->pc += 4;
}

void simSRA(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]>>inst->rtype.shamt;
    ctx->pc += 4;
}

void simSLLV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]<<ctx->regs[inst->rtype.rs];
    ctx->pc += 4;
}

void simSRLV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt]>>ctx->regs[inst->rtype.rs];
    ctx->pc += 4;
}

void simJR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
	ctx->pc = ctx->regs[inst->rtype.rs];
}

void simMFHI(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->HI;
    ctx->pc += 4;
}

void simMFLO(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->LO;
    ctx->pc += 4;
}

void simMULT(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->LO = ctx->regs[inst->rtype.rs] * ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simMULTU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->LO = ctx->regs[inst->rtype.rs] * ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simDIV(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->LO = ctx->regs[inst->rtype.rs] / ctx->regs[inst->rtype.rt];
    ctx->HI = ctx->regs[inst->rtype.rs] % ctx->regs[inst->rtype.rt];
    
    ctx->pc += 4;
}

void simDIVU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->LO = ctx->regs[inst->rtype.rs] / ctx->regs[inst->rtype.rt];
    ctx->HI = ctx->regs[inst->rtype.rs] % ctx->regs[inst->rtype.rt];
    
    ctx->pc += 4;
}

void simADD(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simADDU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simSUB(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simSUBU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simAND(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] & ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simOR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] | ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simXOR(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] ^ ctx->regs[inst->rtype.rt];
    ctx->pc += 4;
}

void simSLT(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    if(ctx->regs[inst->rtype.rs] < ctx->regs[inst->rtype.rt])
        ctx->regs[inst->rtype.rd] = 1;
    else
        ctx->regs[inst->rtype.rd] = 0;
    
    ctx->pc += 4;
}

void simSLTU(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx)
{
    if(ctx->regs[inst->rtype.rs] < ctx->regs[inst->rtype.rt])
        ctx->regs[inst->rtype.rd] = 1;
    else
        ctx->regs[inst->rtype.rd] = 0;
    
    ctx->pc += 4;
}
