
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"
  %include "codegen.inc"

  %define TPARAM_SEGREG   0
  %define TPARAM_BASEREG  4
  %define TPARAM_INDEXREG 8
  %define TPARAM_SCALE    12
  %define TPARAM_DISP     16
  %define TPARAM_MEMSIZE  20
  %define TPARAM_RESULT   24
  %define TPARAM_ERRSTR   28
  %define TPARAM_EXITCODE 32

  %define SIZEOF_TESTITEM 36

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  push    fileName
  call    fcreate
  mov     [fileHandle], eax
  test    eax, eax
  jnz     .fcreateSucceeded
  push    fcreateFailed
  call    echostring
  push    eoln
  call    echostring
  push    dword 1
  call    [ExitProcess]
 .fcreateSucceeded:

  ; Run each test, using test data
  mov     esi, testParameters
 .TestLoop:
  cmp     esi, testParametersEnd
  jae     .DoneTest
  push    dword [fileHandle]
  push    dword x86Call
  push    dword [esi + TPARAM_SEGREG]
  push    dword [esi + TPARAM_BASEREG]
  push    dword [esi + TPARAM_INDEXREG]
  push    dword [esi + TPARAM_SCALE]
  push    dword [esi + TPARAM_DISP]
  push    dword [esi + TPARAM_MEMSIZE]
  call    x86GenOpMem

  cmp     eax, dword [esi + TPARAM_RESULT]
  je      .Succeeded
  push    dword [esi + TPARAM_ERRSTR]
  call    echostring
  push    eoln
  call    echostring
  push    dword [esi + TPARAM_EXITCODE]
  call    [ExitProcess]
 .Succeeded:
  add     esi, SIZEOF_TESTITEM
  jmp     .TestLoop
 .DoneTest:

  ; All tests passed
  push    testsPassed
  call    echostring
  push    eoln
  call    echostring

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

  declstring testFailed, "Test FAILED"
  declstring shouldFail, "Test SHOULD HAVE FAILED"
  declstring testsPassed, "Tests passed"

  fileHandle: dd 0

  testParameters:
    ; call [gs:0]
    dd x86SRegGS
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 1
    dd testFailed
    dd 1
    ; call [gs:0x12]
    dd x86SRegGS
    dd 0
    dd 0
    dd 0
    dd 0x12
    dd 0
    dd 1
    dd testFailed
    dd 2
    ; call [gs:0x1234]
    dd x86SRegGS
    dd 0
    dd 0
    dd 0
    dd 0x1234
    dd 0
    dd 1
    dd testFailed
    dd 3
    ; call [gs:0x12345678]
    dd x86SRegGS
    dd 0
    dd 0
    dd 0
    dd 0x12345678
    dd 0
    dd 1
    dd testFailed
    dd 4
    ; [* 4]
    dd 0
    dd 0
    dd 0
    dd 4
    dd 0
    dd 0
    dd 0
    dd shouldFail
    dd 5
  testParametersEnd:

section .bss use32
