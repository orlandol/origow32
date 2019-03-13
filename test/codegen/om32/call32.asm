
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"
  %include "codegen.inc"

  %define x86Mnemonic x86Call

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

  ; TODO: Write single test loop to iterate through test data

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

  ; TODO: Create array of parameter, result, exit codes, and error strings

section .bss use32
