/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "call_stack.h"
#include <stdlib.h>

static const size_t DEFAULT_CAPACITY = 16;

bool init_call_stack(call_stack_t* stack)
{
    stack->capacity = DEFAULT_CAPACITY;
    stack->size     = 0;

    stack->stack = malloc(sizeof(ptr_t) * stack->capacity);

    return stack->stack;
}

void call_stack_push(call_stack_t* stack, ptr_t pc)
{
    if (stack->size >= stack->capacity)
    {
        stack->capacity *= 2;
        ptr_t* new_stack_ptr = realloc(stack->stack, stack->capacity);
        
        if (!new_stack_ptr)
            report_error(ALLOC_FAILURE, NULL);

        stack->stack = new_stack_ptr;
    }

    stack->stack[stack->size++] = pc;
}

ptr_t call_stack_pop(call_stack_t* stack)
{
    if (!stack->size)
        report_error(STACK_OVERFLOW, "call stack underflow");

    return stack->stack[stack->size--];
}

void free_call_stack(call_stack_t* stack)
{
    stack->capacity = 0;
    stack->size     = 0;
    free(stack->stack);
}
