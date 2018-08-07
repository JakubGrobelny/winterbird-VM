/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "program.h"
#include <stdlib.h>

void free_program(program_t* program)
{
    free(program->data);
    free(program->text);
    program->data_size = 0;
    program->text_size = 0;
}

void init_empty_program(program_t* program)
{
    program->data = NULL;
    program->text = NULL;
    program->data_size = 0;
    program->text_size = 0;
}
