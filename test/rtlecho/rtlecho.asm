
  CPU 386

  BITS 32

segment .text use32

  %include "sys.inc"
  %include "rtl.inc"
  
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

              dd 15
  helloWorld: db "Hello, world!", 13, 10, 0

section .bss use32
