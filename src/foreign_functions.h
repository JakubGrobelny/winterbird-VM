/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef FOREIGN_FUNCTIONS_H
#define FOREIGN_FUNCTIONS_H

#include "vmsyscalls.h"

typedef union
{
    void* ptr;
    void(*void_void)(void);

} funptr_t;

typedef void(*void_void_funptr_t)(void);

funptr_t import_foreign_function(char* libname, char* functname);

#endif
