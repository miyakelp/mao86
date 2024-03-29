#include <stdlib.h>
#include "cpu.h"
#include "register.h"
#include "instruction.h"


CPU *cpu_create(void) {
  CPU *cpu = (CPU *)malloc(sizeof(CPU));
  if (cpu == NULL) {
    return NULL;
  }
  cpu->reg = register_create();
  if (cpu->reg == NULL) {
    free(cpu);
    return NULL;
  }
  cpu_set_register_eip(cpu, 0);
  cpu_set_register_eflags(cpu, 0);

  instruction_init_table();

  return cpu;
}


void cpu_run(CPU *cpu, Memory *memory) {
  do {
    instruction_execute(cpu, memory);    
  } while (cpu_get_register_eip(cpu) != 0x00);
}


uint32_t cpu_get_register_eip(CPU *cpu) {
  return register_get_eip(cpu->reg);
}


void cpu_set_register_eip(CPU *cpu, uint32_t value) {
  register_set_eip(cpu->reg, value);
}


void cpu_add_to_register_eip(CPU *cpu, int32_t value) {
  uint32_t r = register_get_eip(cpu->reg);

  register_set_eip(cpu->reg, (uint32_t)((int32_t)r + value));
}


uint32_t cpu_get_register_eflags(CPU *cpu) {
  return register_get_eflags(cpu->reg);
}


void cpu_set_register_eflags(CPU *cpu, uint32_t value) {
  register_set_eflags(cpu->reg, value);
}


uint32_t cpu_get_register_r(CPU *cpu, uint8_t register_num) {
  return register_get_r(cpu->reg, register_num);
}


void cpu_set_register_r(CPU *cpu, uint8_t register_num, uint32_t value) {
  register_set_r(cpu->reg, register_num, value);
}

