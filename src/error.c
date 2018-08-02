#include "error.h"
#include <stdio.h>

error_t error = {NONE, NULL};

inline bool was_error()
{
	return *(error_flag_t*)(&error);
}

inline void report_error(error_flag_t flag, char* desc)
{
	error.flag = flag;
	error.description = desc;
}

inline void throw_error()
{
	print("Program exiting due to ");

	switch (error.flag)
	{
		case STACK_OVERFLOW:
			fprintf(stderr, "STACK_OVERFLOW");
			break;
		case STACK_UNDERFLOW:
			fprintf(stderr, "STACK_UNDERFLOW");
			break;
		case DIVISION_BY_ZERO:
			fprintf(stderr, "DIVISION_BY_ZERO");
			break;
		case NONE:
			fprintf(stderr, "no error!");
			break;
		default:
			fprintf(stderr, "ERROR(nodescr)!");
		break;
	}

	if (error.description)
		printf(" - %s\n", error.description);
	else
		printf("\n");
	
	exit(error.flag);
}