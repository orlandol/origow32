
  CPU 386

  BITS 32

  %include "rtl.inc"

segment .text use32

..start:
  push    ebp
  mov     ebp, esp

  ; Test STDOUT
  push    dword STD_OUTPUT_HANDLE
  call    [GetStdHandle]
  mov     [stdOut], eax

  %define .tempWritten ebp - 4
  push    dword 0

  push    dword 0
  lea     eax, [.tempWritten]
  push    eax
  push    dword [helloLen]
  push    hello
  push    dword [stdOut]
  call    [WriteFile]

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32
  hello:    db "Hello, world!",0
  helloLen: dd $ - hello - 1

section .bss use32
