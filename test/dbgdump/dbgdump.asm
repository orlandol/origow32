
  CPU 386

  BITS 32

  %include "sys.inc"
  %include "debug.inc"

segment .text use32
  
..start:
  push    ebp
  mov     ebp, esp

  mov     eax, 0x01020304
  mov     ebx, 0x05060708
  mov     ecx, 0x090A0B0C
  mov     edx, 0x0D0E0F11
  mov     esi, 0x12131415
  mov     edi, 0x16171819
  call    dump

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

section .bss use32
