/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef CALL_STACK_H
#define CALL_STACK_H

#include "defines.h"

typedef struct
{
    ptr_t* stack;
    size_t capacity;
    size_t size;

} call_stack_t;

bool init_call_stack(call_stack_t* stack);
void call_stack_push(call_stack_t* stack, ptr_t pc);
ptr_t call_stack_pop(call_stack_t* stack);
void free_call_stack(call_stack_t* stack);

#endif
