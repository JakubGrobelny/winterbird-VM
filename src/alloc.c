#include "alloc.h"
#include <stdlib.h>

bool init_alloc_array(alloc_array_t* array)
{
    array->capacity = 8;
    array->size 	= 0;
    array->ptrs		= malloc(array->capacity);

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

