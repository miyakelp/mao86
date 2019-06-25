#include <stdlib.h>
#include <stdint.h>
#include "Instruction.h"


Instruction *instruction_create(void) {
  Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
  if (instruction == NULL) {
    return NULL;
  }

  return instruction;
}
