
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

  mov     edx, 1

  ; Test standalone displacement generation
  xor     esi, esi
  mov     ecx, 4
 .DisplacementTest:
  test    ecx, ecx
  jz      .DoneDisplacement
  push    dword [fileHandle]
  push    dword x86Call
  push    dword x86SRegSS
  push    dword 0
  push    dword 0
  push    dword 0
  mov     eax, [esi + displacementValue]
  add     esi, 4
  push    dword eax
  push    dword 0
  call    x86GenOpMem
  test    eax, eax
  jz      .DisplacementError
  inc     edx
  dec     ecx
  jmp     .DisplacementTest
 .DisplacementError:
  push    testFailed
  call    echostring
  push    eoln
  call    echostring
  push    edx
  call    [ExitProcess]
 .DoneDisplacement:

  ; Test ESP as index with/without scale values
  xor     esi, esi
  mov     ecx, 4
 .espIndexTest:
  test    ecx, ecx
  jz      .espIndexDone
  push    dword [fileHandle]
  push    dword x86Call
  push    dword 0
  push    dword x86RegESP
  mov     eax, [esi + indexScale]
  add     esi, 4
  push    dword eax
  push    dword 0
  push    dword 0
  push    dword 0
  call    x86GenOpMem
  test    eax, eax
  jnz     .espIndexError
  inc     edx
  dec     ecx
  jmp     .espIndexTest
 .espIndexError:
  push    testFailed
  call    echostring
  push    eoln
  call    echostring
  push    edx
  call    [ExitProcess]
 .espIndexDone:

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
  declstring testsPassed, "Tests passed"

  fileHandle: dd 0

  segRegister:
    dd x86SRegES
    dd x86SRegCS
    dd x86SRegSS
    dd x86SRegDS
    dd x86SRegFS
    dd x86SRegGS

  baseRegister:
    dd x86RegEAX
    dd x86RegECX
    dd x86RegEDX
    dd x86RegEBX
    dd x86RegESP
    dd x86RegEBP
    dd x86RegESI
    dd x86RegEDI

  indexRegister:
    dd x86RegEAX
    dd x86RegECX
    dd x86RegEDX
    dd x86RegEBX
    dd x86RegESP
    dd x86RegEBP
    dd x86RegESI
    dd x86RegEDI

  indexScale:
    dd 0
    dd 1
    dd 2
    dd 4
    dd 8

  displacementValue:
    dd 0
    dd 0x12
    dd 0x1234
    dd 0x12345678

section .bss use32
