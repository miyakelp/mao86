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
uint32_t calc_memory_address(CPU *, ModRM *);
uint32_t get_rm32(CPU *, Memory *, ModRM *);
void set_rm32(CPU *, Memory *, ModRM *, uint32_t);
void push32(CPU *, Memory *, uint32_t);
uint32_t pop32(CPU *, Memory *);

static void add_rm32_r32(CPU *, Memory *);
static void push_r32(CPU *, Memory *);
static void pop_r32(CPU *, Memory *);
static void push_imm32(CPU *, Memory *);
static void push_imm8(CPU *, Memory *);
static void code_83(CPU *, Memory *);
static void mov_rm32_r32(CPU *, Memory *);
static void mov_r32_rm32(CPU *, Memory *);
static void mov_r32_imm32(CPU *, Memory *);
static void near_ret(CPU *, Memory *);
static void mov_rm32_imm32(CPU *, Memory *);
static void leave(CPU *, Memory *);
static void call_rel32(CPU *, Memory *);
static void short_jump(CPU *, Memory *);
static void near_jump(CPU *, Memory *);
static void code_ff(CPU *, Memory *);

/* CODE 0x83 */
static void sub_rm32_imm8(CPU *, Memory*, ModRM *);

/* CODE 0xff */
static void inc_rm32(CPU *, Memory *, ModRM *);
static void dec_rm32(CPU *, Memory *, ModRM *);


