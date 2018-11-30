
  CPU 386

  BITS 32

  %include "sys.inc"

segment .text use32
  
..start:
  push dword 0
  call [ExitProcess]

  %include "rtl.inc"

segment .data use32

section .bss use32

