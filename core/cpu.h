#ifndef _CPU_H
#define _CPU_H

#include "register.h"


typedef struct {
  Register *reg;
  Instruction *instruction;
} CPU;


CPU *CPU_create(void);

#endif

