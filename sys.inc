%ifndef INC_SYS
%define INC_SYS

; ===========
; Win32 types
; ===========

  %define FALSE 0
  %define TRUE  1

; ====================
; Win32 error messages
; ====================

  %define INVALID_HANDLE_VALUE (-1)

  ;
  ; DWORD WINAPI GetLastError()
  ;
  extern GetLastError
  import GetLastError kernel32.dll

  ;
  ; DWORD FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId,
  ;   DWORD dwLanguageId, LPTSTR lpBuffer, DWORD nSize, va_list *Arguments )
  ;
  extern FormatMessageA
  import FormatMessageA kernel32.dll

; ==================
; Win32 command-line
; ==================

  ;
  ; LPTSTR WINAPI GetCommandLine()
  ;
  extern GetCommandLineA
  import GetCommandLineA kernel32.dll

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

  %define GENERIC_ALL     0x10000000
  %define GENERIC_EXECUTE 0x20000000
  %define GENERIC_WRITE   0x40000000
  %define GENERIC_READ    0x80000000

  %define CREATE_NEW        1
  %define CREATE_ALWAYS     2
  %define OPEN_EXISTING     3
  %define OPEN_ALWAYS       4
  %define TRUNCATE_EXISTING 5

  %define FILE_ATTRIBUTE_NORMAL 128

  %define FILE_BEGIN   0
  %define FILE_CURRENT 1
  %define FILE_END     2

  %define INVALID_SET_FILE_POINTER (-1)
  %define INVALID_FILE_SIZE (-1)

  %define MAX_PATH 260

  ;
  ; HANDLE CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess,
  ;   DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  ;   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
  ;   HANDLE hTemplateFile )
  ;
  extern CreateFileA
  import CreateFileA kernel32.dll

  ;
  ; BOOL CloseHandle( HANDLE hObject )
  ;
  extern CloseHandle
  import CloseHandle kernel32.dll

  ;
  ; BOOL ReadFile( HANDLE hFile, LPVOID lpBuffer,
  ;   DWORD nNumberOfBytesToRead, LPDWORD lpNumberofBytesRead,
  ;   LPOVERLAPPED lpOverlapped )
  ;
  extern ReadFile
  import ReadFile kernel32.dll

  ;
  ; BOOL WriteFile( HANDLE hFile, LPVOID lpBuffer,
  ;   DWORD nNumberOfBytesToWrite, LPDWORD lpNumberofBytesWritten,
  ;   LPOVERLAPPED lpOverlapped )
  ;
  extern WriteFile
  import WriteFile kernel32.dll

  ;
  ; BOOL SetEndOfFile( HANDLE hFile )
  ;
  extern SetEndOfFile
  import SetEndOfFile kernel32.dll

  ;
  ; DWORD SetFilePointer( HANDLE hFile, LONG lDistanceToMove,
  ;   PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod )
  ;
  extern SetFilePointer
  import SetFilePointer kernel32.dll

  ;
  ; DWORD GetFileSize( HANDLE hFile, LPDWORD lpFileSizeHigh )
  ;
  extern GetFileSize
  import GetFileSize kernel32.dll

  ;
  ; BOOL CreateDirectoryA( LPCSTR lpPathName,
  ;   LPSECURITY_ATTRIBUTES lpSecurityAttributes )
  ;
  extern CreateDirectoryA
  import CreateDirectoryA kernel32.dll

  ;
  ; BOOL RemoveDirectoryA( LPCSTR lpPathName )
  ;
  extern RemoveDirectoryA
  import RemoveDirectoryA kernel32.dll

  ;
  ; DWORD GetCurrentDirectory(
  ;   DWORD nBufferLength, LPTSTR lpBuffer )
  ;
  extern GetCurrentDirectoryA
  import GetCurrentDirectoryA kernel32.dll

  ;
  ; BOOL SetCurrentDirectory( LPCTSTR lpPathName )
  ;
  extern SetCurrentDirectoryA
  import SetCurrentDirectoryA kernel32.dll

; =================
; Win32 Console I/O
; =================

  %define STD_INPUT_HANDLE  (-10)
  %define STD_OUTPUT_HANDLE (-11)
  %define STD_ERROR_HANDLE  (-12)

  ;
  ; HANDLE WINAPI GetStdHandle( DWORD nStdHandle )
  ;
  extern GetStdHandle
  import GetStdHandle kernel32.dll

%endif
