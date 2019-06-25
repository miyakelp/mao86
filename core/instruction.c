#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "instruction.h"


void init_instruction_table(Instruction *);

void mov_r32_imm32(CPU *, Memory *);
void short_jump(CPU *, Memory *);
void near_jump(CPU *, Memory *);


Instruction *instruction_create(void) {
  Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));
  if (instruction == NULL) {
    return NULL;
  }
  init_instruction_table(instruction);

  return instruction;
}


void init_instruction_table(Instruction *instruction) {
  for (int i = 0; i < REGISTERS_NUM; i++) {
    instruction->table[0xb8] = mov_r32_imm32;
  }
  instruction->table[0xe9] = near_jump;
  instruction->table[0xe9] = short_jump;
}


void mov_r32_imm32(CPU *cpu, Memory *memory) {
  uint8_t reg_num = memory_get_code8(memory, cpu->reg->eip) - 0xB8;
  uint32_t value = memory_get_code32(memory, cpu->reg->eip + 1);

  cpu->reg->r[reg_num] = value;
  cpu->reg->eip += 5;
}


void short_jump(CPU *cpu, Memory *memory) {
  int8_t diff = (int8_t)memory_get_code8(memory, cpu->reg->eip + 1);
  cpu->reg->eip += (diff + 2);
}


void near_jump(CPU *cpu, Memory *memory) {
  int32_t diff = (int32_t)memory_get_code32(memory, cpu->reg->eip + 1);
  cpu->reg->eip += (diff + 5);
}

