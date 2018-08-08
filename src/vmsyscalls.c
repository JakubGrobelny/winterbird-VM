/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "vmsyscalls.h"
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "error.h"

static const size_t INITIAL_HANDLES_CAPACITY = 16;
static const size_t INITIAL_PTR_LIST_CAPACITY = 16;

bool init_lib_handles(lib_handles_list_t* handles)
{
    handles->capacity = INITIAL_HANDLES_CAPACITY;
    handles->size = 0;
    handles->handles = malloc(sizeof(void*) * handles->capacity);

    return handles->handles;
}

void free_lib_handles(lib_handles_list_t* handles)
{
    for (size_t i = 0; i < handles->size; i++)
        dlclose(handles->handles[i]);

    free(handles->handles);
    handles->size = 0;
    handles->capacity = 0;
}

bool init_funct_list(funct_ptrs_list_t* ptrs)
{
    ptrs->capacity = INITIAL_PTR_LIST_CAPACITY;
    ptrs->size = 0;
    ptrs->entries = malloc(sizeof(funct_entry_t) * ptrs->capacity);

    return ptrs->entries;
}

void free_funct_ptrs(funct_ptrs_list_t* ptrs)
{
    for (size_t i = 0; i < ptrs->size; i++)
        ptrs->entries[i].ptr = NULL;

    free(ptrs->entries);
    ptrs->size = 0;
    ptrs->capacity = 0;
}

uint64_t reglib(char* libname, lib_handles_list_t* handles)
{
    void* handle;
    handle = dlopen(libname, RTLD_LAZY);
    
    if (!handle)
    {
        report_error(DLOPEN_FAIL, libname);
        return 0;
    }

    if (handles->size >= handles->capacity)
    {
        handles->capacity *= 2;
        void** new_ptr = realloc(handles->handles, sizeof(void*) * handles->capacity);
        
        if (!new_ptr)
        {
            free_lib_handles(handles);
            report_error(HANDLE_LIST_PUSH_FAIL, "failed to resize lib handles");
            return 0;
        }

        handles->handles = new_ptr;
    }

    handles->handles[handles->size++] = handle;

    return handles->size - 1;
}

uint64_t regfun(uint64_t handle_id, char* functname, 
                lib_handles_list_t* handles, funct_ptrs_list_t* funct_ptrs)
{
    if (handle_id >= handles->size)
    {
        report_error(INVALID_RMVLIB_ARG, NULL);
        return 0;
    }

    funct_ptr_t ptr;

    dlerror(); // clearing errors
    
    void* raw_ptr = dlsym(handles->handles[handle_id], functname);
    ptr = *(funct_ptr_t*)&raw_ptr;
    
    char* error = NULL;
    error = dlerror();

    if (error)
    {
        report_error(DLSYM_FAIL, error);
        return 0;
    }

    if (funct_ptrs->size >= funct_ptrs->capacity)
    {
        funct_ptrs->capacity *= 2;
        funct_entry_t* new_ptr = realloc(funct_ptrs->entries, 
                                        sizeof(funct_entry_t) * funct_ptrs->capacity);

        if (!new_ptr)
        {
            free_funct_ptrs(funct_ptrs);
            report_error(FUNPTR_LIST_PUSH_FAIL, "failed to resize funct handles");
            return 0;
        }

        funct_ptrs->entries = new_ptr;
    }

    funct_entry_t new_entry;
    new_entry.ptr = ptr;
    new_entry.handle_id = handle_id;

    funct_ptrs->entries[funct_ptrs->size++] = new_entry;

    return funct_ptrs->size - 1;
}

void remove_lib(uint64_t handle_id, lib_handles_list_t* handles, 
                                    funct_ptrs_list_t* funct_ptrs)
{
    if (handle_id >= handles->size)
    {
        report_error(INVALID_RMVLIB_ARG, NULL);
        return;
    }

    dlclose(handles->handles[handle_id]);

    // setting related pointers to NULL
    for (size_t i = 0; i < funct_ptrs->size; i++)
    {
        if (funct_ptrs->entries[i].handle_id == handle_id)
            funct_ptrs->entries[i].ptr = NULL;
    }
}