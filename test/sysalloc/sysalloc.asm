
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
  call    sysecho

  push    dword 2048
  push    LPTR
  call    [LocalAlloc]

  test    eax, eax
  jnz     .MemoryAllocated
  push    strFailed
  call    sysecho
  push    dword 1
  call    [ExitProcess]
 .MemoryAllocated:
  mov     [testPtr], eax

  ; Initialize memory
  push    strInitializing
  call    sysecho

  mov     edi, [testPtr]
  mov     ecx, 2048/4
  mov     eax, 0x5C5C5C5C
  rep stosd

  ; Validate memory
  push    strValidating
  call    sysecho

  mov     edi, [testPtr]
  mov     ecx, 2048/4
  mov     eax, 0x5C5C5C5C
  rep scasd

  jz      .Validated2048
  push    strFailed
  call    sysecho
  push    dword 3
  call    [ExitProcess]
 .Validated2048:

  ; Resize memory
  push    strResizing
  call    sysecho

  push    dword LMEM_MOVEABLE
  push    dword 4096
  push    dword [testPtr]
  call    [LocalReAlloc]

  test    eax, eax
  jnz     .MemoryResized
  push    strFailed
  call    sysecho
  push    dword 4
  call    [ExitProcess]
 .MemoryResized:
  mov     [testPtr], eax

  ; Initializing memory
  push    strInitializing
  call    sysecho

  mov     edi, [testPtr]
  mov     ecx, 4096/4
  mov     eax, 0xC1C1C1C1
  rep stosd

  ; Validate memory
  push    strValidating
  call    sysecho

  mov     edi, [testPtr]
  mov     ecx, 4096/4
  mov     eax, 0xC1C1C1C1
  rep scasd

  jz      .Validated4096
  push    strFailed
  call    sysecho
  push    dword 3
  call    [ExitProcess]
 .Validated4096:

  ; Release memory
  push    strReleasing
  call    sysecho

  push    dword [testPtr]
  call    [LocalFree]

  test    eax, eax
  jz      .MemoryReleased
  push    strFailed
  call    sysecho
  push    dword 4
  call    [ExitProcess]
 .MemoryReleased:

  ; Done
  push    strPassed
  call    sysecho

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

;
; func sysecho( string strValue )
;
sysecho:
  push    ebp
  mov     ebp, esp

  %define .dumpText ebp + 8

  %define .tempWritten ebp - 4
  push    dword 0

  push    ebx
  push    ecx
  push    edx
  push    esi

  push    dword STD_OUTPUT_HANDLE
  call    [GetStdHandle]

  mov     esi, [.dumpText]
  test    esi, esi
  jz      .Exit

  xor     ecx, ecx
 .CalcSize:
  mov     dl, [esi + ecx]
  test    dl, dl
  jz      .DoneCalc
  inc     ecx
  jmp     .CalcSize
 .DoneCalc:

  lea     ebx, [.tempWritten]

  push    dword 0
  push    ebx
  push    ecx
  push    esi
  push    eax
  call    [WriteFile]

 .Exit:
  pop     esi
  pop     edx
  pop     ecx
  pop     ebx

  mov     esp, ebp
  pop     ebp
  ret 4

segment .data use32

  strAllocating:   db "Allocating memory...",13,10,0
  strInitializing: db "Initializing memory...",13,10,0
  strValidating:   db "Validating memory...",13,10,0
  strResizing:     db "Resizing memory...",13,10,0
  strReleasing:    db "Releasing memory...",13,10,0
  strFailed:       db 13,10,"Tests FAILED",13,10,0
  strPassed:       db 13,10,"Tests passed",13,10,0

section .bss use32

  testPtr: resd 1
