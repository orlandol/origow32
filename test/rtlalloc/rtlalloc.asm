
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

  ; Test memory allocation
  push    strAllocating
  call    echostring

  ; Initialize memory
  push    strInitializing
  call    echostring

  ; Validate memory
  push    strValidating
  call    echostring

  ; Resize memory
  push    strResizing
  call    echostring

  ; Initializing memory
  push    strInitializing
  call    echostring

  ; Validate memory
  push    strValidating
  call    echostring

  ; Release memory
  push    strReleasing
  call    echostring

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
