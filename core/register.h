#ifndef _REGISTER_H
#define _REGISTER_H

#include <stdint.h>


enum Registers {
  EAX,
  ECX,
  EDX,
  EBX,
  ESP,
  EBP,
  ESI,
  EDI,
  REGISTERS_NUM
};

typedef struct {
  uint32_t eip;
  uint32_t eflags;
  uint32_t r[REGISTERS_NUM];
} Register;


Register *register_create(void);
uint32_t register_get_eip(Register *);
void register_set_eip(Register *, uint32_t);
uint32_t register_get_eflags(Register *);
void register_set_eflags(Register *, uint32_t);
uint32_t register_get_r(Register *, uint8_t);
void register_set_r(Register *, uint8_t, uint32_t);


#endif

