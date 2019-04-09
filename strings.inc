%ifndef INC_STRINGS
%define INC_STRINGS

;
; func int Compare( string left, right )
;
Compare:
  push    ebp
  mov     ebp, esp

  %define .right ebp + 12
  %define .left  ebp + 8

  push    ebx
  push    esi
  push    edi

  mov     esi, [.rightt]
  mov     edi, [.left]

  mov     eax, 1

  test    esi, esi
  jz      .Exit

  test    edi, edi
  jz      .Exit

  ; TODO: Add code to change character case
 .Match:
  mov     al, [edi]
  mov     bl, [esi]
  sub     al, bl
  jne     .DoneMatch
  inc     edi
  inc     esi
  jmp     .Match
 .DoneMatch:

 .Exit:
  movsx   eax, al

  pop     edi
  pop     esi
  pop     ebx

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func int CaseCompare( string left, right )
;
CaseCompare:
  push    ebp
  mov     ebp, esp

  %define .right ebp + 12
  %define .left  ebp + 8

  push    ebx
  push    esi
  push    edi

  mov     esi, [.rightt]
  mov     edi, [.left]

  mov     eax, 1

  test    esi, esi
  jz      .Exit

  test    edi, edi
  jz      .Exit

 .Match:
  mov     al, [edi]
  mov     bl, [esi]
  sub     al, bl
  jne     .DoneMatch
  inc     edi
  inc     esi
  jmp     .Match
 .DoneMatch:

 .Exit:
  movsx   eax, al

  pop     edi
  pop     esi
  pop     ebx

  mov     esp, ebp
  pop     ebp
  ret 8

%endif
