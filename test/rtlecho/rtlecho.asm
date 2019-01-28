
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  push    helloWorld
  call    echostring

  push    dword 0
  call    echouint
  push    eoln
  call    echostring

  push    dword -1
  call    echouint
  push    eoln
  call    echostring

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

  declstring helloWorld, 'Hello, world!',13,10

section .bss use32
