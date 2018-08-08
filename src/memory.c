
/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes,
                       size_t argc, char** argv)
{
    for (size_t i = 0; i < REG_NUMBER; i++)
        memory->registers[i].i64 = 0;

    memory->stack_size = stack_megabytes * MEGABYTE;
    memory->stack = calloc(memory->stack_size, sizeof(byte_t));

    memory->stack_ptr = memory->stack;

    memory->test_flag = 0;
    memory->instr_ptr = 0;    

    memory->return_value = -1;
    memory->halt         = false;

    memory->memprep_ptr = NULL;
    memory->memprep_val = 0;

    init_empty_program(&memory->program_data);

    #ifndef NO_TRACK_ALLOC
        if (!init_alloc_array(&memory->allocated_ptrs))
            return false;
    #endif

    #ifdef SEPARATE_CALL_STACK
        if (!init_call_stack(&memory->call_stack))
            return false;
    #endif

    memory->argc = argc;
    memory->argv = argc
                 ? argv
                 : NULL;

    return memory->stack;
}

void stack_push(memory_t* memory, value_t value, word_size_t size)
{
    if (memory->stack_ptr + size > memory->stack + memory->stack_size)
    {
        report_error(STACK_OVERFLOW, NULL);
        return;
    }

    value_t* ptr = (void*)memory->stack_ptr;

    switch (size)
    {
        case SIZE_8:
            ptr->u8 = value.u8;
            break;
        case SIZE_16:
            ptr->u16 = value.u16;
            break;
        case SIZE_32:
            ptr->u32 = value.u32;
            break;
        case SIZE_64:
            ptr->u64 = value.u64;
            break;
        default:
            report_error(INVALID_INSTRUCTION_SIZE, NULL);
            break;
    }

    memory->stack_ptr += size;
}

value_t stack_pop(memory_t* memory, word_size_t size)
{
    if (memory->stack_ptr < memory->stack + size)
    {
        report_error(STACK_UNDERFLOW, NULL);
        value_t return_val;
        return_val.u64 = 0;
        return return_val;
    }

    memory->stack_ptr -= size;
    value_t* ptr = (void*)memory->stack_ptr;
    value_t result = {0};

    switch (size)
    {
        case SIZE_8:
            result.u8 = ptr->u8;
            break;
        case SIZE_16:
            result.u16 = ptr->u16;
            break;
        case SIZE_32:
            result.u32 = ptr->u32;
            break;
        case SIZE_64:
            result.u64 = ptr->u64;
            break;
        default:
            report_error(INVALID_INSTRUCTION_SIZE, NULL);
            break;
    }

    return result;
}

void stack_duplicate(memory_t* memory, size_t bytes)
{
    if (memory->stack_ptr + bytes > memory->stack + memory->stack_size)
    {
        report_error(STACK_OVERFLOW, NULL);
        return;
    }

    if (memory->stack_ptr - bytes < memory->stack)
    {
        report_error(STACK_UNDERFLOW, NULL);
        return;
    }

    memcpy(memory->stack_ptr, memory->stack_ptr - bytes, bytes);
    memory->stack_ptr += bytes;
}

void free_memory(memory_t* memory)
{
    free(memory->stack);
    memory->stack_size = 0;
    memory->stack_ptr = NULL;
    free_program(&memory->program_data);
    
    #ifndef NO_TRACK_ALLOC
        free_alloc_array(&memory->allocated_ptrs);
    #endif
    
    #ifdef SEPARATE_CALL_STACK
        free_call_stack(&memory->call_stack);
    #endif
}

value_t* get_pointer_from_operand(memory_t* memory, instruction_t* instruction, uint8_t operand_id)
{
    value_t* operand = &instruction->operands[operand_id];
    uint16_t mode = operand_id 
                  ? (uint16_t)(instruction->op_modes >> 16)
                  : (uint16_t)instruction->op_modes & 0x0000FFFF;

    switch (mode)
    {
        case AM_REG:
            return &memory->registers[operand->u16];
        case AM_IMM:
            return operand;
        case AM_REG_DEREF:
        {
            uint16_t reg   =            operand->u16;
            uint16_t idx   = (uint16_t)(operand->u32 >> 16);
            uint16_t scale = (uint16_t)(operand->u64 >> 32);
            uint16_t off   = (uint16_t)(operand->u64 >> 48);

            value_t reg_val = memory->registers[reg];
            value_t idx_val = memory->registers[idx];
            
            uint8_t* ptr = (uint8_t*)reg_val.ptr + (idx_val.u64 << scale) + off;
            return (value_t*)ptr;
        }
        case AM_IMM_DEREF:
            return (value_t*)(memory->program_data.data + operand->u64);
        default:
            report_error(INVALID_OPERAND_TYPE, NULL);
            return NULL;
        case AM_NONE:
            return NULL;
        
    }
}

