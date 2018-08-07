/*

    Winterbird Virtual Machine
    Copyright (c) 2018 Jakub Grobelny
    Distributed under the MIT License
    http://opensource.org/licenses/MIT

*/

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>

uint64_t rotate_left(uint64_t val, uint64_t amount);
uint64_t rotate_right(uint64_t val, uint64_t amount);
void fprintf_bytes64(FILE* stream, uint64_t val);
void fprintf_bytes32(FILE* stream, uint32_t val);
void fprintf_byte(FILE* stream, uint8_t val);

#endif
