#ifndef VMSYSCALLS_H
#define VMSYSCALLS_H

#include "memory.h"

#define SYSCALLS_SIZE 1

typedef void (*syscall_t)(memory_t*);

extern syscall_t syscalls[];

#endif