void instruction_init_table(void) {
  instruction_table[0x01] = add_rm32_r32;
  for (int i = 0; i < REGISTERS_NUM; i++) {
    instruction_table[0x50 + i] = push_r32;
  }
  for (int i = 0; i < REGISTERS_NUM; i++) {
    instruction_table[0x58 + i] = pop_r32;
  }
  instruction_table[0x68] = push_imm32;
  instruction_table[0x6a] = push_imm8;
  instruction_table[0x83] = code_83;
  instruction_table[0x89] = mov_rm32_r32;
  instruction_table[0x8b] = mov_r32_rm32;
  for (int i = 0; i < REGISTERS_NUM; i++) {
    instruction_table[0xb8 + i] = mov_r32_imm32;
  }
  instruction_table[0xc3] = near_ret;
  instruction_table[0xc7] = mov_rm32_imm32;
  instruction_table[0xc9] = leave;
  instruction_table[0xe8] = call_rel32;
  instruction_table[0xe9] = near_jump;
  instruction_table[0xeb] = short_jump;
  instruction_table[0xff] = code_ff;
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


uint32_t calc_memory_address(CPU *cpu, ModRM *modrm) {
  if (modrm->mod == 0x00) {
    if (modrm->rm == 0x04) {
      /* TODO */
      exit(1);
    } else if (modrm->rm == 0x05) {
      return modrm->disp32;
    }
  } else if (modrm->mod == 0x01) {
    if (modrm->rm == 0x04) {
      /* TODO */
      exit(1);
    } else {
      return cpu_get_register_r(cpu, modrm->rm) + modrm->disp8;
    }
  } else if (modrm->mod == 0x02) {
    if (modrm->rm == 0x04) {
      /* TODO */
      exit(1);
    } else {
      return cpu_get_register_r(cpu, modrm->rm) + modrm->disp32;
    }
  } else {
    /* TODO */
    exit(1);
  }
}


uint32_t get_rm32(CPU *cpu, Memory *memory, ModRM *modrm) {
  if (modrm->mod == 0x03) {
    return cpu_get_register_r(cpu, modrm->rm);
  } else {
    uint32_t address = calc_memory_address(cpu, modrm);
    return memory_get_code32(memory, address);
  }
}


void set_rm32(CPU *cpu, Memory *memory, ModRM *modrm, uint32_t value) {
  if (modrm->mod == 0x03) {
    cpu_set_register_r(cpu, modrm->rm, value);
  } else {
    uint32_t address = calc_memory_address(cpu, modrm);
    memory_set_code32(memory, address, value);
  }
}


void push32(CPU *cpu, Memory *memory, uint32_t value) {
  uint32_t address = cpu_get_register_r(cpu, ESP) - 4;
  cpu_set_register_r(cpu, ESP, address);
  memory_set_code32(memory, address, value);
}


uint32_t pop32(CPU *cpu, Memory *memory) {
  uint32_t address = cpu_get_register_r(cpu, ESP);
  cpu_set_register_r(cpu, ESP, address + 4);
  return memory_get_code32(memory, address);
}


static void add_rm32_r32(CPU *cpu, Memory *memory) {
  cpu_add_to_register_eip(cpu, 1);

  ModRM modrm;
  parse_modrm(cpu, memory, &modrm);

  uint32_t r32 = cpu_get_register_r(cpu, modrm.register_index);
  uint32_t rm32 = get_rm32(cpu, memory, &modrm);
  set_rm32(cpu, memory, &modrm, rm32 + r32);
}


static void push_r32(CPU *cpu, Memory *memory) {
  uint8_t register_index = memory_get_code8(memory, cpu_get_register_eip(cpu)) - 0x50;
  push32(cpu, memory, cpu_get_register_r(cpu, register_index));
  cpu_add_to_register_eip(cpu, 1);
}


static void pop_r32(CPU *cpu, Memory *memory) {
  uint8_t register_index = memory_get_code8(memory, cpu_get_register_eip(cpu)) - 0x58;
  cpu_set_register_r(cpu, register_index, pop32(cpu, memory));
  cpu_add_to_register_eip(cpu, 1);
}


static void push_imm32(CPU *cpu, Memory *memory) {
  uint8_t value = memory_get_code32(memory, cpu_get_register_eip(cpu) + 1);
  push32(cpu, memory, value);
  cpu_add_to_register_eip(cpu, 5);
}


static void push_imm8(CPU *cpu, Memory *memory) {
  uint8_t value = memory_get_code8(memory, cpu_get_register_eip(cpu) + 1);
  push32(cpu, memory, value);
  cpu_add_to_register_eip(cpu, 2);
}


static void code_83(CPU *cpu, Memory *memory) {
  cpu_add_to_register_eip(cpu, 1);

  ModRM modrm;
  parse_modrm(cpu, memory, &modrm);

  switch (modrm.opecode) {
  case 0x05:
    sub_rm32_imm8(cpu, memory, &modrm);
    break;
  default:
    /* TODO */
    exit(1);
    break;
  }
}


static void mov_rm32_r32(CPU *cpu, Memory *memory) {
  cpu_add_to_register_eip(cpu, 1);

  ModRM modrm;
  parse_modrm(cpu, memory, &modrm);

  uint32_t r32 = cpu_get_register_r(cpu, modrm.register_index);
  set_rm32(cpu, memory, &modrm, r32);
}


static void mov_r32_rm32(CPU *cpu, Memory *memory) {
  cpu_add_to_register_eip(cpu, 1);

  ModRM modrm;
  parse_modrm(cpu, memory, &modrm);

  uint32_t rm32 = get_rm32(cpu, memory, &modrm);
  cpu_set_register_r(cpu, modrm.register_index, rm32);
}


static void mov_rm32_imm32(CPU *cpu, Memory *memory) {
  cpu_add_to_register_eip(cpu, 1);

  ModRM modrm;
  parse_modrm(cpu, memory, &modrm);

  uint32_t imm = memory_get_code32(memory, cpu_get_register_eip(cpu));
  cpu_add_to_register_eip(cpu, 4);
  set_rm32(cpu, memory, &modrm, imm);
}


static void near_ret(CPU *cpu, Memory *memory) {
  cpu_set_register_eip(cpu, pop32(cpu, memory));
}


static void mov_r32_imm32(CPU *cpu, Memory *memory) {
  uint8_t register_num = memory_get_code8(memory, cpu->reg->eip) - 0xB8;
  uint32_t value = memory_get_code32(memory, cpu->reg->eip + 1);

  cpu_set_register_r(cpu, register_num, value);
  cpu_add_to_register_eip(cpu, 5);
}


static void leave(CPU *cpu, Memory *memory) {
  uint32_t ebp = cpu_get_register_r(cpu, EBP);
  cpu_set_register_r(cpu, ESP, ebp);
  cpu_set_register_r(cpu, EBP, pop32(cpu, memory));
  cpu_add_to_register_eip(cpu, 1);
}


static void call_rel32(CPU *cpu, Memory *memory) {
  int32_t diff = (int32_t)memory_get_code32(memory, cpu_get_register_eip(cpu));
  push32(cpu, memory, cpu_get_register_eip(cpu));
  cpu_add_to_register_eip(cpu, diff + 5);
}


static void short_jump(CPU *cpu, Memory *memory) {
  int8_t diff = (int8_t)memory_get_code8(memory, cpu->reg->eip + 1);
  cpu_add_to_register_eip(cpu, diff + 2);
}


static void near_jump(CPU *cpu, Memory *memory) {
  int32_t diff = (int32_t)memory_get_code32(memory, cpu->reg->eip + 1);
  cpu_add_to_register_eip(cpu, diff + 5);
}


static void code_ff(CPU *cpu, Memory *memory) {
  cpu_add_to_register_eip(cpu, 1);

  ModRM modrm;
  parse_modrm(cpu, memory, &modrm);

  switch (modrm.opecode) {
  case 0x00:
    inc_rm32(cpu, memory, &modrm);
    break;
  case 0x01:
    dec_rm32(cpu, memory, &modrm);
    break;
  default:
    /* TODO */
    exit(1);
    break;
  }
}


static void sub_rm32_imm8(CPU *cpu, Memory *memory, ModRM *modrm) {
  uint32_t rm32 = get_rm32(cpu, memory, modrm);
  uint32_t imm8 = (int8_t)memory_get_code8(memory, cpu_get_register_eip(cpu));
  cpu_add_to_register_eip(cpu, 1);
  set_rm32(cpu, memory, modrm, rm32 - imm8);
}


static void inc_rm32(CPU *cpu, Memory *memory, ModRM *modrm) {
  uint32_t value = get_rm32(cpu, memory, modrm);
  set_rm32(cpu, memory, modrm, value + 1);
}


static void dec_rm32(CPU *cpu, Memory *memory, ModRM *modrm) {
  uint32_t value = get_rm32(cpu, memory, modrm);
  set_rm32(cpu, memory, modrm, value + 1);
}

