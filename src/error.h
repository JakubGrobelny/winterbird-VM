#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

typedef enum
{
	INIT_ERROR = -1,
	NONE = 0,
	STACK_OVERFLOW,
	STACK_UNDERFLOW,
	INVALID_OPCODE,
	INVALID_OPERAND_TYPE,
	INVALID_INSTRUCTION_SIZE,
	DIVISION_BY_ZERO

} error_flag_t;

typedef struct
{
	error_flag_t flag;
	char*		 description;

} error_t;

error_t error;

inline bool was_error();
inline void throw_error();
inline void report_error(error_flag_t flag, char* desc);

#endif