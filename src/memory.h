#ifndef MEMORY_H
#define MEMORY_H

#include "defines.h"
#include "value.h"
#include "instruction.h"
#include "program.h"
#include "alloc.h"
#include "call_stack.h"

typedef struct
{
    value_t registers[128];

    byte_t*   stack;
    size_t    stack_size;
    byte_t*   stack_ptr;

    ptr_t     instr_ptr;
    uint64_t  test_flag;

    program_t program_data;

    int32_t   return_value;
    bool      halt;

    byte_t*   memprep_ptr;
    int64_t   memprep_val;

    #ifdef SEPARATE_CALL_STACK
        call_stack_t  call_stack;
    #endif
    
    #ifndef NO_TRACK_ALLOC
        alloc_array_t allocated_ptrs;
    #endif

    size_t argc;
    char** argv;

} memory_t;

const int sz = sizeof(memory_t);

typedef enum
{
    PST_REG   = 1,
    PST_STACK = 2,
    PST_DATA  = 4,
    PST_TEXT  = 8,
    PST_ALLOC = 16

} stack_trace_args_t;

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes, size_t argc, char** argv);
bool load_program(memory_t* memory, char* path);
void free_memory(memory_t* memory);
value_t* get_pointer_from_operand(memory_t* memory, instruction_t* instruction, uint8_t operand_id);
void stack_push(memory_t* memory, value_t value, word_size_t size);
value_t stack_pop(memory_t* memory, word_size_t size);
void print_stack_trace(memory_t* memory, uint32_t what);

#endif