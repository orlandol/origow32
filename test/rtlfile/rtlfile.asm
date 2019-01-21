
  CPU 386

  BITS 32

segment .text use32

  %include "sys.inc"
  %include "rtl.inc"

..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  push    testingCreate
  call    echostring
  nop
  call    TestCreate

  push    testingUpdate
  call    echostring
  nop
  call    TestUpdate

  push    testingOpen
  call    echostring
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

  push    dword 0
  %define .testFileHandle ebp - 4

  push    dword 0
  %define .tempPos ebp - 8

  push    dword 0
  %define .tempSize ebp - 12

  push    dword 0
  %define .tempCh ebp - 16

  ; Create test file with read/write access
  push    testFileName
  call    fcreate

  test    eax, eax
  jnz     .CreateSucceeded
  push    fcreateFailed
  call    echostring
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
  push    fposFailed
  call    echostring
  push    dword 2
  call    [ExitProcess]
 .GetPos0Succeeded:

  cmp     dword [.tempPos], 0
  jz      .IsPos0
  push    fposFailed
  call    echostring
  push    dword 3
  call    [ExitProcess]
 .IsPos0:

  ; Skip to offset 13
  push    dword [.testFileHandle]
  push    dword 13
  call    fseek

  test    eax, eax
  jnz     .fseekMidSucceeded
  push    fseekFailed
  call    echostring
  push    dword 4
  call    [ExitProcess]
 .fseekMidSucceeded:

  ; Change file size by setting EOF
  push    dword [.testFileHandle]
  call    fseteof

  test    eax, eax
  jnz     .fseteofSucceeded
  push    fseteofFailed
  call    echostring
  push    dword 5
  call    [ExitProcess]
 .fseteofSucceeded:

  ; Attempt to read a character past EOF
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadChPastEOFSucceeded
  push    freadchFailed
  call    echostring
  push    dword 6
  call    [ExitProcess]
 .ReadChPastEOFSucceeded:

  ; Write an uppercase character
  push    dword [.testFileHandle]
  push    upperTestText + 13
  call    fwritech

  test    eax, eax
  jnz     .WriteChSucceeded
  push    fwritechFailed
  call    echostring
  push    dword 7
  call    [ExitProcess]
 .WriteChSucceeded:

  ; Write the last uppercase half of the test string
  push    dword [.testFileHandle]
  push    upperTestText + 14
  push    dword 12
  call    fwrite

  cmp     eax, 12
  je      .WriteSucceeded
  push    fwriteFailed
  call    echostring
  push    dword 8
  call    [ExitProcess]
 .WriteSucceeded:

  ; Verify file size
  push    dword [.testFileHandle]
  lea     eax, [.tempSize]
  push    eax
  call    fsize

  test    eax, eax
  jnz     .FsizeSucceeded
  push    fsizeFailed
  call    echostring
  push    dword 9
  call    [ExitProcess]
 .FsizeSucceeded:

  cmp     dword [.tempSize], 26
  je      .FileSizeIsCorrect
  push    fsizeFailed
  call    echostring
  push    dword 10
  call    [ExitProcess]
 .FileSizeIsCorrect:

  ; Go back to offset 13
  push    dword [.testFileHandle]
  push    dword 13
  call    fseek

  test    eax, eax
  jnz     .FseekMidChSucceeded
  push    fseekFailed
  call    echostring
  push    dword 11
  call    [ExitProcess]
 .FseekMidChSucceeded:

  ; Read the first uppercase character
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadMidChSucceeded
  push    freadchFailed
  call    echostring
  push    dword 12
  call    [ExitProcess]
 .ReadMidChSucceeded:

  cmp     byte [.tempCh], 'N'
  je      .MidCharIsCorrect
  push    freadchFailed
  call    echostring
  push    dword 13
  call    [ExitProcess]
 .MidCharIsCorrect:

  ; Go back to offset 25
  push    dword [.testFileHandle]
  push    dword 25
  call    fseek

  test    eax, eax
  jnz     .FseekLastChSucceeded
  push    fseekFailed
  call    echostring
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
  push    freadchFailed
  call    echostring
  push    dword 15
  call    [ExitProcess]
 .ReadLastChSucceeded:

  cmp     byte [.tempCh], 'Z'
  je      .LastCharIsCorrect
  push    freadchFailed
  call    echostring
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
  push    fcloseFailed
  call    echostring
  push    dword 17
  call    [ExitProcess]
 .FileClosed:

  mov     esp, ebp
  pop     ebp
  ret