bool load_program(memory_t* memory, char* path)
{
    program_t* program = &memory->program_data;

    FILE* file;

    file = fopen(path, "rb");

    if (!file)
        return false;

    /* Winterbird bytecode file (.wbx):
        8 bytes                     -- size of unitialized .data section
        8 bytes                     -- size of initialized .data section
        8 bytes                     -- size of .text (number of instructions)
        X bytes                     -- bytes of initialized .data section
        Y bytes                     -- instructions
        ? bytes                     -- optional flags
    */

   // size of uninitialized data
   size_t uninit_data_size;

    // reading uninit_data_size
    if (!fread(&uninit_data_size, sizeof(size_t), 1, file))
        file_exit(file)
    // reading initialized data size
    if (!fread(&program->data_size, sizeof(size_t), 1, file))
        file_exit(file)
    // reading instruction count
    if (!fread(&program->text_size, sizeof(size_t), 1, file))
        file_exit(file);

    // allocating space for both uninitialized and initialized data
    if (program->data_size + uninit_data_size)
    {
        // we want data size to be mutliple of 8 bytes
        if ((program->data_size + uninit_data_size) % sizeof(uint64_t))
        {
            size_t diff = (program->data_size + uninit_data_size) % sizeof(uint64_t);
            diff = 8 - diff;
            uninit_data_size += diff;
        }

        program->data = calloc(program->data_size + uninit_data_size, sizeof(uint8_t));
        if (!program->data)
            file_exit(file)
        // reading data
        if (fread(program->data + uninit_data_size, 1, program->data_size, file) 
            != program->data_size)
            file_exit(file)
        // setting overall data size to be the sum of init and uninit data size
        program->data_size += uninit_data_size;
    }
    
    if (program->text_size)
    {
        // allocating space for instructions
        program->text = malloc(program->text_size * sizeof(instruction_t));
        if (!program->text)
            file_exit(file)
        // reading instruction data
        if (fread(program->text, sizeof(instruction_t), program->text_size, file) 
            != program->text_size)
            file_exit(file)    
    }

    fclose(file);

    return true;
}

void print_stack_trace(memory_t* memory, uint32_t what)
{
    fprintf(stderr, "=====================================\n");
    if (what & PST_REG)
    {
        fprintf(stderr, "REGISTERS:\n");
        for (size_t i = 0; i < REG_NUMBER; i++)
            if (memory->registers[i].u64)
                fprintf(stderr, "$%3ld: 0x%016lx\n", i, memory->registers[i].u64);
        fprintf(stderr, "=====================================\n");
    }
    if (what & PST_STACK)
    {
        fprintf(stderr, "STACK_ADDR: 0x%16p\n", (void*)memory->stack);
        fprintf(stderr, "STACK_PTR:  0x%16p\n", (void*)memory->stack_ptr);
        fprintf(stderr, "STACK:\n");

        uint64_t* stack_ptr = (void*)memory->stack;
        size_t stack_size_in_words = memory->stack_ptr - memory->stack;
        stack_size_in_words = stack_size_in_words % (sizeof(uint64_t))
                            ? stack_size_in_words / 8 + 1
                            : stack_size_in_words / 8;

        for (size_t i = 0; i < stack_size_in_words; i++)
        {
            fprintf(stderr, "0x%16p: ", (void*)(stack_ptr + i));
            fprintf_bytes64(stderr, stack_ptr[i]);
            fprintf(stderr, "\n");
        }

        fprintf(stderr, "=====================================\n");
    }
    if (what & PST_DATA)
    {
        fprintf(stderr, "DATA:\n");

        uint64_t* data_ptr = (void*)memory->program_data.data;
        size_t data_size_in_words = memory->program_data.data_size;
        data_size_in_words = data_size_in_words % (sizeof(uint64_t))
                           ? data_size_in_words / 8 + 1
                           : data_size_in_words / 8;

        for (size_t i = 0; i < data_size_in_words; i++)
        {
            fprintf(stderr, "0x%16p: ", (void*)(data_ptr + i));
            fprintf_bytes64(stderr, data_ptr[i]);
            fprintf(stderr, "\n");
        }

        fprintf(stderr, "=====================================\n");
    }
    if (what & PST_TEXT)
    {
        fprintf(stderr, "TEXT:\n");

        for (size_t i = 0; i < memory->program_data.text_size; i++)
        {
            instruction_t* instr = &memory->program_data.text[i];

            //TODO: finish (maybe disassemble)
            fprintf(stderr, "0x%016lx: 0x%08x", i, instr->opcode);
        }
        fprintf(stderr, "=====================================\n");
    }
    if (what & PST_ALLOC)
    {
        fprintf(stderr, "ALLOC:\n");

        for (size_t i = 0; i < memory->allocated_ptrs.size; i++)
        {
            alloc_ptr_t* ptr = &memory->allocated_ptrs.ptrs[i];
            fprintf(stderr, "0x%16p: 0x%016lx\n", ptr->ptr, ptr->size);
            //TODO: add printing of the actual contents under the allocated ptr
        }
        fprintf(stderr, "=====================================\n");
    }

    fprintf(stderr, "RETURN_VALUE: 0x%08x\n", memory->return_value);
    fprintf(stderr, "=====================================\n");
}
