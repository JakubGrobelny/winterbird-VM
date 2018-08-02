#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"
#include "value.h"
#include "instruction.h"
#include "program.h"

typedef struct
{
	value_t registers[128];

	byte_t*   stack;
	size_t	  stack_size;
	byte_t*	  stack_ptr;
	byte_t*   stack_frame_ptr;
  
	ptr_t	  instr_ptr;
	uint64_t  test_flag;

	program_t program_data;

	int32_t   return_value;
	bool 	  halt;

} memory_t;

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes);
bool load_program(memory_t* memory, char* path);
void free_memory(memory_t* memory);
value_t* get_pointer_from_operand(memory_t* memory, instruction_t* instruction, uint8_t operand_id);
void stack_push(memory_t* memory, value_t value, word_size_t size);
value_t stack_pop(memory_t* memory, word_size_t size);

#endif