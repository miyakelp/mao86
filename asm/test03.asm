BITS 32
start:
  call start2 
  jmp short start
start2:
  mov eax, 0xff
  ret