;
; Test read/write functions on existing file
;
TestUpdate:
  push    ebp
  mov     ebp, esp

  push    dword 0
  %define .testFileHandle ebp - 4

  push    dword 0
  %define .tempPos ebp - 8

  push    dword 0
  %define .tempSize ebp - 12

  push    dword 0
  %define .tempCh ebp - 16

  ; Open the test file with read/write access
  push    testFileName
  call    fupdate

  test    eax, eax
  jnz     .UpdateSucceeded
  push    fupdateFailed
  call    echostring
  push    dword 18
  call    [ExitProcess]
 .UpdateSucceeded:

  mov     [.testFileHandle], eax

  ; Get file position
  push    dword [.testFileHandle]
  lea     eax, [.tempPos]
  push    eax
  call    fpos

  test    eax, eax
  jnz     .GetPos0Succeeded
  push    fposFailed
  call    echostring
  push    dword 19
  call    [ExitProcess]
 .GetPos0Succeeded:

  cmp     dword [.tempPos], 0
  jz      .IsPos0
  push    fposFailed
  call    echostring
  push    dword 20
  call    [ExitProcess]
 .IsPos0:

  ; Skip to offset 26
  push    dword [.testFileHandle]
  push    dword 26
  call    fseek

  test    eax, eax
  jnz     .fseekMidSucceeded
  push    fseekFailed
  call    echostring
  push    dword 21
  call    [ExitProcess]
 .fseekMidSucceeded:

  ; Change file size by setting EOF
  push    dword [.testFileHandle]
  call    fseteof

  test    eax, eax
  jnz     .fseteofSucceeded
  push    fseteofFailed
  call    echostring
  push    dword 22
  call    [ExitProcess]
 .fseteofSucceeded:

  ; Attempt to read a character past EOF
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadChPastEOFSucceeded
  push    freadchFailed
  call    echostring
  push    dword 23
  call    [ExitProcess]
 .ReadChPastEOFSucceeded:

  ; Return to offset 0
  push    dword [.testFileHandle]
  push    dword 0
  call    fseek

  test    eax, eax
  jnz     .fseek0Succeeded
  push    fseekFailed
  call    echostring
  push    dword 24
  call    [ExitProcess]
 .fseek0Succeeded:

  ; Write an uppercase character
  push    dword [.testFileHandle]
  push    lowerTestText
  call    fwritech

  test    eax, eax
  jnz     .WriteChSucceeded
  push    fwritechFailed
  call    echostring
  push    dword 25
  call    [ExitProcess]
 .WriteChSucceeded:

  ; Write the last uppercase half of the test string
  push    dword [.testFileHandle]
  push    lowerTestText + 1
  push    dword 12
  call    fwrite

  cmp     eax, 12
  je      .WriteSucceeded
  push    fwriteFailed
  call    echostring
  push    dword 26
  call    [ExitProcess]
 .WriteSucceeded:

  ; Verify file size
  push    dword [.testFileHandle]
  lea     eax, [.tempSize]
  push    eax
  call    fsize

  test    eax, eax
  jnz     .FsizeSucceeded
  push    fsizeFailed
  call    echostring
  push    dword 27
  call    [ExitProcess]
 .FsizeSucceeded:

  cmp     dword [.tempSize], 26
  je      .FileSizeIsCorrect
  push    fsizeFailed
  call    echostring
  push    dword 28
  call    [ExitProcess]
 .FileSizeIsCorrect:

  ; Return to offset 0
  push    dword [.testFileHandle]
  push    dword 0
  call    fseek

  test    eax, eax
  jnz     .FseekFirstChSucceeded
  push    fseekFailed
  call    echostring
  push    dword 29
  call    [ExitProcess]
 .FseekFirstChSucceeded:

  ; Read the last lowercase character
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadFirstChSucceeded
  push    freadchFailed
  call    echostring
  push    dword 30
  call    [ExitProcess]
 .ReadFirstChSucceeded:

  cmp     byte [.tempCh], 'a'
  je      .FirstCharIsCorrect
  push    freadchFailed
  call    echostring
  push    dword 31
  call    [ExitProcess]
 .FirstCharIsCorrect:

  ; Return to offset 12
  push    dword [.testFileHandle]
  push    dword 12
  call    fseek

  test    eax, eax
  jnz     .FseekLastChSucceeded
  push    fseekFailed
  call    echostring
  push    dword 32
  call    [ExitProcess]
 .FseekLastChSucceeded:

  ; Read the mid lower character
  push    dword [.testFileHandle]
  lea     eax, [.tempCh]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadMidChSucceeded
  push    freadchFailed
  call    echostring
  push    dword 33
  call    [ExitProcess]
 .ReadMidChSucceeded:

  cmp     byte [.tempCh], 'm'
  je      .LastCharIsCorrect
  push    freadchFailed
  call    echostring
  push    dword 34
  call    [ExitProcess]
 .LastCharIsCorrect:

 .Exit:
  ; Close the file
  lea     eax, [.testFileHandle]
  push    eax
  call    fclose

  cmp     dword [.testFileHandle], 0
  je      .FileClosed
  push    fcloseFailed
  call    echostring
  push    dword 35
  call    [ExitProcess]
 .FileClosed:

  mov     esp, ebp
  pop     ebp
  ret

