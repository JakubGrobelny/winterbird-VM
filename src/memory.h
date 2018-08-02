#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"
#include "value.h"

typedef struct
{
	value_t 	registers[128];

	byte_t* 	stack;
	size_t		stack_size;
	byte_t*		stack_ptr;
	byte_t*     stack_frame_ptr;

	ptr_t		instr_ptr;
	uint8_t		test_flag;

} memory_t;

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes);
void free_memory(memory_t* memory);

void stack_push(memory_t* memory, value_t value, word_size_t size);
value_t stack_pop(memory_t* memory, word_size_t size);

#endif