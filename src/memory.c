#include "memory.h"
#include <stdlib.h>
#include <string.h>

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes)
{
	for (int i = 0; i < 32; i++)
		memory->registers[i].i64 = 0;

	memory->stack_size = stack_megabytes * MEGABYTE;
	memory->stack = malloc(memory->stack_size);

	memory->stack_ptr 		= memory->stack;
	memory->stack_frame_ptr = memory->stack;

	memory->test_flag = 0;
	memory->instr_ptr = 0;	

	return memory->stack;
}

void stack_push(memory_t* memory, value_t value, word_size_t size)
{
	if (memory->stack_ptr + size > memory->stack_ptr + memory->stack_size)
		report_error(STACK_OVERFLOW, NULL);

	memcpy(memory->stack_ptr, &value + (8 - size), size);
	memory->stack_ptr += size;
}

value_t stack_pop(memory_t* memory, word_size_t size)
{
	if (memory->stack_ptr < size)
		report_error(STACK_UNDERFLOW, NULL);

	value_t result;
	result.i64 = 0x0;

	memory->stack_ptr -= size;
	memcpy(&result, memory->stack_ptr, size);
}

void free_memory(memory_t* memory)
{
	free(memory->stack);
	memory->stack_size = 0;
	memory->stack_ptr = NULL;
	memory->stack_frame_ptr = NULL;
}