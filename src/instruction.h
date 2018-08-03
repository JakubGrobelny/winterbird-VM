#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "value.h"

typedef enum
{
    //mem:
    OP_MOV, OP_ALLOC, OP_FREE, OP_SWAP, OP_FOPEN, OP_FCLOSE, OP_FREADB, OP_PUSH, OP_POP,
    //alu:
    OP_FTOI, OP_ITOF, // casts
    OP_ADD, OP_SUB, OP_NEG, OP_MUL, OP_DIV, OP_MOD, OP_ROR, OP_ROL, OP_SHR, OP_SHL, // signed integer
    OP_UMUL, OP_UDIV, OP_UMOD, OP_USHR, OP_USHL, // unsigned integer
    OP_FADD, OP_FSUB, OP_FNEG, OP_FMUL, OP_FDIV, // float
    OP_AND, OP_OR, OP_XOR, OP_NOT, // logical
    OP_BSET, OP_BCLR, OP_DEC, OP_INC, // other
    //test:
    OP_NTEST, OP_STEST, OP_GTEST, 
    OP_CMPZ, OP_CMPG, OP_CMPL, OP_CMPEQ, // signed
    OP_CMPFZ, OP_CMPFG, OP_CMPFL, OP_CMPFEQ, OP_CMPFI, OP_CMPFN, // float
    OP_CMPUL, OP_CMPUG,// unsigned
    //branch:
    OP_JMP, OP_JIF, OP_CALL, OP_RET,
    // other:
    OP_SYSCALL, OP_DEBUG, OP_HALT,
    // new instructions:
    OP_FWRITEB,
     
    /*
    Memory:

        //lw {REG, ADDRESS} 		- loads a word to register from address
        //sw {ADDRESS, REG} 		- stores a word at an address from given register
        mov {REG, REG}	  			- copies a value from second register to first register, MUST SPECIFY SIZE
        alloc {ADDRESS, UINT32}	 	- allocates memory of size given as second operand 
                                        and stores the pointer in first operand
        free {ADDRESS}				- frees allocated memory at pointer from operand address
        swap {LOCATION, LOCATION} 	- swaps the contents of two operands
        fopen {REG, NAME}			- opens file indicated by second operand and stores pointer in first, 
                                        mode to be used when opening should be given in first register
        fclose {REG}				- closes file pointed by operand
        freadb {REG}, {REG}			- reads byte from file from first register and stores it in second register
        fwriteb {REG}, {REG}		- writes byte from second operand to file from first operand
        push {REG}
        pop  {REG}

    ALU:

        ftoi {REG, REG} - casts float to integer
        itof {REG, REG} - casts integer to float

        add, sub, neg, mul, div, mod, 
        umul, udiv, umod, 
        dec, inc, 
        ror, rol, shr, shl, ushr, ushl, 
        and, or, not, xor, 
        fadd, fsub, fneg, fmul, fdiv,

        bset {REG, VAL} - sets the bit of the first operand indicated by second operand
        bclr {REG, VAL} - clears the bit of the first operand indicated by second operand

    Test:

        ntest				- negates the value of test flag
        stest	{VAL}		- sets the value of test flag to given value
        gtest   {VAL}		- sets the value to value of test flag
        cmpz 	{VAL}		- sets test flag if operand is zero
        cmpeq   {VAL, VAL}	- sets test flag if operands are equal
        cmpg  	{VAL, VAL}	- sets test flag if first operand is greater
        cmpl  	{VAL, VAL}	- sets test flag if first operand is lesser
        cmpfz	{VAL}		- sets test flag if float is zero
        cmpfi	{VAL}		- sets test flag if float is INF
        cmpfn	{VAL}		- sets test flag if float is NaN
        cmpfeq	{VAL, VAL}	- sets test if floats are equal
        cmpfg	{VAL, VAL}	- sets test if first float is greater
        cmpfl	{VAL, VAL}  - sets test if first float is lesser
        cmpul	{VAL, VAL}  - sets test flag if first unsigned is lesser
        cmpug   {VAL, VAL}  - sets test flag if first unsigned is lesser

    Branch:

        //NOTE: addresses are absolute.
        jmp 	{ADDRESS} 		- always jumps
        jif 	{ADDRESS} 		- jumps if test flag is set
        call 	{ADDRESS} 		- saves instruction pointer to the stack
        ret 	{} 				- jumps to the address from the stack

    Special:

        halt    {VAL}		 - quits the program with given code
        syscall {ID} 		 - calls syscall
        debug	{FLAG}	 	 - halts the program and prints stack and registers

    */

} opcode_t;

typedef enum
{
    /* addressing modes: 
        %reg,							- AM_REG
        $imm,							- AM_IMMEDIATE
        *(%reg + %idx * $scale + $imm)	- AM_REG_DEREF
        *($imm)							- AMM_IMM_DEREF //ONLY FOR .DATA
    */

    AM_NONE,
    AM_REG,
    AM_REG_DEREF,
    AM_IMM,
    AM_IMM_DEREF // ONLY FOR ACCESSING .DATA

} addr_mode_t;

typedef struct
{
    value_t 		immediate;
    addr_mode_t 	type;
    reg_id_t 		base_reg;
    reg_id_t 		index_reg;
    uint8_t 		scale;

} operand_t;

typedef struct
{
    operand_t 	operands[2];
    opcode_t 	instr;
    word_size_t size;

} instruction_t;

operand_t create_empty_operand();
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