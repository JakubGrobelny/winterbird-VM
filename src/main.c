#include "interpreter.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	assert(argc == 2);

	const size_t DEFAULT_STACK_SIZE_IN_MB = 16;
	
	memory_t memory;

	if (!initialize_memory(&memory, DEFAULT_STACK_SIZE_IN_MB))
	{
		fprintf(stderr, "Failed to initialize memory!\n");
		free_memory(&memory);
		return INIT_ERROR;
	}

	if (!load_program(&memory, argv[1]))
	{
		fprintf(stderr, "Failed to load file \"%s\"\n", argv[1]);
		free_memory(&memory);
		return INIT_ERROR;
	}

	run_bytecode(&memory);

#ifndef NDEBUG
	print_stack_trace(&memory, 5);
#endif

	free_memory(&memory);
	return memory.return_value;
}