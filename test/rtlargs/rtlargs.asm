
  CPU 386

  BITS 32

  %include "sys.inc"
  %include "rtl.inc"
  %include "chtable.inc"

segment .text use32

..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  call    CommandLineLength

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

section .bss use32
