#ifndef OPCODES_H
#define OPCODES_H

typedef enum
{
    OP_MOV,     OP_ALLOC,   OP_FREE,   OP_SWAP,     OP_FOPEN,
    OP_FCLOSE,  OP_FREADB,  OP_PUSH,   OP_POP,      OP_FTOI, 
    OP_ITOF,    OP_ADD,     OP_SUB,    OP_NEG,      OP_MUL, 
    OP_DIV,     OP_MOD,     OP_ROR,    OP_ROL,      OP_SHR,
    OP_SHL,     OP_UMUL,    OP_UDIV,   OP_UMOD,     OP_USHR, 
    OP_USHL,    OP_FADD,    OP_FSUB,   OP_FNEG,     OP_FMUL, 
    OP_FDIV,    OP_AND,     OP_OR,     OP_XOR,      OP_NOT, 
    OP_BSET,    OP_BCLR,    OP_DEC,    OP_INC,      OP_NTEST,
    OP_STEST,   OP_GTEST,   OP_CMPZ,   OP_CMPG,     OP_CMPL,
    OP_CMPEQ,   OP_CMPFZ,   OP_CMPFG,  OP_CMPFL,    OP_FWRITEB, 
    OP_CMPFI,   OP_CMPFN,   OP_CMPUL,  OP_CMPUG,    OP_JMP, 
    OP_JIF,     OP_CALL,    OP_RET,    OP_SYSCALL,  OP_DEBUG, 
    OP_HALT,    OP_STACKG,  OP_STACKS, OP_GETCH,    OP_PUTCH
     
     // described in README.mb

} opcode_t;

#endif