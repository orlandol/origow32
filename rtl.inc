
  %include "sys.inc"

; =================
; Memory management
; =================

segment .text

;
; func @ alloc( usize size )
;
alloc:
  ret 4

;
; func free( @@ptr )
;
free:
  ret 4

;
; func @ realloc( @@ptr; usize newSize )
;
realloc:
  ret 8

; ========
; File I/O
; ========

struc file
  handle: resd 1
endstruc

segment .text

;
; method bool file.open( string fileName )
;
file.open:
  push    ebp
  mov     ebp, esp

  %define self ebp + 4

  mov     esp, ebp
  pop     ebp
  ret 8

;
; method bool file.create( string newFileName )
;
file.create:
  ret 8
