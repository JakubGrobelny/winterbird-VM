/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef PROGRAM_H
#define PROGRAM_H

#include "defines.h"
#include "instruction.h"

typedef struct
{
    byte_t*         data;
    instruction_t*  text;

    size_t data_size;
    size_t text_size;

} program_t;

void free_program(program_t* program);
void init_empty_program(program_t* program);

#endif
