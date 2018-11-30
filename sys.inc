
; =====================
; Win32 threads/process
; =====================

  ;
  ; void ExitProcess( UINT uExitCode )
  ;
  extern ExitProcess
  import ExitProcess kernel32.dll

; =======================
; Win32 memory management
; =======================

  %define LHND          0x0042
  %define LMEM_FIXED    0x0000
  %define LMEM_MOVEABLE 0x0002
  %define LMEM_ZEROINIT 0x0040
  %define LMEM_MODIFY   0x0080
  %define LPTR          (LMEM_FIXED | LMEM_ZEROINIT)
  %define NONZEROLHND   LMEM_MOVEABLE
  %define NONZEROLPTR   LMEM_FIXED

  ;
  ; DECLSPEC_ALLOCATOR HLOCAL LocalAlloc(
  ;   UINT uFlags, SIZE_T uBytes )
  ;
  extern LocalAlloc
  import LocalAlloc kernel32.dll

  ;
  ; DECLSPEC_ALLOCATOR HLOCAL LocalReAlloc(
  ;   HLOCAL hMem, SIZE_T uBytes, UINT uFlags )
  ;
  extern LocalReAlloc
  import LocalReAlloc kernel32.dll

  ;
  ; HLOCAL LocalFree( HLOCAL hMem )
  ;
  extern LocalFree
  import LocalFree kernel32.dll

; ==============
; Win32 file I/O
; ==============

