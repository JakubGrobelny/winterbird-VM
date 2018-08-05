#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool initialize_memory(memory_t* memory, uint64_t stack_megabytes)
{
    for (size_t i = 0; i < REG_NUMBER; i++)
        memory->registers[i].i64 = 0;

    memory->stack_size = stack_megabytes * MEGABYTE;
    memory->stack = malloc(memory->stack_size);

    memory->stack_ptr = memory->stack;

    memory->test_flag = 0;
    memory->instr_ptr = 0;    

    memory->return_value = -1;
    memory->halt         = false;

    init_empty_program(&memory->program_data);

#ifndef NO_TRACK_ALLOC
    if (!init_alloc_array(&memory->allocated_ptrs))
        return false;
#endif

    return memory->stack;
}

void stack_push(memory_t* memory, value_t value, word_size_t size)
{
    if (memory->stack_ptr + size > memory->stack_ptr + memory->stack_size)
        report_error(STACK_OVERFLOW, NULL);

    memcpy(memory->stack_ptr, &value + (8 - size), size);
    memory->stack_ptr += size;
}

value_t stack_pop(memory_t* memory, word_size_t size)
{
    if (memory->stack_ptr < memory->stack + size)
        report_error(STACK_UNDERFLOW, NULL);

    value_t result;
    result.i64 = 0x0;

    memory->stack_ptr -= size;
    memcpy(&result, memory->stack_ptr, size);

    return result;
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

    /* Winterbird bytecode file (.wb):
        8 bytes                     -- size of .data section
        X bytes                     -- bytes of .data section, at least one byte
        n * sizeof(instruction_t)   -- n instructions (at least one)
    */

    if (!fread(&program->data_size, sizeof(size_t), 1, file))
        file_exit(file)

    program->data = malloc(program->data_size);

    if (!program->data)
        file_exit(file)

    if (!fread(program->data, program->data_size, 1, file))
        file_exit(file)

    size_t text_capacity = 1;
    program->text_size = 0;
    program->text = malloc(text_capacity * sizeof(instruction_t));

    if (!program->text)
        file_exit(file)

    instruction_t instr;

    while (fread(&instr, sizeof(instruction_t), 1, file))
    {
        if (program->text_size >= text_capacity)
        {
            text_capacity *= 2;
            program->text = realloc(program->text, text_capacity * sizeof(instruction_t));

            if (!program->text)
                file_exit(file)
        }

        program->text[program->text_size++] = instr;
    }
    
    //FIXME:
    // removing uneccessary space (it slows down file loading but it happens only once so whatever)
    // instruction_t* new_ptr = realloc(program->text, program->text_size);
    // if (new_ptr)
        // program->text = new_ptr;

    fclose(file);

    return true;
}

void print_stack_trace(memory_t* memory, uint32_t what)
{
    const uint32_t reg   = 1;
    const uint32_t stack = 2;
    const uint32_t data  = 4;
    const uint32_t text  = 8;
    const uint32_t alloc = 16;

    if (what & reg)
    {
        fprintf(stderr, "REGISTERS:\n");
        for (size_t i = 0; i < REG_NUMBER; i++)
            if (memory->registers[i].u64)
                fprintf(stderr, "$%ld: %lx\n", i, memory->registers[i].u64);
        fprintf(stderr, "\n");
    }
    if (what & stack)
    {
        fprintf(stderr, "STACK:\n");
        uint32_t* address = (void*)memory->stack;

        while (address <= (uint32_t*)memory->stack_ptr)
            fprintf(stderr, "0x%lx: %x %x\n", address, *address++, *address++);
        fprintf(stderr, "\n");
    }
    if (what & data)
    {
        fprintf(stderr, "DATA:\n");
        uint32_t* address = (void*)memory->program_data.data;

        while (address <= (uint32_t*)(memory->program_data.data 
                                    + memory->program_data.data_size))
            fprintf(stderr, "0x%lx: %x %x\n", address, *address++, *address++);
        fprintf(stderr, "\n");
    }
    if (what & text)
    {
        fprintf(stderr, "TEXT:\n");

        for (size_t i = 0; i < memory->program_data.text_size; i++)
        {
            instruction_t* instr = &memory->program_data.text[i];

            //TODO: finish (maybe disassemble)
            fprintf(stderr, "0x%lx: %x", i, instr->opcode);
        }
    }
    if (what & alloc)
    {
        fprintf(stderr, "ALLOC:\n");

        for (size_t i = 0; i < memory->allocated_ptrs.size; i++)
        {
            alloc_ptr_t* ptr = &memory->allocated_ptrs.ptrs[i];
            fprintf(stderr, "0x%lx: %lx\n", ptr->ptr, ptr->size);
            //TODO: add printing of the actual contents under the allocated ptr
        }
    }
}

void* tracked_alloc(memory_t* memory, size_t size)
{
    if (memory->allocated_ptrs.capacity <= memory->allocated_ptrs.size)
    {
        memory->allocated_ptrs.capacity *= 2;
        alloc_ptr_t* new_array = realloc(memory->allocated_ptrs.ptrs,
                                         memory->allocated_ptrs.capacity);

        if (!new_array)
        {
            memory->allocated_ptrs.capacity /= 2;
            return NULL;
        }

        memory->allocated_ptrs.ptrs = new_array;
    }

    alloc_ptr_t new_ptr;
    new_ptr.size = size;
    new_ptr.ptr = malloc(size);

    memory->allocated_ptrs.ptrs[memory->allocated_ptrs.size++] = new_ptr;

    return new_ptr.ptr;
}

void tracked_free(memory_t* memory, void* ptr)
{
    for (size_t i = 0; i < memory->allocated_ptrs.size; i++)
    {
        alloc_ptr_t* alloc_ptr = &memory->allocated_ptrs.ptrs[i];

        if (alloc_ptr->ptr == ptr)
        {
            alloc_ptr->size = 0;
            free(ptr);
            return;
        }
    }

    report_error(UNALLOCATED_FREE, NULL);
}
