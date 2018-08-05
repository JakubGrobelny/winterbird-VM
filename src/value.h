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
    void*    ptr;
    char*    str;

    double   f64;
    float    f32;

    int64_t  i64;
    int32_t  i32;
    int16_t  i16;
    int8_t   i8;

    uint64_t u64;
    uint32_t u32;
    uint16_t u16;
    uint8_t  u8;

} value_t;

#endif