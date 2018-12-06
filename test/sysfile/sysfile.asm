
  CPU 386

  BITS 32

segment .text use32

  %include "sys.inc"
  %include "rtl.inc"
  
..start:
  push    ebp
  mov     ebp, esp

  push    dword STD_OUTPUT_HANDLE
  call    [GetStdHandle]
  mov     [stdOut], eax

  call    [GetCommandLineA]
  push    eax
  mov     esi, eax
  xor     ecx, ecx
 .LenLoop:
  mov     al, [esi]
  inc     esi
  test    al, al
  jz      .ExitLoop
  inc     ecx
  jmp     .LenLoop
 .ExitLoop:
  push    ecx
  call    sysecho

  push    dword 0
  call    [ExitProcess]

;
; func sysecho( string outText, usize textLen )
;
sysecho:
  push    ebp
  mov     ebp, esp

  %define outText ebp + 12
  %define textLen ebp + 8

  %define tempWritten ebp - 4
  push    dword 0

  push    dword 0
  lea     eax, [tempWritten]
  push    eax
  push    dword [textLen]
  push    dword [outText]
  push    dword [stdOut]
  call    [WriteFile]

  mov     esp, ebp
  pop     ebp
  ret 8

segment .data use32

  digits:    db "0123456789", 0
  lenDigits: dd $ - digits - 1

section .bss use32
