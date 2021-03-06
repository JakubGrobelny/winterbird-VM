/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef OPCODES_H
#define OPCODES_H

typedef enum
{
    OP_MOV64   = 0x0000,
    OP_MOV32   = 0x0100,
    OP_MOV16   = 0x0200,
    OP_MOV8    = 0x0300,
    OP_SWAP    = 0x0400,
    OP_LADDR   = 0x0500,
    OP_MEMPREP = 0x0600,
    OP_MEMCPY  = 0x0700,
    OP_MEMSET  = 0x0800,
    OP_MOV64IF = 0x0900,
    OP_MOV32IF = 0x0A00,
    OP_MOV16IF = 0x0B00,
    OP_MOV8IF  = 0x0C00,

    OP_PUSH64  = 0x0001,
    OP_PUSH32  = 0x0101,
    OP_PUSH16  = 0x0201,
    OP_PUSH8   = 0x0301,
    OP_DUPL    = 0x0401,

    OP_POP64   = 0x0002,
    OP_POP32   = 0x0102,
    OP_POP16   = 0x0202,
    OP_POP8    = 0x0302,

    OP_ALLOC   = 0x0003,
    OP_FREE    = 0x0103,
    
    OP_FOPEN   = 0x0004,
    OP_FCLOSE  = 0x0104,
    OP_FREADB  = 0x0204,
    OP_FWRITEB = 0x0304,

    OP_FTOI    = 0x0005,
    OP_ITOF    = 0x0105,
    OP_F32TOI  = 0x0205,
    OP_ITOF32  = 0x0305,

    OP_ADD     = 0x0006,
    OP_SUB     = 0x0106,
    OP_NEG     = 0x0206,
    OP_MUL     = 0x0306,
    OP_DIV     = 0x0406,
    OP_MOD     = 0x0506,
    OP_ROR     = 0x0606,
    OP_ROL     = 0x0706,
    OP_SHR     = 0x0806,
    OP_SHL     = 0x0906,

    OP_UMUL    = 0x0007,
    OP_UDIV    = 0x0107,
    OP_UMOD    = 0x0207,
    OP_USHR    = 0x0307,
    OP_USHL    = 0x0407,

    OP_FADD    = 0x0008,
    OP_FSUB    = 0x0108,
    OP_FNEG    = 0x0208,
    OP_FMUL    = 0x0308,
    OP_FDIV    = 0x0408,
    OP_F32ADD  = 0x0608,
    OP_F32SUB  = 0x0708,
    OP_F32NEG  = 0x0808,
    OP_F32MUL  = 0x0908,
    OP_F32DIV  = 0x0A08,

    OP_AND     = 0x0009,
    OP_OR      = 0x0109,
    OP_XOR     = 0x0209,
    OP_NOT     = 0x0309,

    OP_BSET    = 0x000A,
    OP_BCLR    = 0x010A,
    OP_DEC     = 0x020A,
    OP_INC     = 0x030A,

    OP_NTEST   = 0x000B,
    OP_STEST   = 0x010B,
    OP_GTEST   = 0x020B,

    OP_CMPZ    = 0x000C,
    OP_CMPG    = 0x010C,
    OP_CMPL    = 0x020C,
    OP_CMPEQ   = 0x030C,
    OP_CMPFZ   = 0x040C,
    OP_CMPFG   = 0x050C,
    OP_CMPFL   = 0x060C,
    OP_CMPFI   = 0x070C,
    OP_CMPFN   = 0x080C,
    OP_CMPUL   = 0x090C,
    OP_CMPUG   = 0x0A0C,
    OP_CMPF32Z = 0x0B0C,
    OP_CMPF32G = 0x0C0C,
    OP_CMPF32L = 0x0D0C,
    OP_CMPF32I = 0x0E0C,
    OP_CMPF32N = 0x0F0C,

    OP_JMP     = 0x000D,
    OP_JIF     = 0x010D,
    OP_CALL    = 0x020D,
    OP_RET     = 0x030D,

    OP_SYSCALL = 0x000E,
    OP_DEBUG   = 0x010E,
    OP_HALT    = 0x020E,
    OP_STACKG  = 0x030E,
    OP_STACKS  = 0x040E,
    OP_GETCH   = 0x050E,
    OP_PUTCH   = 0x060E,
    OP_ARGCG   = 0x070E,
    OP_ARGVG   = 0x080E,
    OP_REGLIB  = 0x090E,
    OP_REGFUN  = 0x0A0E,
    OP_RMVLIB  = 0x0B0E

    // described in README.mb

} opcode_t;

#endif
