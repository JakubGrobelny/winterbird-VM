#include "program.h"

void free_program(program_t* program)
{
	free(program->data);
	free(program->text);
	program->data_size = 0;
	program->text_size = 0;
}