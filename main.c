#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"


int main(int argc, char *argv[]) {
  Memory *memory;
  CPU *cpu;
  FILE *binary;

  memory = memory_create(DEFAULT_MEMORY_SIZE);
  cpu = cpu_create();

  if (memory == NULL || cpu == NULL) {
    printf("Allocation failer\n");
  }

  binary = fopen(argv[1], "rb");
  if (binary == NULL) {
    printf("fopen error\n");
    return 1;
  }
  memory_load_file(memory, binary, 0);
  fclose(binary);


  
}

