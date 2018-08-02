#ifndef VALUE_H
#define VALUE_H

#include "defines.h"

typedef enum 
{
	SIZE_64 = 8,
	SIZE_32 = 4,
	SIZE_16 = 2,
	SIZE_8  = 1

} word_size_t;

typedef union
{
	void*		ptr;
	double   	f64;
	int64_t  	i64;
	int32_t     i32;
	int16_t     i16;
	int8_t      i8;

} value_t;

#endif