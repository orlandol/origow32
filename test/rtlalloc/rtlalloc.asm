
  CPU 386

  BITS 32

segment .text use32

  %include "sys.inc"
  %include "rtl.inc"

..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  ; TODO: Test allocating 0 bytes
  ; TODO: Test resizing to 0 bytes
  ; TODO: Validate 2048 bytes immediately after resize

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
  push    dword 4
  call    [ExitProcess]
 .MemoryResized:
  mov     [testPtr], eax

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

  jz      .Validated4096
  push    strFailed
  call    echostring
  push    dword 3
  call    [ExitProcess]
 .Validated4096:

  ; Release memory
  push    strReleasing
  call    echostring

  push    testPtr
  call    free

  test    eax, eax
  jz      .MemoryReleased
  push    strFailed
  call    echostring
  push    dword 4
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

                   dd 22
  strAllocating:   db "Allocating memory...",13,10,0

                   dd 24
  strInitializing: db "Initializing memory...",13,10,0

                   dd 22
  strValidating:   db "Validating memory...",13,10,0

                   dd 20
  strResizing:     db "Resizing memory...",13,10,0

                   dd 21
  strReleasing:    db "Releasing memory...",13,10,0

                   dd 16
  strFailed:       db 13,10,"Tests FAILED",13,10,0

                   dd 16
  strPassed:       db 13,10,"Tests passed",13,10,0

section .bss use32

  testPtr: resd 1
