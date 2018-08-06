#include "util.h"

uint64_t rotate_left(uint64_t val, uint64_t amount)
{
    uint64_t out = val >> (64 - amount);
    uint64_t shifted = val << amount;

    return out | shifted;
}

uint64_t rotate_right(uint64_t val, uint64_t amount)
{
    uint64_t out = val << (64 - amount);
    uint64_t shifted = val >> amount;

    return out | shifted;
}

void fprintf_bytes64(FILE* stream, uint64_t val)
{
    fprintf(stream, "%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx",
                    (uint8_t)(val >> 56),
                    (uint8_t)(val >> 48),
                    (uint8_t)(val >> 40),
                    (uint8_t)(val >> 32),
                    (uint8_t)(val >> 24),
                    (uint8_t)(val >> 16),
                    (uint8_t)(val >> 8),
                    (uint8_t)val);
}

void fprintf_bytes32(FILE* stream, uint32_t val)
{
    fprintf(stream, "%02hhx %02hhx %02hhx %02hhx",
                    (uint8_t)(val >> 24),
                    (uint8_t)(val >> 16),
                    (uint8_t)(val >> 8),
                    (uint8_t)val);
}

void fprintf_byte(FILE* stream, uint8_t val)
{
    fprintf(stream, "%02hhx", val);
}
