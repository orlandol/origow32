
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  ; TODO: Test allocating 0 bytes
  ; TODO: Test resizing to 0 bytes

  ; Test memory allocation
  push    strAllocating
  call    echostring

  push    dword 2048
  call    alloc

  test    eax, eax
  jnz     .MemoryAllocated
  push    strFailed
  call    echostring
  push    dword 1
  call    [ExitProcess]
 .MemoryAllocated:
  mov     [testPtr], eax

  ; Initialize memory
  push    strInitializing
  call    echostring

  mov     edi, [testPtr]
  mov     ecx, 2048/4
  mov     eax, 0x5C5C5C5C
  rep stosd

  ; Validate memory
  push    strValidating
  call    echostring

  mov     edi, [testPtr]
  mov     ecx, 2048/4
  mov     eax, 0x5C5C5C5C
  rep scasd

  jz      .ValidatedInit2048
  push    strFailed
  call    echostring
  push    dword 2
  call    [ExitProcess]
 .ValidatedInit2048:

  ; Resize memory
  push    strResizing
  call    echostring

  push    dword [testPtr]
  push    dword 4096
  call    realloc

  test    eax, eax
  jnz     .MemoryResized
  push    strFailed
  call    echostring
  push    dword 3
  call    [ExitProcess]
 .MemoryResized:
  mov     [testPtr], eax

  ; Validate memory
  push    strValidating
  call    echostring

  mov     edi, [testPtr]
  mov     ecx, 2048/4
  mov     eax, 0x5C5C5C5C
  rep scasd

  jz      .ValidatedRealloc2048
  push    strFailed
  call    echostring
  push    dword 4
  call    [ExitProcess]
 .ValidatedRealloc2048:

  ; Initializing memory
  push    strInitializing
  call    echostring

  mov     edi, [testPtr]
  mov     ecx, 4096/4
  mov     eax, 0xC1C1C1C1
  rep stosd

  ; Validate memory
  push    strValidating
  call    echostring

  mov     edi, [testPtr]
  mov     ecx, 4096/4
  mov     eax, 0xC1C1C1C1
  rep scasd

  jz      .ValidatedInit4096
  push    strFailed
  call    echostring
  push    dword 5
  call    [ExitProcess]
 .ValidatedInit4096:

  ; Release memory
  push    strReleasing
  call    echostring

  push    testPtr
  call    free

  test    eax, eax
  jz      .MemoryReleased
  push    strFailed
  call    echostring
  push    dword 6
  call    [ExitProcess]
 .MemoryReleased:

  ; Done
  push    strPassed
  call    echostring

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

  declstring strAllocating, 13,10,'Allocating memory...',13,10
  declstring strInitializing, 'Initializing memory...',13,10
  declstring strValidating, 'Validating memory...',13,10
  declstring strResizing, 'Resizing memory...',13,10
  declstring strReleasing, 'Releasing memory...',13,10
  declstring strFailed, 13,10,'Test FAILED',13,10
  declstring strPassed, 13,10,'Tests passed',13,10

section .bss use32

  testPtr: resd 1
