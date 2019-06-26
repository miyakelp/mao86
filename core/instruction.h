#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "cpu.h"
#include "memory.h"

typedef void instruction_function_t(CPU *, Memory);

typedef struct {
  instruction_function_t *table[0xff];
} Instruction;


Instruction *instruction_create(void);
void instruction_execute(Instruction *, CPU *, Memory *);

#endif

