/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "interpreter.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    //TODO: FIXME:
    // change to proper argv parsing
    assert(argc == 2);

    const size_t DEFAULT_STACK_SIZE_IN_MB = 8;
    
    memory_t memory;

    //TODO: get argc and argv from parsing

    if (!initialize_memory(&memory, DEFAULT_STACK_SIZE_IN_MB, argc - 2, argv))
    {
        fprintf(stderr, "Failed to initialize memory!\n");
        free_memory(&memory);
        return INIT_ERROR;
    }

    if (!load_program(&memory, argv[1]))
    {
        fprintf(stderr, "Failed to load file \"%s\"\n", argv[1]);
        free_memory(&memory);
        return INIT_ERROR;
    }

    run_bytecode(&memory);

#ifndef NDEBUG
    print_stack_trace(&memory, PST_REG | PST_DATA | PST_ALLOC | PST_STACK);
#endif

    free_memory(&memory);
    return memory.return_value;
}
