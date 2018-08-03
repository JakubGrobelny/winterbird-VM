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

