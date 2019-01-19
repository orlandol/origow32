
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

  push    testsPassed
  call    echostring

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
  %define .tempSize ebp - 12
  push    dword 0
  %define .tempCh ebp - 16
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
  jnz     .fseekMidSucceeded
  ; TODO: Display error message
  push    dword 4
  call    [ExitProcess]
 .fseekMidSucceeded:

  ; Change file size by setting EOF
  push    dword [.testFileHandle]
  call    fseteof

  test    eax, eax
  jnz     .fseteofSucceeded
  ; TODO: Display error message
  push    dword 5
  call    [ExitProcess]
 .fseteofSucceeded:

  ; Write an uppercase character
  push    dword [.testFileHandle]
  push    upperTestText + 13
  call    fwritech

  test    eax, eax
  jnz     .WriteChSucceeded
  ; TODO: Display error message
  push     dword 6
  call     [ExitProcess]
 .WriteChSucceeded:

  ; Write the last uppercase half of the test string
  push    dword [.testFileHandle]
  push    upperTestText + 14
  push    dword 12
  call    fwrite

  cmp     eax, 12
  je      .WriteSucceeded
  ; TODO: Display error message
  push     dword 7
  call     [ExitProcess]
 .WriteSucceeded:

  ; Verify file size
  push    dword [.testFileHandle]
  lea     eax, [.tempSize]
  push    eax
  call    fsize

  test    eax, eax
  jnz     .FsizeSucceeded
  ; TODO: Display error message
  push    dword 8
  call    [ExitProcess]
 .FsizeSucceeded:

  cmp     dword [.tempSize], 26
  je      .FileSizeIsCorrect
  ; TODO: Display error message
  push    dword 9
  call    [ExitProcess]
 .FileSizeIsCorrect:

  ; Go back to offset 13
  push    dword [.testFileHandle]
  push    dword 13
  call    fseek

  test    eax, eax
  jnz     .FseekMidChSucceeded
  ; TODO: Display error message
  push    dword 10
  call    [ExitProcess]
 .FseekMidChSucceeded:

  ; Read the first uppercase character
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadMidChSucceeded
  ; TODO - Display error message
  push    dword 11
  call    [ExitProcess]
 .ReadMidChSucceeded:

  cmp     byte [.tempCh], 'N'
  je      .MidCharIsCorrect
  ; TODO - Display error message
  push    dword 12
  call    [ExitProcess]
 .MidCharIsCorrect:

  ; Go back to offset 25
  push    dword [.testFileHandle]
  push    dword 25
  call    fseek

  test    eax, eax
  jnz     .FseekLastChSucceeded
  ; TODO: Display error message
  push    dword 14
  call    [ExitProcess]
 .FseekLastChSucceeded:

  ; Read the first uppercase character
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadLastChSucceeded
  ; TODO - Display error message
  push    dword 15
  call    [ExitProcess]
 .ReadLastChSucceeded:

  cmp     byte [.tempCh], 'Z'
  je      .LastCharIsCorrect
  ; TODO - Display error message
  push    dword 16
  call    [ExitProcess]
 .LastCharIsCorrect:

 .Exit:
  ; Close the file
  lea     eax, [.testFileHandle]
  push    eax
  call    fclose

  cmp     dword [.testFileHandle], 0
  je      .FileClosed
  ; TODO: Display error message
  push    dword 17
  call    [ExitProcess]
 .FileClosed:

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

               dd 16
  testsPassed: db 13,10,'Tests passed',13,10,0

section .bss use32
