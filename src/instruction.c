#include "instruction.h"

instruction_t create_instruction(opcode_t opcode, 
                                 uint32_t opmode,
                                 value_t  op1, 
                                 value_t  op2)
{
    instruction_t instr = {{op1, op2}, opcode, opmode};
    return instr;
}
