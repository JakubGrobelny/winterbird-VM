#include "memory.h"
#include <stdlib.h>

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes)
{
	for (int i = 0; i < 32; i++)
		memory->i_registers[i] = 0x0;
	for (int i = 0; i < 32; i++)
		memory->f_registers[i] = 0x0f;

	const uint64_t STACK_SIZE = stack_megabytes * MEGABYTE;
	memory->stack = malloc(STACK_SIZE);

	memory->test_flag = 0x0;
	memory->instr_ptr = 0x0;
	memory->stack_ptr = 0;

	return memory->stack;
}