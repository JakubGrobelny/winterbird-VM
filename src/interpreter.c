#include "interpreter.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void run_bytecode(memory_t* memory)
{
    while (!memory->halt)
    {
        instruction_t* instruction = &memory->program_data.text[memory->instr_ptr];

        run_instruction(memory, instruction);

        if (was_error())
            throw_error();

        memory->instr_ptr++;
        
        if (memory->instr_ptr == memory->program_data.text_size)
            break;
    }
}

void run_instruction(memory_t* memory, instruction_t* instruction)
{
    value_t* op1 = get_pointer_from_operand(memory, instruction, 0);
    value_t* op2 = get_pointer_from_operand(memory, instruction, 1);

    switch (instruction->opcode)
    {
    // === MEMORY === //
        // basic value movement
        case OP_MOV8:
            op1->u8 = op2->u8;
            break;
        case OP_MOV16:
            op1->u16 = op2->u16;
            break;
        case OP_MOV32:
            op1->u32 = op2->u32;
            break;
        case OP_MOV64:
            op1->u64 = op2->u64;
            break;
        case OP_SWAP:
        {
            value_t temp;
            temp.u64 = op1->u64;
            op1->u64 = op2->u64;
            op2->u64 = temp.u64;
            break;
        }
        case OP_LADDR:
            op1->ptr = op2;
            break;
        case OP_MEMPREP:
            memory->memprep_ptr = op1->ptr;
            memory->memprep_val = op1->i64;
            break;
        case OP_MEMCPY:
            memcpy(memory->memprep_ptr, op1->ptr, op2->u64);
            break;
        case OP_MEMSET:
            memset(memory->memprep_ptr, op1->i8, op1->u64);
            break;
        // dynamic allocation
        case OP_ALLOC:
#ifdef NO_TRACK_ALLOC
            op1->ptr = malloc(op2->u64);
#else
            op1->ptr = tracked_alloc(memory, op2->u64);
#endif
            break;
        case OP_FREE:
#ifdef NO_TRACK_ALLOC
            free(op1->ptr);
#else
            tracked_free(memory, op1->ptr);
#endif
            break;
        // files
        case OP_FOPEN:
        {
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
                    report_error(INVALID_FILE_MODE, op2->str);
                    return;
            }

            FILE* file = fopen(op2->str, mode);
            op1->ptr = (void*)file;
            break;
        }
        case OP_FCLOSE:
            fclose(op1->ptr);
            break;
        case OP_FREADB:
            //NOTE: sets test flag if failed to read
            if(!fread(&op1, 1, 1, op2->ptr))
                memory->test_flag = true;
            break;
        case OP_FWRITEB:
            //NOTE: sets test flag if failed to write
            if (!fwrite(&op2, 1, 1, op1->ptr))
                memory->test_flag = true;
            break;
        // stack
        case OP_PUSH8:
            stack_push(memory, *op1, SIZE_8);
            break;
        case OP_PUSH16:
            stack_push(memory, *op1, SIZE_16);
            break;
        case OP_PUSH32:
            stack_push(memory, *op1, SIZE_32);
            break;
        case OP_PUSH64:
            stack_push(memory, *op1, SIZE_64);
            break;
        case OP_POP8:
            op1->u64 = stack_pop(memory, SIZE_8).u64;
            break;
        case OP_POP16:
            op1->u64 = stack_pop(memory, SIZE_16).u64;
            break;
        case OP_POP32:
            op1->u64 = stack_pop(memory, SIZE_32).u64;
            break;
        case OP_POP64:
            op1->u64 = stack_pop(memory, SIZE_64).u64;
            break;
        case OP_STACKG:
            op1->ptr = memory->stack_ptr;
            break;
        case OP_STACKS:
            memory->stack_ptr = op1->ptr;
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
        case OP_SHL:
            op1->i64 <<= op2->u64;
            break;
        // unsigned
        case OP_ROR:
            op1->u64 = rotate_right(op1->u64, op2->u64);
            break;
        case OP_ROL:
            op1->u64 = rotate_left(op1->u64, op2->u64);
            break;
        case OP_SHR:
            op1->i64 >>= op2->u64;
            break;
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
        case OP_F32ADD:
            op1->f32 += op2->f32;
            break;
        case OP_F32SUB:
            op1->f32 -= op2->f32;
            break;
        case OP_F32NEG:
            op1->f32 = -op2->f32;
            break;
        case OP_F32MUL:
            op1->f32 *= op2->f32;
            break;
        case OP_F32DIV:
            op1->f32 /= op2->f32;
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
            break;
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
            memory->test_flag = op1->f64 == 0.0;
            break;
        case OP_CMPFI:
            memory->test_flag = op1->f64 == INFINITY;
            break;
        case OP_CMPFN:
            memory->test_flag = op2->f64 == NAN;
            break;
        case OP_CMPFG:
            memory->test_flag = op1->f64 > op2->f64;
            break;
        case OP_CMPFL:
            memory->test_flag = op1->f64 < op2->f64;
            break;
        // float comparisons
        case OP_CMPF32Z:
            memory->test_flag = op1->f32 == 0.0f;
            break;
        case OP_CMPF32I:
            memory->test_flag = op1->f32 == INFINITY;
            break;
        case OP_CMPF32N:
            memory->test_flag = op1->f32 == NAN;
            break;
        case OP_CMPF32G:
            memory->test_flag = op1->f32 > op2->f32;
            break;
        case OP_CMPF32L:
            memory->test_flag = op1->f32 < op2->f32;
            break;

    // === BRANCHING === //
        case OP_JMP:
            memory->instr_ptr = op1->i64 - 1;
            break;
        case OP_JIF:
            memory->instr_ptr = memory->test_flag 
                              ? memory->instr_ptr
                              : op1->u64 - 1;
            break;
        case OP_CALL:
#ifdef SEPARATE_CALL_STACK
            call_stack_push(&memory->call_stack, memory->instr_ptr);
#else
        {
            value_t pc;
            pc.u64 = memory->instr_ptr;
            stack_push(memory, pc, sizeof(memory->instr_ptr));
        }
#endif
            memory->instr_ptr = op1->u64 - 1;
            break;
        case OP_RET:
#ifdef SEPARATE_CALL_STACK
            memory->instr_ptr = call_stack_pop(&memory->call_stack);
#else
            memory->instr_ptr = stack_pop(memory, sizeof(memory->instr_ptr)).i64;
#endif
            break;
    // === OTHER === //
        case OP_HALT:
            memory->return_value = op1->i64;
            memory->halt = true;
            break;
        case OP_SYSCALL:
            //TODO: implement
            assert(false && "OP_SYSCALL undefined");
            break;
        case OP_DEBUG:
            print_stack_trace(memory, op1->u32);
            break;
        case OP_PUTCH:
            putc(op1->i8, op2->u64 ? stdout : stderr);
            break;
        case OP_GETCH:
            op1->u64 = getc(stdin);
            break;
    // === DEFAULT === //
        default:
            report_error(INVALID_OPCODE, NULL);
            break;
    }
}
