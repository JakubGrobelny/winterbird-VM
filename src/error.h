#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

typedef enum
{
	NONE = 0,
	STACK_OVERFLOW,
	STACK_UNDERFLOW

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