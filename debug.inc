
segment .text use32

;
; func dump()
;
dump:
  push    ebp
  mov     esp, ebp

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

  push    dword 0
  call    [ExitProcess]

  mov     ebp, esp
  pop     ebp
  ret

;
; func string dumpUint( uint value )
;
dumpUint:
  push    ebp
  mov     ebp, esp

  %define .value

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func string dumpUint32( uint value )
;
dumpUint32:
  push    ebp
  mov     ebp, esp

  %define .value

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func string dumpHex( uint value )
;
dumpHex:
  push    ebp
  mov     ebp, esp

  %define .value

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func string dumpHex32( uint value )
;
dumpHex32:
  push    ebp
  mov     ebp, esp

  %define .value

  mov     esp, ebp
  pop     ebp
  ret 8

section .data use32

  dumpLine1: db '  --  Registers ----------------------------------------------',13,10,
  dumpLine2: db '  eax = 0x00000000 (0000000000)  ebx = 0x00000000 (0000000000)',13,10,
  dumpLine3: db '  ecx = 0x00000000 (0000000000)  edx = 0x00000000 (0000000000)',13,10,
  dumpLine4: db '  esi = 0x00000000 (0000000000)  edi = 0x00000000 (0000000000)',13,10,
  dumpLine5: db '  ebp = 0x00000000 (0000000000)  esp = 0x00000000 (0000000000)',13,10,
  dumpLine6: db '  ------------------------------------------------------------',13,10,0
