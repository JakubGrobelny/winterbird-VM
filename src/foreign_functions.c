/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#include "foreign_functions.h"
#include <dlfcn.h>
#include "error.h"

funptr_t import_foreign_function(char* libname, char* functname)
{
    void* handle;

    funptr_t functptr;
    functptr.void_void = NULL;

    handle = dlopen(libname, RTLD_LAZY);
    if (!handle)
    {
        report_error(DLOPEN_FAIL, libname);
        return functptr;
    }

    dlerror();

    functptr.ptr = dlsym(handle, functname);

    char* error = dlerror();
    
    if (error)
    {
        report_error(DLSYM_FAIL, error);
        return functptr;
    }

    //dlclose(handle);

    return functptr;
}
