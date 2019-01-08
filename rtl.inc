
%include "sys.inc"

; =========
; RTL state
; =========

segment .text use32

;
; func InitRTL()
;
InitRTL:
  push    ebp
  mov     ebp, esp

  push    dword STD_INPUT_HANDLE
  call    [GetStdHandle]
  mov     [stdIn], eax

  push    dword STD_OUTPUT_HANDLE
  call    [GetStdHandle]
  mov     [stdOut], eax

  push    dword STD_ERROR_HANDLE
  call    [GetStdHandle]
  mov     [stdErr], eax

  mov     esp, ebp
  pop     ebp
  ret

;
; func usize CommandLineLength()
;
CommandLineLength:
  push    ebp
  mov     ebp, esp
  push    ebx
  push    ecx
  push    edx
  push    esi
  push    edi

  call    [GetCommandLineA]

  mov     esi, eax
  xor     ecx, ecx

  test    esi, esi
  je      .DoneCounting

 .SkipSpace:
  mov     al, [esi]
  test    al, al
  jz      .DoneCounting
  inc     esi
  cmp     al, '"'
  je      .CountString
  cmp     al, ' '
  je      .SkipSpace
  cmp     al, 9 ; Tab
  je      .SkipSpace
  cmp     al, 13 ; CR
  je      .SkipSpace
  cmp     al, 10 ; LF
  je      .SkipSpace

 .CountCharacters:
  mov     al, [esi]
  test    al, al
  jz      .DoneCounting
  inc     esi
  cmp     al, '"'
  je      .CountString
  inc     ecx
  jmp     .CountCharacters

 .CountString:
  mov     al, [esi]
  test    al, al
  jz      .DoneCounting
  inc     esi
  cmp     al, '"'
  je      .SkipSpace
  inc     ecx
  jmp     .CountString

 .DoneCounting:
  mov     eax, ecx

  pop     edi
  pop     esi
  pop     edx
  pop     ecx
  pop     ebx
  mov     esp, ebp
  pop     ebp
  ret

;
; func unsigned ArgCount( string commandLine; usize commandLength )
;
ArgCount:
  push    ebp
  mov     ebp, esp

  %define .commandLine   ebp + 12
  %define .commandLength ebp + 8

  mov     esp, ebp
  pop     ebp
  ret 8

; =================
; Memory management
; =================

segment .text use32

;
; func @ alloc( usize size )
;
alloc:
  push    ebp
  mov     ebp, esp
  push    ecx

  %define .size ebp + 4
  mov     ecx, [.size]
  test    ecx, ecx
  jz      .Exit

  add     ecx, 4

  push    ecx
  push    LPTR
  call    [LocalAlloc]

  test    eax, eax
  jz      .Exit

  mov     ecx, [.size]
  mov     [eax + 4], ecx
  add     eax, 4

 .Exit:
  pop     ecx
  mov     esp, ebp
  pop     ebp
  ret 4

;
; func free( ref pointer )
;
free:
  push    ebp
  mov     ebp, esp
  push    esi
  push    edi

  %define .pointer ebp + 4

  ; Load and validate reference pointer
  mov     esi, [.pointer]
  test    esi, esi
  jz      .Exit

  ; Load and validate pointer
  mov     edi, [esi]
  test    edi, edi
  jz      .Exit

  ; Offset and release memory
  sub     edi, 4
  push    edi
  call    [LocalFree]

  ; Set pointer to null if released
  test    eax, eax
  jnz     .Exit
  mov     dword [edi], 0

 .Exit:
  pop     edi
  pop     esi
  mov     esp, ebp
  pop     ebp
  ret 4

;
; func @ realloc( @@pointer; usize newSize )
;
realloc:
  push    ebp
  mov     ebp, esp

  %define .pointer ebp + 8
  %define .newSize ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

; ========
; File I/O
; ========

segment .text use32

struc file
  handle: resd 1
endstruc

segment .text use32

;
; method bool fopen( string fileName )
;
fopen:
  push    ebp
  mov     ebp, esp

  %define .self     ebp + 8
  %define .fileName ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

;
; method bool fcreate( string newFileName )
;
fcreate:
  push    ebp
  mov     ebp, esp

  %define .self        ebp + 8
  %define .newFileName ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

; ===========
; Console I/O
; ===========

segment .text use32

;
; func echostring( string strValue )
;
echostring:
  push    ebp
  mov     ebp, esp

  %define .strValue ebp + 8

  %define .tempWritten ebp - 4
  push    dword 0

  push    esi

  mov     esi, [.strValue]
  test    esi, esi
  jz      .Exit

  push    dword 0
  lea     eax, [.tempWritten]
  push    eax
  push    dword [esi - 4] ; length
  push    esi
  push    dword [stdOut]
  call    [WriteFile]

 .Exit
  pop     esi
  mov     esp, ebp
  pop     ebp
  ret 4

;
; func echouint( uint uintValue )
;
echouint:
  push    ebp
  mov     ebp, esp

  %define .uintValue ebp + 8

  %define .valueStr ebp - 12
  push    dword 0
  push    dword 0
  push    dword 0

  %define .tempWritten ebp - 16
  push    dword 0

  push    ebx
  push    ecx
  push    edi

  mov     eax, [.uintValue]
  lea     edi, [.valueStr + 11]
  mov     ebx, 10
  xor     ecx, ecx

 .ConvertLoop:
  xor     edx, edx
  div     ebx
  add     dl, '0'
  mov     [edi], dl
  inc     ecx
  test    eax, eax
  jz      .DoneConverting
  dec     edi
  jmp     .ConvertLoop
 .DoneConverting:

  push    dword 0
  lea     eax, [.tempWritten]
  push    eax
  push    ecx
  push    edi
  push    dword [stdOut]
  call    [WriteFile]

 .Exit:
  pop     edi
  pop     ecx
  pop     ebx
  mov     esp, ebp
  pop     ebp
  ret 4

segment .data use32

  stdIn:  dd 0
  stdOut: dd 0
  stdErr: dd 0

        dd 2
  eoln: db 13, 10, 0

; =======
; Strings
; =======

;
; func length( string ofString )
;
length:
  push    ebp
  mov     ebp, esp

  %define .ofString ebp + 8

  mov     esp, ebp
  pop     ebp
  ret 8
