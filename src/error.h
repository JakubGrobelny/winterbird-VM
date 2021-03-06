/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

typedef enum
{
    INIT_ERROR = -1,
    NONE = 0,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    INVALID_OPCODE,
    INVALID_OPERAND_TYPE,
    INVALID_INSTRUCTION_SIZE,
    DIVISION_BY_ZERO,
    INVALID_FILE_MODE,
    UNALLOCATED_FREE,
    ALLOC_FAILURE,
    DLOPEN_FAIL,
    DLSYM_FAIL,
    HANDLE_LIST_PUSH_FAIL,
    FUNPTR_LIST_PUSH_FAIL,
    INVALID_SYSCALL,
    INVALID_RMVLIB_ARG

} error_flag_t;

typedef struct
{
    error_flag_t flag;
    char*        description;

} error_t;

error_t error;

bool was_error(void);
void throw_error(void);
void report_error(error_flag_t flag, char* desc);

#endif
