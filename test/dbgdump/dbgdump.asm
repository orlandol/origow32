
  CPU 386

  BITS 32

  %include "debug.inc"

segment .text use32

run
..start:
  push    ebp
  mov     ebp, esp

  mov     eax, 0x01020304
  mov     ebx, 0x05060708
  mov     ecx, 0x090A0B0C
  mov     edx, 0x0D0E0F10
  mov     esi, 0x11121314
  mov     edi, 0x15161718
  mov     ebp, 0x19202122
  call    dump

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

section .bss use32
