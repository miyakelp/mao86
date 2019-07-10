BITS 32
start:
  mov esp, 0x0100
  call start2 
  jmp short start
start2:
  mov eax, 0xff
  ret

