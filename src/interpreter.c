#include "interpreter.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
	// === MEMORY === //
		// basic value movement
		case OP_MOV:
			switch (instruction->size)
			{
				case SIZE_8:
					op1->u8 = op2->u8;
					break;
				case SIZE_16:
					op1->u16 = op2->u16;
					break;
				case SIZE_32:
					op1->u32 = op2->u32;
					break;
				case SIZE_64:
					op1->u64 = op2->u64;
					break;
				default:
					report_error(INVALID_INSTRUCTION_SIZE, NULL);
					break;
			}
			break;
		case OP_SWAP:
			value_t temp;
			temp.u64 = op1->u64;
			op1->u64 = op2->u64;
			op2->u64 = temp.u64;
			break;
		// dynamic allocation
		case OP_ALLOC:
			op1->ptr = malloc(op2->u64);
			break;
		case OP_FREE:
			free(op1->ptr);
			break;
		// files
		case OP_FOPEN:
			//TODO:
			//create a registry of opened files and close them in case of a crash
			char* mode;
			switch (op1->u8)
			{
				case 1: mode = "rb";
					break;
				case 2: mode = "wb";
					break;
				case 3: mode = "ab";
					break;
				case 4: mode = "rb+";
					break;
				case 5: mode = "wb+";
					break;
				case 6: mode = "ab+";
					break;
				default:
					mode = "rb";
					break;
			}

			FILE* file = fopen(op2->str, mode);
			op1->ptr = (void*)file;
			break;
		case OP_FCLOSE:
			fclose(op1->ptr);
			break;
		case OP_FREADB:
			fread(&op2, 1, 1, op1->ptr);
			break;
		// stack
		case OP_PUSH:
			stack_push(memory, *op1, instruction->size);
			break;
		case OP_POP:
			op1->u64 = stack_pop(memory, instruction->size).u64;
			break;
	// === ALU === //
		// signed
		case OP_ADD:
			op1->i64 += op2->i64;
			break;
		case OP_SUB:
			op1->i64 -= op2->i64;
			break;
		case OP_NEG:
			op1->i64 = -op2->i64;
			break;
		case OP_MUL:
			op1->i64 *= op2->i64;
			break;
		case OP_DIV:
			if (op2->i64)
				op1->i64 /= op2->i64;
			else
				report_error(DIVISION_BY_ZERO, NULL);
			break;
		case OP_MOD:
			if (op2->i64)
				op1->i64 %= op2->i64;
			else
				report_error(DIVISION_BY_ZERO, NULL);
			break;
		case OP_ROR:
			asm("rorq %0, %1"
				:"=r"(op1->u64)
				:"r"(op2->u64)
				:);
			break;
		case OP_ROL:
			asm("rolq %0, %1"
				:"=r"(op1->u64)
				:"r"(op2->u64)
				:);
			break;
		case OP_SHR:
			op1->i64 >>= op2->u64;
			break;
		case OP_SHL:
			op1->i64 <<= op2->u64;
			break;
		// unsigned
		case OP_UMUL:
			op1->u64 *= op2->u64;
			break;
		case OP_UDIV:
			if (op2->u64)
				op1->u64 /= op2->u64;
			else
				report_error(DIVISION_BY_ZERO, NULL);
			break;
		case OP_UMOD:
			if (op2->u64)
				op1->u64 %= op2->u64;
			else
				report_error(DIVISION_BY_ZERO, NULL);
			break;
		case OP_USHR:
			op1->u64 >>= op2->u64;
			break;
		case OP_USHL:
			op1->u64 <<= op2->u64;
			break;
		// float
		case OP_FADD:
			op1->f64 += op2->f64;
			break;
		case OP_FSUB:
			op1->f64 -= op2->f64;
			break;
		case OP_FNEG:
			op1->f64 = -op2->f64;
			break;
		case OP_FMUL:
			op1->f64 *= op2->f64;
			break;
		case OP_FDIV:
			op1->f64 /= op2->f64;
			break;
		// casts
		case OP_FTOI:
			op1->i64 = (int64_t)op2->f64;
			break;
		case OP_ITOF:
			op1->f64 = (double)op2->i64;
			break;
		// logical
		case OP_AND:
			op1->u64 &= op2->u64;
			break;
		case OP_OR:
			op1->u64 |= op2->u64;
			break;
		case OP_XOR:
			op1->u64 ^= op2->u64;
			break;
		case OP_NOT:
			op1->u64 = ~op2->u64;
			break;
		// other
		case OP_BSET:
			op1->u64 |= (uint64_t)1 << op2->u64;
		case OP_BCLR:
			op1->u64 &= ~((uint64_t)1 << op2->u64);
			break;
		case OP_DEC:
			op1->u64--;
			break;
		case OP_INC:
			op1->u64++;
			break;
	// === COMPARISONS === //
		// test flag manipulators
		case OP_NTEST:
			memory->test_flag = !memory->test_flag;
			break;
		case OP_STEST:
			memory->test_flag = op1->i64;
			break;
		// integer comparisons
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
		// unsigned comparisons
		case OP_CMPUG:
			memory->test_flag = op1->u64 > op2->u64;
			break;
		case OP_CMPUL:
			memory->test_flag = op1->u64 < op2->u64;
			break;
		// float comparisons
		case OP_CMPFZ:
			memory->test_flag = op1->f64 == 0.0f;
			break;
		case OP_CMPFI:
			memory->test_flag = op1->f64 == INFINITY;
			break;
		case OP_CMPFN:
			memory->test_flag = op1->f64 == NAN;
			break;
		case OP_CMPFEQ:
			memory->test_flag = op1->f64 == op2->f64;
			break;
		case OP_CMPFG:
			memory->test_flag = op1->f64 > op2->f64;
			break;
		case OP_CMPFL:
			memory->test_flag = op1->f64 < op2->f64;
			break;
	// === BRANCHING === //
		//NOTE: PC is incremented outside this function, so address should be 
		//      pointing to previous instruction (relatively to jump target)
		case OP_JMP:
			memory->instr_ptr = op1->i64;
			break;
		case OP_JIF:
			memory->instr_ptr = memory->test_flag 
							  ? memory->instr_ptr
							  : op1->i64;
			break;
		case OP_CALL:
			stack_push(memory, (value_t)memory->instr_ptr, sizeof(memory->instr_ptr));
			memory->instr_ptr = op1->i64;
			break;
		case OP_RET:
			memory->instr_ptr = stack_pop(memory, sizeof(memory->instr_ptr)).i64;
			break;
	// === OTHER === //
		case OP_HALT:
			memory->return_value = op1->i64;
			memory->halt = true;
			break;
		case OP_SYSCALL:
			//TODO: implement
			break;
		case OP_DEBUG:
			print_stack_trace(memory, op1->u32);
			break;
	// === DEFAULT === //
		default:
			report_error(INVALID_OPCODE, NULL);
			break;
	}
}
