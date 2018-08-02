#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

	memory->return_value = -1;
	memory->halt 		 = false;

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
	free_program(&memory->program_data);
}

value_t* get_pointer_from_operand(memory_t* memory, instruction_t* instruction, uint8_t operand_id)
{
	operand_t* operand = &instruction->operands[operand_id];

	switch (operand->type)
	{
		case AM_REG:
			return &memory->registers[operand->base_reg];
		case AM_REG_DEREF:
			return memory->registers[operand->base_reg].ptr 
				+ (memory->registers[operand->index_reg].i64 * operand->scale 
				+ operand->immediate.i64);
		case AM_IMM:
			return &operand->immediate;
		case AM_IMM_DEREF:
			return memory->program_data.data + operand->immediate.i64;
		case AM_NONE:
			return NULL;
		default:
			report_error(INVALID_OPERAND_TYPE, NULL);
			break;
	}
}

bool load_program(memory_t* memory, char* path)
{
	program_t* program = &memory->program_data;

	FILE* file;

	file = fopen(path, "rb");

	if (!file)
	{
		fclose(file);
		return false;
	}

	/* Winterbird bytecode file (.wb):
		8 bytes 					-- size of .data section
		X bytes 					-- bytes of .data section, at least one byte
		n * sizeof(instruction_t) 	-- n instructions (at least one)
	*/

	fread(&program->data_size, sizeof(size_t), 1, file);

	program->data = malloc(sizeof(program->data_size));

	if (!program->data)
	{
		fclose(file);
		return false;
	}

	fread(program->data, program->data_size, 1, file);

	const size_t instruction_length = sizeof(instruction_t);
	size_t text_capacity = 1;
	program->text_size = 0;
	program->text = malloc(sizeof(text_capacity));

	if (!program->text)
	{
		fclose(file);
		return false;
	}

	instruction_t* instr_ptr;

	while (fread(instr_ptr, instruction_length, 1, file))
	{
		if (program->text_size >= text_capacity)
		{
			text_capacity *= 2;
			program->text = realloc(program->text, sizeof(text_capacity));

			if (!program->text)
			{
				fclose(file);
				return false;
			}
		}

		program->text[program->text_size++] = *instr_ptr;
	}

	return true;
}
