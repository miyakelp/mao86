#include <stdlib.h>
#include <stdint.h>
#include "register.h"


Register *register_create(void) {
  Register *r = (Register *)malloc(sizeof(Register));
  return r;
}


uint32_t register_get_eip(Register *reg) {
  return reg->eip;
}


void register_set_eip(Register *reg, uint32_t value) {
  reg->eip = value;
}


uint32_t register_get_eflags(Register *reg) {
  return reg->eflags;
}


void register_set_eflags(Register *reg, uint32_t value) {
  reg->eflags = value;
}


uint32_t register_get_r(Register *reg, uint8_t register_num) {
  return reg->r[register_num];
}


void register_set_r(Register *reg, uint8_t register_num, uint32_t value) {
  reg->r[register_num] = value;
}

