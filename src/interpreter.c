#include "interpreter.h"

void run_bytecode(memory_t* memory)
{
	while (!memory->halt)
	{
		if (was_error())
			throw_error();

		memory->instr_ptr++;
	}
}

void run_instruction(memory_t* memory, instruction_t* instruction)
{
	value_t* op1 = get_pointer_from_operand(memory, instruction, 0);
	value_t* op2 = get_pointer_from_operand(memory, instruction, 1);

	switch (instruction->instr)
	{
		case OP_HALT:
			memory->return_value = op1->i64;
			memory->halt = true;
			break;
	// === COMPARISONS === //
		case OP_NTEST:
			memory->test_flag = !memory->test_flag;
			break;
		case OP_STEST:
			memory->test_flag = op1->i64;
			break;
		case OP_CMPZ:
			memory->test_flag = op1->i64 == 0;
			break;
		case OP_GTEST:
			op1->i8 = memory->test_flag;
			break;
		case OP_CMPG:
			memory->test_flag = op1->i64 > op2->i64;
			break;
		case OP_CMPL:
			memory->test_flag = op1->i64 < op2->i64;
			break;
		case OP_CMPEQ:
			memory->test_flag = op1->i64 == op2->i64;
			break;
		case OP_CMPUG:
			memory->test_flag = op1->u64 > op2->u64;
			break;
		case OP_CMPUL:
			memory->test_flag = op1->u64 < op2->u64;
			break;
		case OP_CMPFZ:
		case OP_CMPFI:
		case OP_CMPFN:
		case OP_CMPFEQ:
		case OP_CMPFG:
		case OP_CMPFL:
		default:
			report_error(INVALID_OPCODE, NULL);
			break;
	}
}
