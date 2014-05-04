/**
	@file
	@author Andrew D. Zonenberg
	@brief Startup code for simulator
 */
#include "sim.h"

/**
	@brief Program entry point
 */
int main(int argc, char* argv[])
{
	//Sanity check args
	if(argc != 2)
	{
		printf("Usage: sim foo.elf\n");
		return 0;
	}
	
	//Read and map the file
	struct virtual_mem_region* memory = NULL;
	struct context ctx;
	ReadELF(argv[1], &memory, &ctx);
	
	//Run the CPU
	RunSimulator(memory, &ctx);
	
	//TODO: clean up
	
	return 0;
}

/**
	@brief Reads an ELF executable
 */
void ReadELF(const char* fname, struct virtual_mem_region** memory, struct context* ctx)
{
	//Zeroize all context stuff
	for(int i=0; i<32; i++)
		ctx->regs[i] = 0;
	
	//Open the file
	printf("Reading ELF file %s...\n", fname);
	FILE* fp = fopen(fname, "rb");
	if(fp == NULL)
	{
		printf("failed to load\n");
		exit(1);
	}
	
	//Read and validate the ELF header
	Elf32_Ehdr hdr;
	if(1 != fread(&hdr, sizeof(hdr), 1, fp))
	{
		printf("failed to read elf header\n");
		exit(1);
	}
	if( (hdr.e_ident[EI_MAG0] != 0x7f) ||
		(hdr.e_ident[EI_MAG1] != 'E') ||
		(hdr.e_ident[EI_MAG2] != 'L') ||
		(hdr.e_ident[EI_MAG3] != 'F')
		)
	{
		printf("bad ELF magic\n");
		exit(1);
	}
	if(hdr.e_ident[EI_DATA] != ELFDATA2LSB)
	{
		printf("not little endian\n");
		exit(1);
	}
	if(hdr.e_ident[EI_CLASS] != ELFCLASS32)
	{
		printf("not an ELFCLASS32\n");
		exit(1);
	}
	if(hdr.e_machine != EM_MIPS)
	{
		printf("not a MIPS binary\n");
		exit(1);
	}
	if(hdr.e_type != ET_EXEC)
	{
		printf("not an executable file\n");
		exit(1);
	}
	if(hdr.e_version != EV_CURRENT)
	{
		printf("not the right ELF version\n");
		exit(1);
	}
	
	//Save the entry point address
	printf("    Virtual address of entry point is %08x\n", hdr.e_entry);
	ctx->pc = hdr.e_entry;
	
	//Read the program headers
	if(0 != fseek(fp, hdr.e_phoff, SEEK_SET))
	{
		printf("fail to seek to phdrs\n");
		exit(1);
	}
	if(hdr.e_phentsize != sizeof(Elf32_Phdr))
	{
		printf("invalid phentsize\n");
		exit(1);
	}
	for(size_t i=0; i<hdr.e_phnum; i++)
	{
		Elf32_Phdr phdr;
		if(1 != fread(&phdr, sizeof(phdr), 1, fp))
		{
			printf("fail to read phdr\n");
			exit(1);
		}
		
		//Make the memory segment
		struct virtual_mem_region* region =
			(struct virtual_mem_region*)calloc(sizeof(struct virtual_mem_region), 1);
		region->vaddr = phdr.p_vaddr;
		region->len = phdr.p_memsz;
		region->data = calloc(phdr.p_memsz, 1);
		region->next = *memory;
		*memory = region;
		printf("    Mapping 0x%x bytes of virtual memory from executable at address %x\n", region->len, region->vaddr);
		
		//Skip non-loadable stuff
		if(phdr.p_type != PT_LOAD)
			continue;
			
		//Get ready to read the segment
		long current_pos = ftell(fp);
		if(0 != fseek(fp, phdr.p_offset, SEEK_SET))
		{
			printf("fail to re-seek\n");
			exit(1);
		}
		
		//Read the memory segment
		if(phdr.p_filesz != 0)
		{
			if(phdr.p_filesz != fread(region->data, 1, phdr.p_filesz, fp))
			{
				printf("fail to read memory region\n");
				exit(1);
			}
		}
		
		//Done, go back to where we were
		if(0 != fseek(fp, current_pos, SEEK_SET))
		{
			printf("fail to re-seek\n");
			exit(1);
		}
	}
	
	fclose(fp);
	
	//Create one last memory region for the stack, then point the stack pointer to it
	struct virtual_mem_region* region =
		(struct virtual_mem_region*)calloc(sizeof(struct virtual_mem_region), 1);
	region->vaddr = 0xc0000000;
	region->len = 0x8000;
	region->data = calloc(region->len, 1);
	region->next = *memory;
	*memory = region;
	ctx->regs[REGID_SP] = region->vaddr + region->len - 4;
	printf("    Mapping 0x%x bytes of virtual memory for stack at address %x\n", region->len, region->vaddr);
}
