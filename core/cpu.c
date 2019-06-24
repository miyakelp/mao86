#include <stdlib.h>
#include "cpu.h"
#include "register.h"


CPU *CPU_create(void) {
  CPU *cpu = (CPU *)malloc(sizeof(CPU));
  if (cpu == NULL) {
    return NULL;
  }
  cpu->reg = register_create();
  if (cpu->reg == NULL) {
    free(cpu);
    return NULL;
  }
  return CPU;
}

