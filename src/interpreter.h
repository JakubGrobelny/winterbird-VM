/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "memory.h"

void run_bytecode(memory_t* memory);
void run_instruction(memory_t* memory, instruction_t* instruction);

#endif
