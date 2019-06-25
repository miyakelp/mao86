#ifndef _CPU_H
#define _CPU_H

#include "register.h"


typedef struct {
  Register *reg;
  Instruction *instruction;
} CPU;


CPU *CPU_create(void);
uint32_t cpu_get_register_eip(CPU *);
void cpu_set_register_eip(CPU *, uint32_t);
void cpu_add_to_register_eip(CPU *, int32_t);
uint32_t cpu_get_register_eflags(CPU *);
void cpu_set_register_eflags(CPU *, uint32_t);
uint32_t cpu_get_register_r(CPU *, uint8_t);
void cpu_set_register_r(CPU *, uint32_t);

#endif

