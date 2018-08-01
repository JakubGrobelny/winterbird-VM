#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"

typedef struct
{
	uint64_t 	i_registers[32];
	double   	f_registers[32];

	byte_t* 	stack;

	uint64_t  	test_flag;
	uint64_t    instr_ptr;
	uint64_t    stack_ptr;

} memory_t;


bool initialize_memory(memory_t* memory);

#endif