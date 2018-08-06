#include "alloc.h"
#include <stdlib.h>

bool init_alloc_array(alloc_array_t* array)
{
    array->capacity = 8;
    array->size     = 0;
    array->ptrs     = malloc(array->capacity);

    return array->ptrs;
}

void free_alloc_array(alloc_array_t* array)
{
    for (size_t i = 0; i < array->size; i++)
        if (array->ptrs[i].size)
            free(array->ptrs[i].ptr);

    free(array->ptrs);
    array->capacity = 0;
    array->size = 0;
}

void* tracked_alloc(alloc_array_t* array, size_t size)
{
    if (array->capacity <= array->size)
    {
        array->capacity *= 2;
        alloc_ptr_t* new_array = realloc(array->ptrs,
                                         array->capacity);

        if (!new_array)
        {
            array->capacity /= 2;
            return NULL;
        }

        array->ptrs = new_array;
    }

    alloc_ptr_t new_ptr;
    new_ptr.size = size;
    new_ptr.ptr = malloc(size);

    array->ptrs[array->size++] = new_ptr;

    return new_ptr.ptr;
}

void tracked_free(alloc_array_t* array, void* ptr)
{
    for (size_t i = 0; i < array->size; i++)
    {
        alloc_ptr_t* alloc_ptr = &array->ptrs[i];

        if (alloc_ptr->ptr == ptr)
        {
            alloc_ptr->size = 0;
            free(ptr);
            return;
        }
    }

    report_error(UNALLOCATED_FREE, NULL);
}
