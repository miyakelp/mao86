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

