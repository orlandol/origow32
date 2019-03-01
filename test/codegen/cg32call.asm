
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"
  %include "codegen.inc"

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  push    fileHandle
  call    fcreate
  mov     [fileHandle], eax
  test    eax, eax
  jz      .fcreateSucceeded
  push    fcreateFailed
  call    echostring
  push    eoln
  call    echostring
  push    dword 1
  call    [ExitProcess]
 .fcreateSucceeded:

  push    dword [fileHandle]
  push    dword x86Call
  push    dword 0
  push    dword x86RegESP
  push    dword x86RegEBP
  push    dword 2
  push    dword 0
  push    dword 4
  call    x86GenOpMem

 .Exit:
  lea     eax, [fileHandle]
  push    eax
  call    fclose

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

  declstring fileName, "call32.cg"
  declstring fcreateFailed, "Error creating file call32.cg"

  fileHandle: dd 0

section .bss use32
