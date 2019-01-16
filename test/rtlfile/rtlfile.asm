
  CPU 386

  BITS 32

segment .text use32

  %include "sys.inc"
  %include "rtl.inc"

..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  nop
  call    TestCreate

  nop
  call    TestModify

  nop
  call    TestOpen

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

;
; Test read/write functions on new file
;
TestCreate:
  push    ebp
  mov     ebp, esp

  %define .testFileHandle ebp - 4
  push    dword 0

  push    testFileName
  call    fcreate

  mov     [.testFileHandle], eax

 .Exit:
  lea     eax, [.testFileHandle]
  push    eax
  call    fclose

  mov     esp, ebp
  pop     ebp
  ret

;
; Test read/write functions on existing file
;
TestModify:
  push    ebp
  mov     ebp, esp

  mov     esp, ebp
  pop     ebp
  ret

;
; Test read/write functions on read only file
;
TestOpen:
  push    ebp
  mov     ebp, esp

  mov     esp, ebp
  pop     ebp
  ret

segment .data use32

                dd 8
  testFileName: db 'test.txt',0

section .bss use32
