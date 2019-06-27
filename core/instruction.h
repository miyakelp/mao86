#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "cpu.h"
#include "memory.h"

typedef void instruction_function_t(CPU *, Memory *);

void instruction_init_table(void);
void instruction_execute(CPU *, Memory *);

#endif

