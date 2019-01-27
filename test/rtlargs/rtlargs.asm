
  CPU 386

  BITS 32

  %include "rtl.inc"

segment .text use32

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

                dd 23
  argCountText: db 13,10,"Number of arguments: ",0

                   dd 30
  listingArgsText: db 13,10,"Listing program arguments...",13,10,0

              dd 3
  argIdxOpen: db 13,10,"[",0

               dd 2
  argIdxClose: db "] ",0

section .bss use32
