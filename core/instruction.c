#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "instruction.h"

instruction_function_t *instruction_table[0xff];

void mov_r32_imm32(CPU *, Memory *);
void short_jump(CPU *, Memory *);
void near_jump(CPU *, Memory *);


void instruction_init_table(void) {
  for (int i = 0; i < REGISTERS_NUM; i++) {
    instruction_table[0xb8 + i] = mov_r32_imm32;
  }
  instruction_table[0xe9] = near_jump;
  instruction_table[0xeb] = short_jump;
}


void instruction_execute(CPU *cpu, Memory *memory) {
  uint8_t opcode = memory_get_code8(memory, cpu_get_register_eip(cpu));
  instruction_table[opcode](cpu, memory);
}


void mov_r32_imm32(CPU *cpu, Memory *memory) {
  uint8_t register_num = memory_get_code8(memory, cpu->reg->eip) - 0xB8;
  uint32_t value = memory_get_code32(memory, cpu->reg->eip + 1);

  cpu_set_register_r(cpu, register_num, value);
  cpu_add_to_register_eip(cpu, 5);
}


void short_jump(CPU *cpu, Memory *memory) {
  int8_t diff = (int8_t)memory_get_code8(memory, cpu->reg->eip + 1);
  cpu_add_to_register_eip(cpu, diff + 2);
}


void near_jump(CPU *cpu, Memory *memory) {
  int32_t diff = (int32_t)memory_get_code32(memory, cpu->reg->eip + 1);
  cpu_add_to_register_eip(cpu, diff + 5);
}

