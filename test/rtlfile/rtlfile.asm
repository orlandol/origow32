
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

  ; TODO: echo( "Tests passed", eoln )

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

  ; Create the test file
  push    testFileName
  call    fcreate

  test    eax, eax
  jnz     .CreateSucceeded
  ; TODO: Display error message
  push    dword 1
  call    [ExitProcess]
 .CreateSucceeded:

  mov     [.testFileHandle], eax

  ; Get file position
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
  ; TODO: Display error message
  push    dword 3
  call    [ExitProcess]
 .IsPos0:

  ; Skip to offset 13
  push    dword [.testFileHandle]
  push    dword 13
  call    fseek

  test    eax, eax
  jnz     .fseek13Succeeded
  ; TODO: Display error message
  push    dword 4
  call    [ExitProcess]
 .fseek13Succeeded:

  ; Change file size by setting EOF
  push    dword [.testFileHandle]
  call    fseteof

  test    eax, eax
  jnz     .fseteofSucceeded
  ; TODO: Display error message
  push    dword 5
  call    [ExitProcess]
 .fseteofSucceeded:

  ; Write the last uppercase half of the test string
  push    dword [.testFileHandle]
  push    upperTestText + 13
  push    dword 13
  call    fwrite

  cmp     eax, 13
  je      .Write13Succeeded
  ; TODO: Display error message
  push     dword 6
  call     [ExitProcess]
 .Write13Succeeded:

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
  lowerTestText: db 'abcdefghijklmnopqrstuvwxyz',0

                 dd 26
  upperTestText: db 'ABCDEFGHIJKLMNOPQRSTUVWXYZ',0

section .bss use32
