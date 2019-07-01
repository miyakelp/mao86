#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "instruction.h"

instruction_function_t *instruction_table[0xff];

typedef struct {
  uint8_t mod;
  union {
    uint8_t opecode;
    uint8_t register_index;
  };
  uint8_t rm;
  uint8_t sib;
  union {
    int8_t disp8;
    uint32_t disp32; 
  };
} ModRM;

void parse_modrm(CPU *, Memory *, ModRM *);
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


void parse_modrm(CPU *cpu, Memory *memory, ModRM *modrm) {
  uint8_t code;
  memset(modrm, 0, sizeof(ModRM));

  code = memory_get_code8(memory, cpu_get_register_eip(cpu));
  modrm->mod = ((code >> 6) & 0x03);
  modrm->opecode = ((code >> 3) & 0x07);
  modrm->rm = (code & 0x07);

  cpu_add_to_register_eip(cpu, 1);
  if (modrm->mod != 0x03 && modrm->rm == 0x04) {
    modrm->sib = memory_get_code8(memory, cpu_get_register_eip(cpu));
    cpu_add_to_register_eip(cpu, 1);
  }

  if ((modrm->mod == 0x00 && modrm->rm == 0x05) || modrm->mod == 0x02) {
    modrm->disp32 = memory_get_code32(memory, cpu_get_register_eip(cpu));
    cpu_add_to_register_eip(cpu, 4);
  } else if (modrm->mod == 0x01) {
    modrm->disp8 = (int8_t)memory_get_code8(memory, cpu_get_register_eip(cpu));
    cpu_add_to_register_eip(cpu, 1);
  }

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

