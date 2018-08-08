/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/#include "vmsyscalls.h"

#include <stdio.h>

void free_allocs(memory_t* memory)
{
    for (size_t i = 0; i < memory->allocated_ptrs.size; i++)
        tracked_free(&memory->allocated_ptrs, memory->allocated_ptrs.ptrs[i].ptr);

}

syscall_t syscalls[] = {
    free_allocs
};
