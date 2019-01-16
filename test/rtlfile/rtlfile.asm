
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
  %define .tempPos ebp - 8
  push    dword 0

  ; Attempt to create file
  push    testFileName
  call    fcreate

  test    eax, eax
  jnz     .CreateSucceeded
  ; TODO: Display error message
  push    dword 1
  call    [ExitProcess]
 .CreateSucceeded:

  mov     [.testFileHandle], eax

  ; Attempt to get file position
  push    dword [.testFileHandle]
  lea     eax, [.tempPos]
  push    eax
  call    fpos

  test    eax, eax
  jnz     .GetPos0Succeeded
  ; TODO: Display error message
  push    dword 2
  call    [ExitProcess]
 .GetPos0Succeeded:

  cmp     dword [.tempPos], 0
  jz      .IsPos0
  push    dword [.tempPos]
  call    echouint
  ; TODO: Display error message
  push    dword 3
  call    [ExitProcess]
 .IsPos0:

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

            dd 26
  testText: db 'abcdefghijklmnopqrstuvwxyz',0

section .bss use32
