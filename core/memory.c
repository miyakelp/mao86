#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"


Memory *memory_create(uint32_t size) {
  Memory *memory = (Memory *)malloc(sizeof(Memory));
  if (memory == NULL) {
    return NULL;
  }
  memory->memory = (uint8_t *)malloc(sizeof(uint8_t) * size);
  if (memory->memory == NULL) {
    free(memory);
    return NULL;
  }
  memory->size = size;
  return memory;
}


int memory_load_file(Memory *memory, FILE *file, uint32_t address) {
  int count = fread(memory->memory + address, 1, memory->size - address, file);
  fclose(file);

  if (count == memory->size - address) { 
    return 0;
  } else {
    return 1;
  }
}


uint8_t memory_get_code8(Memory * memory, uint32_t address) {
  return memory->memory[address];
}


uint32_t memory_get_code32(Memory * memory, uint32_t address) {
  uint32_t r = 0;
  for (int i = 0; i < 4; i++) {
    r |= ((uint32_t)memory->memory[address + i] << (8 * i));
  }
  return r;
}


void memory_set_code8(Memory * memory, uint8_t address, uint32_t value) {
  memory->memory[address] = value;
}


void memory_set_code32(Memory * memory, uint32_t address, uint32_t value) {
  for (int i = 0; i < 4; i++) {
    memory->memory[address + i] = (value >> (8 * i));
  }
}

