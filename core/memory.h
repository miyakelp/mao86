#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_MEMORY_SIZE 0xffff

typedef struct Memory {
  uint32_t size;
  uint8_t *memory;
} Memory;


Memory *memory_create(uint32_t);
int memory_load_file(Memory *, FILE *, uint32_t);
uint8_t memory_get_code8(Memory *, uint32_t);
uint32_t memory_get_code32(Memory *, uint32_t);
void memory_set_code8(Memory *, uint8_t, uint32_t);
void memory_set_code32(Memory *, uint32_t, uint32_t);


#endif

