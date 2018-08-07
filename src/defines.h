/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

//#define NDEBUG
//#define NO_TRACK_ALLOC
//#define DEBUG_EXTRA
#define SEPARATE_CALL_STACK

#include "error.h"
#include "util.h"

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t   byte_t;
typedef uint64_t  size_t;
typedef uint64_t  ptr_t;
typedef uint64_t  size_t;
typedef uint64_t  ptr_t;

#define REG_NUMBER (size_t)128

#ifndef NULL
    #define NULL (void*)0
#endif

#define MEGABYTE (uint64_t)1048576

#define file_exit(n) {fclose(n); return false;}
