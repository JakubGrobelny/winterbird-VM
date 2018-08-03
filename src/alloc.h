#ifndef ALLOC_H
#define ALLOC_H

#include "defines.h"

//TODO: use tree dictionary in the future
//NOTE: can be used in the future if I want to manage heap myself

typedef struct
{
	void*  ptr;
	size_t size;

} alloc_ptr_t;

typedef struct
{
	alloc_ptr_t* ptrs;
	size_t 		 capacity;
	size_t 		 size;

} alloc_array_t;

bool init_alloc_array(alloc_array_t* array);
void free_alloc_array(alloc_array_t* array);

#endif