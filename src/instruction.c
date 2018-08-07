/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "instruction.h"

instruction_t create_instruction(opcode_t opcode, 
                                 uint32_t opmode,
                                 value_t  op1, 
                                 value_t  op2)
{
    instruction_t instr = {opcode, opmode, {op1, op2}};
    return instr;
}
