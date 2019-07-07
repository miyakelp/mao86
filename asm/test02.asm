BITS 32
start:
  mov esp, 0x1000
  sub esp, 0x0010
  mov ebp, esp
  mov eax, 2
  mov dword [ebp + 4], 5
  add dword [ebp + 4], eax
  mov esi, [ebp + 4]
  inc dword [ebp + 4]
  mov edi, [ebp + 4]
  push eax
  pop ecx
  jmp 0

