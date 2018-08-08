#include "vmsyscalls.h"

#include <stdio.h>
#include <dlfcn.h>

void hello_world(memory_t* memory)
{
    printf("Hello world!\n");
}

syscall_t syscalls[] = {
    hello_world
};
