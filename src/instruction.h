#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "value.h"
#include "opcodes.h"

typedef enum
{
    /* addressing modes: 
        %reg,                            - AM_REG
        $imm,                            - AM_IMMEDIATE
        *(%reg + %idx * $scale + $off)   - AM_REG_DEREF
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
    // register encoding:
    // 16 bit scale, 16 bit offset, 16 bit idx, 16 bit reg

    opcode_t    opcode;
    uint32_t    op_modes;
    value_t     operands[2];

} instruction_t;

instruction_t create_instruction(opcode_t opcode, 
                                 uint32_t opmode, 
                                 value_t  op1, 
                                 value_t  op2);

#endif
