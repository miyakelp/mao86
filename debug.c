#include "debug.h"
#include "cpu.h"
#include "register.h"


void debug_print_registers(CPU *cpu) {
  printf("EIP 0x%x\tEFLAGS 0x%x\n", cpu_get_register_eip(cpu), cpu_get_register_eflags(cpu));
  for (int i = 0; i < REGISTERS_NUM; i++) {
    printf("r[%d] 0x%x\n", i, cpu_get_register_r(cpu, i));
  }
}

