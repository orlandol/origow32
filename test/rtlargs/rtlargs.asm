
  CPU 386

  BITS 32

  %include 'rtl.inc'

segment .text use32

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL
  ; Duplicate InitRTL to test internal duplicate initialization guard
  call    InitRTL

  push    argCountText
  call    echostring
  push    dword [argc]
  call    echouint
  push    eoln
  call    echostring

  push    listingArgsText
  call    echostring

  xor     ecx, ecx
  mov     ebx, [argv]
 .ArgLoop:

  cmp     ecx, [argc]
  je      .DoneArgs

  push    argIdxOpen
  call    echostring
  push    ecx
  call    echouint
  push    argIdxClose
  call    echostring

  push    dword [ebx]
  call    echostring

  add     ebx, 4
  inc     ecx
  jmp     .ArgLoop
 .DoneArgs:

  push    eoln
  call    echostring

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

  declstring argCountText, 13,10,'Number of arguments: '
  declstring listingArgsText, 13,10,'Listing program arguments...',13,10
  declstring argIdxOpen, 13,10,'['
  declstring argIdxClose, '] '

section .bss use32
