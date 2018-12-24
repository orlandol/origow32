
segment .text use32

;
; func dump()
;
dump:
  push    ebp
  mov     esp, ebp

  ; ebp == (esp - 8) ?
  %define .dumpEBP ebp + 0
  %define .dumpEAX ebp - 24
  %define .dumpEBX ebp - 20
  %define .dumpECX ebp - 16
  %define .dumpEDX ebp - 12
  %define .dumpESI ebp - 8
  %define .dumpEDI ebp - 4

  push    edi
  push    esi
  push    edx
  push    ecx
  push    ebx
  push    eax

  ; uint(eax)
  push    dword (dumpLine2 +  10)
  push    dword [.dumpEAX]
  call    dumpHex

  push    dword (dumpLine2 + 20)
  push    dword [.dumpEAX]
  call    dumpUint

  ; uint(ebx)
  push    dword (dumpLine2 +  41)
  push    dword [.dumpEAX]
  call    dumpHex

  push    dword (dumpLine2 + 51)
  push    dword [.dumpEAX]
  call    dumpUint

  push    dumpLine
  call    dumpEcho

  push    dword 0
  call    [ExitProcess]

  mov     ebp, esp
  pop     ebp
  ret

;
; func dumpEcho( string dumpText )
;
dumpEcho:
  push    ebp
  mov     ebp, esp

  %define .dumpText ebp + 8

  push    dword STD_OUTPUT_HANDLE
  call    [GetStdHandle]

  %define .tempWritten ebp - 4
  push    dword 0

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
  push    dword 0
  call    [ExitProcess]

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func string dumpUint( uint value )
;
dumpUint:
  push    ebp
  mov     ebp, esp

  %define .value ebp + 8

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func string dumpHex( uint value )
;
dumpHex:
  push    ebp
  mov     ebp, esp

  %define .value ebp + 8

  mov     esp, ebp
  pop     ebp
  ret 4

section .data use32

  dumpHexDigits: db '0123456789ABCDEF'

  dumpLine:  db 13, 10
  dumpLine1: db '  --  Registers ----------------------------------------------',13,10
  dumpLine2: db '  eax = 0x00000000 (0000000000)  ebx = 0x00000000 (0000000000)',13,10
  dumpLine3: db '  ecx = 0x00000000 (0000000000)  edx = 0x00000000 (0000000000)',13,10
  dumpLine4: db '  esi = 0x00000000 (0000000000)  edi = 0x00000000 (0000000000)',13,10
  dumpLine5: db '  ebp = 0x00000000 (0000000000)  esp = 0x00000000 (0000000000)',13,10
  dumpLine6: db '  ------------------------------------------------------------',13,10,0
