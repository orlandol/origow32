
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"
  %include "console.inc"
  %include "ui.inc"

extern GetLastError
import GetLastError kernel32.dll

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  call    InitUI

  ; BOOL WINAPI WriteConsoleA( HANDLE  hConsoleOutput,
  ;   const VOID *lpBuffer, DWORD nNumberOfCharsToWrite,
  ;   LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved )
  push    dword 0
  push    dword 0
  push    dword [arrowStr - 4]
  push    arrowStr
  push    dword [stdOut]
  call    [WriteConsoleA]

  push    dword TRUE
  push    interceptCtrl
  call    [SetConsoleCtrlHandler]

 .Exit:
  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

; func bool WINAPI CtrlHandler( DWORD fdwCtrlType )
interceptCtrl:
  push    ebp
  mov     ebp, esp

  %define .fdwCtrlType ebp + 8

  %define .result ebp - 4
  push    dword 0

  cmp     dword [.fdwCtrlType], CTRL_C_EVENT
  jne     .Exit
  mov     dword [.result], TRUE
  jmp     .Exit

 .Exit:
  mov     eax, [.result]
  mov     esp, ebp
  pop     ebp
  ret 4

segment .data use32

  declstring arrowStr, 16, 17, 30, 31

  inputBuffer: times (128 * SIZEOF_INPUT_RECORD) db 0

section .bss use32