;
; Test read/write functions on read only file
;
TestOpen:
  push    ebp
  mov     ebp, esp

  push    dword 0
  %define .testFileHandle ebp - 4

  push    dword 0
  %define .tempPos ebp - 8

  push    dword 0
  %define .tempSize ebp - 12

  push    dword 0
  %define .tempCh ebp - 16

  push    dword 0
  push    dword 0
  push    dword 0
  push    dword 0
  %define .tempStr ebp - 32

  ; Open the test file with read/write access
  push    testFileName
  call    fopen

  test    eax, eax
  jnz     .UpdateSucceeded
  push    fupdateFailed
  call    echostring
  push    dword 36
  call    [ExitProcess]
 .UpdateSucceeded:

  mov     [.testFileHandle], eax

  ; Write an uppercase character
  push    dword [.testFileHandle]
  push    upperTestText
  call    fwritech

  test    eax, eax
  jz      .WriteChFailed
  push    fwritechFailed
  call    echostring
  push    dword 37
  call    [ExitProcess]
 .WriteChFailed:

  ; Write the last uppercase half of the test string
  push    dword [.testFileHandle]
  push    upperTestText + 1
  push    dword 12
  call    fwrite

  test    eax, eax
  jz      .WriteFailed
  push    fwriteFailed
  call    echostring
  push    dword 38
  call    [ExitProcess]
 .WriteFailed:

  ; Change file size by setting EOF
  push    dword [.testFileHandle]
  call    fseteof

  test    eax, eax
  jz      .fseteofFailed
  push    fseteofFailed
  call    echostring
  push    dword 39
  call    [ExitProcess]
 .fseteofFailed:

  ; Verify file size
  push    dword [.testFileHandle]
  lea     eax, [.tempSize]
  push    eax
  call    fsize

  test    eax, eax
  jnz     .FsizeSucceeded
  push    fsizeFailed
  call    echostring
  push    dword 40
  call    [ExitProcess]
 .FsizeSucceeded:

  cmp     dword [.tempSize], 26
  je      .FileSizeIsCorrect
  push    fsizeFailed
  call    echostring
  push    dword 41
  call    [ExitProcess]
 .FileSizeIsCorrect:

  ; Get file position
  push    dword [.testFileHandle]
  lea     eax, [.tempPos]
  push    eax
  call    fpos

  test    eax, eax
  jnz     .GetPos0Succeeded
  push    fposFailed
  call    echostring
  push    dword 42
  call    [ExitProcess]
 .GetPos0Succeeded:

  cmp     dword [.tempPos], 0
  jz      .IsPos0
  push    fposFailed
  call    echostring
  push    dword 43
  call    [ExitProcess]
 .IsPos0:

  ; Read the first character
  push    dword [.testFileHandle]
  lea     eax, [.tempStr]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadFirstChSucceeded
  push    freadchFailed
  call    echostring
  push    dword 44
  call    [ExitProcess]
 .ReadFirstChSucceeded:

  ; Read the next 12 characters
  push    dword [.testFileHandle]
  lea     eax, [.tempStr + 1]
  push    eax
  push    dword 12
  call    fread

  cmp     eax, 12
  je      .FreadStartSucceeded
  push    freadFailed
  call    echostring
  push    dword 45
  call    [ExitProcess]
 .FreadStartSucceeded:

  ; Verify that first 13 characters are lowercase
  mov     esi, lowerTestText
  lea     edi, [.tempStr]
  mov     ecx, 13
  xor     eax, eax
 .CompareLower:
  mov     al, [esi]
  inc     esi
  sub     al, [edi]
  inc     edi
  dec     ecx
  jnz     .CompareLower

  test    eax, eax
  jz      .StringIsLowercase
  push    freadFailed
  call    echostring
  push    dword 46
  call    [ExitProcess]
 .StringIsLowercase:

  ; Read the middle character
  push    dword [.testFileHandle]
  lea     eax, [.tempStr]
  push    eax
  call    freadch

  test    eax, eax
  jnz     .ReadMidChSucceeded
  push    freadchFailed
  call    echostring
  push    dword 47
  call    [ExitProcess]
 .ReadMidChSucceeded:

  ; Read the next 12 characters
  push    dword [.testFileHandle]
  lea     eax, [.tempStr + 1]
  push    eax
  push    dword 12
  call    fread

  cmp     eax, 12
  je      .FreadMidSucceeded
  push    freadFailed
  call    echostring
  push    dword 48
  call    [ExitProcess]
 .FreadMidSucceeded:

  ; Verify that last 13 characters are uppercase
  mov     esi, upperTestText + 13
  lea     edi, [.tempStr]
  mov     ecx, 13
  xor     eax, eax
 .CompareUpper:
  mov     al, [esi]
  inc     esi
  sub     al, [edi]
  inc     edi
  dec     ecx
  jnz     .CompareUpper

  test    eax, eax
  jz      .StringIsUppercase
  push    freadFailed
  call    echostring
  push    dword 49
  call    [ExitProcess]
 .StringIsUppercase:

  ; Return to the middle of the file
  push    dword [.testFileHandle]
  push    dword 13
  call    fseek

  test    eax, eax
  jnz     .SeekMidSucceeded
  push    fseekFailed
  call    echostring
  push    dword 50
  call    [ExitProcess]
 .SeekMidSucceeded:

 .Exit:
  ; Close the file
  lea     eax, [.testFileHandle]
  push    eax
  call    fclose

  cmp     dword [.testFileHandle], 0
  je      .FileClosed
  push    fcloseFailed
  call    echostring
  push    dword 51
  call    [ExitProcess]
 .FileClosed:

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

                 dd 41
  testingCreate: db 13,10,'Testing create file read/write I/O...',13,10,0

                 dd 41
  testingUpdate: db 13,10,'Testing update file read/write I/O...',13,10,0

               dd 39
  testingOpen: db 13,10,'Testing oppen file read only I/O...',13,10,0

               dd 16
  testsPassed: db 13,10,'Tests passed',13,10,0

                 dd 18
  fcreateFailed: db 13,10,'fcreate FAILED',13,10,0

                 dd 18
  fupdateFailed: db 13,10,'fupdate FAILED',13,10,0

               dd 16
  fopenFailed: db 13,10,'fopen FAILED',13,10,0

                dd 17
  fcloseFailed: db 13,10,'fclose FAILED',13,10,0

               dd 16
  freadFailed: db 13,10,'fread FAILED',13,10,0

                 dd 18
  freadchFailed: db 13,10,'freadch FAILED',13,10,0

                dd 17
  fwriteFailed: db 13,10,'fwrite FAILED',13,10,0

                  dd 19
  fwritechFailed: db 13,10,'fwritech FAILED',13,10,0

               dd 16
  fseekFailed: db 13,10,'fseek FAILED',13,10,0

              dd 15
  fposFailed: db 13,10,'fpos FAILED',13,10,0

               dd 16
  fsizeFailed: db 13,10,'fsize FAILED',13,10,0

                 dd 18
  fseteofFailed: db 13,10,'fseteof FAILED',13,10,0

section .bss use32
