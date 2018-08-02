//#define NDEBUG
#include "error.h"

#include <assert.h>

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  	byte_t;
typedef uint64_t 	size_t;
typedef uint64_t 	ptr_t;
typedef uint64_t 	size_t;
typedef uint64_t 	ptr_t;
typedef uint8_t  	reg_id_t;

#define REG_NUMBER (size_t)128

#define NULL (void*)0

#define MEGABYTE (uint64_t)1048576