
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

  %define size ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func free( @@pointer )
;
free:
  push    ebp
  mov     ebp, esp

  %define pointer ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func @ realloc( @@pointer; usize newSize )
;
realloc:
  push    ebp
  mov     ebp, esp

  %define pointer ebp + 8
  %define newSize ebp + 4

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

  %define self ebp + 8
  %define fileName ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

;
; method bool file.create( string newFileName )
;
file.create:
  push    ebp
  mov     ebp, esp

  %define self ebp + 8
  %define newFileName ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

; ===========
; Console I/O
; ===========

segment .text use32

;
; func echostring( string outText )
;
echostring:
  push    ebp
  mov     ebp, esp

  %define outText ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 4

segment .data use32

  stdIn:  dd 0
  stdOut: dd 0
  stdErr: dd 0

