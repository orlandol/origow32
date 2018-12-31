
segment .text use32

;
; func dump()
;
dump:
  push    ebp
  mov     ebp, esp

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
  push    dword [.dumpEBX]
  call    dumpHex

  push    dword (dumpLine2 + 51)
  push    dword [.dumpEBX]
  call    dumpUint

  ; uint(ecx)
  push    dword (dumpLine3 +  10)
  push    dword [.dumpECX]
  call    dumpHex

  push    dword (dumpLine3 + 20)
  push    dword [.dumpECX]
  call    dumpUint

  ; uint(edx)
  push    dword (dumpLine3 +  41)
  push    dword [.dumpEDX]
  call    dumpHex

  push    dword (dumpLine3 + 51)
  push    dword [.dumpEDX]
  call    dumpUint

  ; uint(esi)
  push    dword (dumpLine4 +  10)
  push    dword [.dumpESI]
  call    dumpHex

  push    dword (dumpLine4 + 20)
  push    dword [.dumpESI]
  call    dumpUint

  ; uint(edi)
  push    dword (dumpLine4 +  41)
  push    dword [.dumpEDI]
  call    dumpHex

  push    dword (dumpLine4 + 51)
  push    dword [.dumpEDI]
  call    dumpUint

  ; uint(ebp)
  push    dword (dumpLine5 +  10)
  push    dword [.dumpEBP]
  call    dumpHex

  push    dword (dumpLine5 + 20)
  push    dword [.dumpEBP]
  call    dumpUint

  ; uint(esp)
  push    dword (dumpLine5 +  41)
  push    ebp
  call    dumpHex

  push    dword (dumpLine5 + 51)
  push    ebp
  call    dumpUint

  ; Display the values
  push    dumpLine
  call    dumpEcho

  push    dword 0
  call    [ExitProcess]

  mov     esp, ebp
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
; func string[10] dumpUint( uint value )
;
dumpUint:
  push    ebp
  mov     ebp, esp

  push    edi
  push    edx
  push    ecx

  %define .result ebp + 12
  %define .value  ebp + 8

  mov     eax, [.value]
  mov     ecx, 10

  mov     edi, [.result]
  test    edi, edi
  jz      .Exit
  add     edi, 9

 .ConvertLoop:
  test    eax, eax
  jz      .Exit

  xor     edx, edx
  div     ecx

  add     dl, '0'
  mov     [edi], dl
  dec     edi
  jmp     .ConvertLoop

 .Exit:
  pop     ecx
  pop     edx
  pop     edi

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func string[8] dumpHex( uint value )
;
dumpHex:
  push    ebp
  mov     ebp, esp

  %define .result ebp + 12
  %define .value  ebp + 8

  mov     eax, [.value]
  mov     ecx, 8

  mov     edi, [.result]
  test    edi, edi
  jz      .Exit
  add     edi, 7

 .ConvertLoop:
  mov     ebx, eax
  shr     eax, 4
  and     ebx, 0x0000000F

  mov     dl, [ebx + dumpHexDigits]
  mov     [edi], dl
  dec     edi

  dec     ecx
  jnz     .ConvertLoop

 .Exit:
  mov     esp, ebp
  pop     ebp
  ret 8

section .data use32

  dumpHexDigits: db '0123456789ABCDEF'

  dumpLine:  db 13, 10
  dumpLine1: db '  --  Registers ----------------------------------------------',13,10
  dumpLine2: db '  eax = 0x00000000 (0000000000)  ebx = 0x00000000 (0000000000)',13,10
  dumpLine3: db '  ecx = 0x00000000 (0000000000)  edx = 0x00000000 (0000000000)',13,10
  dumpLine4: db '  esi = 0x00000000 (0000000000)  edi = 0x00000000 (0000000000)',13,10
  dumpLine5: db '  ebp = 0x00000000 (0000000000)  esp = 0x00000000 (0000000000)',13,10
  dumpLine6: db '  ------------------------------------------------------------',13,10,0
