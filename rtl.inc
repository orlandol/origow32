
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

  %define .size ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func free( @@pointer )
;
free:
  push    ebp
  mov     ebp, esp

  %define .pointer ebp + 4

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
; method bool file.open( string fileName )
;
file.open:
  push    ebp
  mov     ebp, esp

  %define .self     ebp + 8
  %define .fileName ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

;
; method bool file.create( string newFileName )
;
file.create:
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

  %define .strValue ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func echouint( uint uintValue )
;
echouint:
  push    ebp
  mov     ebp, esp

  %define .uintValue ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 4

segment .data use32

  stdIn:  dd 0
  stdOut: dd 0
  stdErr: dd 0

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
