#include "instruction.h"

operand_t create_empty_operand(void)
{
    operand_t result;

    result.type          = AM_NONE;
    result.immediate.i64 = 0;
    result.base_reg      = 0;
    result.index_reg     = 0;
    result.scale         = 0;

    return result;
}

operand_t create_operand(addr_mode_t type, reg_id_t reg, reg_id_t idx_reg, value_t imm, uint8_t scale)
{
    operand_t result;

    result.immediate.i64 = imm.i64;
    result.type          = type;
    result.base_reg      = reg;
    result.index_reg     = idx_reg;
    result.scale         = scale;

    return result;
}

instruction_t create_instruction(opcode_t instr, word_size_t size, operand_t op1, operand_t op2)
{
    instruction_t result;

    result.operands[0] = op1;
    result.operands[1] = op2;
    result.instr       = instr;
    result.size        = size;

    return result;
}

