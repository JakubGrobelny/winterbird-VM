/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef VMSYSCALLS_H
#define VMSYSCALLS_H

#include <stdint.h>
#include <stdbool.h>

typedef unsigned long size_t;

typedef uint8_t* (*funct_ptr_t)(uint8_t*, uint64_t*, size_t);

typedef struct
{
    void** handles;
    size_t size;
    size_t capacity;

} lib_handles_list_t;

typedef struct
{
    funct_ptr_t ptr;
    uint32_t handle_id;

} funct_entry_t;

typedef struct
{
    funct_entry_t* entries;
    size_t size;
    size_t capacity;

} funct_ptrs_list_t;

bool init_lib_handles(lib_handles_list_t* handles);
void free_lib_handles(lib_handles_list_t* handles);
bool init_funct_list(funct_ptrs_list_t* ptrs);
void free_funct_ptrs(funct_ptrs_list_t* ptrs);

uint64_t reglib(char* libname, lib_handles_list_t* handles);
uint64_t regfun(uint64_t handle_id, char* functname, 
                lib_handles_list_t* handles, funct_ptrs_list_t* funct_ptrs);
void remove_lib(uint64_t handle_id, lib_handles_list_t* handles,
                                    funct_ptrs_list_t* funct_ptrs);

#endif
