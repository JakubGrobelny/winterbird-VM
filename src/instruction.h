#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "value.h"
#include "opcodes.h"

typedef enum
{
    /* addressing modes: 
        %reg,                            - AM_REG
        $imm,                            - AM_IMMEDIATE
        *(%reg + %idx * $scale + $imm)   - AM_REG_DEREF
        *($imm)                          - AMM_IMM_DEREF //ONLY FOR .DATA
    */

    AM_NONE,
    AM_REG,
    AM_REG_DEREF,
    AM_IMM,
    AM_IMM_DEREF // ONLY FOR ACCESSING .DATA

} addr_mode_t;

typedef struct
{
    value_t      immediate;
    addr_mode_t  type;
    reg_id_t     base_reg;
    reg_id_t     index_reg;
    uint8_t      scale;

} operand_t;

typedef struct
{
    operand_t   operands[2];
    opcode_t    instr;
    word_size_t size;

} instruction_t;

operand_t create_empty_operand(void);
operand_t create_operand(addr_mode_t type, 
                         reg_id_t reg, 
                         reg_id_t idx_reg, 
                         value_t imm, 
                         uint8_t scale);
instruction_t create_instruction(opcode_t instr, 
                                 word_size_t size, 
                                 operand_t op1, 
                                 operand_t op2);

#endif