%ifndef INC_RTL
%define INC_RTL

%include "sys.inc"
%include "chtable.inc"

; ==========
; RTL macros
; ==========

  %macro declstring 2+
        dd %%strEnd - %%str - 1
    %%str:
    %1: db %2,0
    %%strEnd:
  %endmacro

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

  call    InitArgs

  mov     esp, ebp
  pop     ebp
  ret

;
; func InitArgs()
;
InitArgs:
  push    ebp
  mov     ebp, esp

  push    dword 0
  %define .commandLine ebp - 4

  push    dword 0
  %define .argc ebp - 8

  push    dword 0
  %define .argv ebp - 12

  push    dword 0
  %define .argSize ebp - 16

  push    ebx
  push    ecx
  push    edx
  push    esi
  push    edi

  ; Only execute once
  cmp     dword [argv], 0
  jne     .Exit

  call    [GetCommandLineA]
  mov     [.commandLine], eax

  mov     esi, eax
  xor     eax, eax
  test    esi, esi
  jz      .Exit

  ; Calculate argc, and the size needed for argv
  mov     al, [esi]
  inc     esi

 .SkipSpace:
  test    al, al
  jz      .DoneCounting
  test    byte [eax + chTable], chSpace
  jz      .DoneSpace
  mov     al, [esi]
  inc     esi
  jmp     .SkipSpace
 .DoneSpace:

  cmp     al, '"'
  jne     .NotString
  mov     al, [esi]
  inc     esi
 .CountString:
  test    al, al
  jz      .DoneString
  cmp     byte [eax + chTable], 0
  je      .InvalidArgument
  cmp     al, '"'
  je      .ClosedString
  mov     al, [esi]
  inc     esi
  inc     dword [.argSize]
  jmp     .CountString
 .ClosedString:
  mov     al, [esi]
  inc     esi
 .DoneString:
  inc     dword [.argc]
  add     dword [.argSize], 9 ; ptr + size + null
  jmp     .SkipSpace
 .NotString:

 .CountChar:
  test    al, al
  jz      .DoneString
  cmp     byte [eax + chTable], 0
  je      .InvalidArgument
  test    byte [eax + chTable], chSpace
  jnz     .DoneChar
  mov     al, [esi]
  inc     esi
  inc     dword [.argSize]
  jmp     .CountChar
 .DoneChar:
  inc     dword [.argc]
  add     dword [.argSize], 9 ; ptr + size + null
  jmp     .SkipSpace

 .DoneCounting:

  ; Allocate memory, and populate the local argv
  push    dword [.argSize]
  call    alloc
  mov     [.argv], eax
  test    eax, eax
  jz      .Exit

  ; Copy arguments
  mov     esi, [.commandLine]
  xor     eax, eax

  mov     al, [esi]
  inc     esi

  mov     ecx, [.argc]
  mov     ebx, [.argv]
  lea     edi, [ebx + ecx * 4]

 .SkipArgSpace:
  test    al, al
  jz      .DoneCopying
  test    byte [eax + chTable], chSpace
  jz      .DoneArgSpace
  mov     al, [esi]
  inc     esi
  jmp     .SkipArgSpace
 .DoneArgSpace:

  mov     edx, edi
  xor     ecx, ecx
  mov     dword [edi], 0
  add     edi, 4
  mov     [ebx], edi
  add     ebx, 4

  cmp     al, '"'
  jne     .NotArgString
  mov     al, [esi]
  inc     esi
 .CopyArgString:
  test    al, al
  jz      .DoneArgString
  cmp     byte [eax + chTable], 0
  je      .InvalidArgument
  cmp     al, '"'
  je      .ClosedArgString
  mov     [edi], al
  inc     edi
  mov     al, [esi]
  inc     esi
  inc     ecx
  jmp     .CopyArgString
 .ClosedArgString:
  mov     al, [esi]
  inc     esi
 .DoneArgString:
  mov     byte [edi], 0
  mov     [edx], ecx
  jmp     .SkipArgSpace
 .NotArgString:

 .CopyArgChar:
  test    al, al
  jz      .DoneArgString
  cmp     byte [eax + chTable], 0
  je      .InvalidArgument
  test    byte [eax + chTable], chSpace
  jnz     .DoneArgChar
  mov     [edi], al
  inc     edi
  mov     al, [esi]
  inc     esi
  inc     ecx
  jmp     .CopyArgChar
 .DoneArgChar:
  mov     byte [edi], 0
  mov     [edx], ecx
  jmp     .SkipArgSpace

 .InvalidArgument:
  lea     eax, [.argv]
  push    eax
  call    free

 .DoneCopying:
  mov     ecx, [.argc]
  mov     ebx, [.argv]
  mov     [argc], ecx
  mov     [argv], ebx

 .Exit:
  pop     edi
  pop     esi
  pop     edx
  pop     ecx
  pop     ebx

  mov     esp, ebp
  pop     ebp
  ret

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

  %define .size ebp + 8

  push    ecx
  push    edx

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
  mov     [eax], ecx
  add     eax, 4

 .Exit:
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func bool realloc( @pointer; usize newSize )
;
realloc:
  push    ebp
  mov     ebp, esp

  %define .pointer ebp + 12
  %define .newSize ebp + 8

  push    ecx
  push    edx
  push    esi

  mov     esi, [.pointer]
  mov     ecx, [.newSize]

  test    esi, esi
  jz      .Exit

  test    ecx, ecx
  jz      .Exit

  sub     esi, 4
  add     ecx, 4

  push    dword LMEM_MOVEABLE
  push    dword ecx
  push    dword esi
  call    [LocalReAlloc]

  test    eax, eax
  jnz     .ResizeSucceeded
  mov     eax, [.pointer]
  jmp     .Exit

 .ResizeSucceeded:
  mov     ecx, [.newSize]
  mov     [eax], ecx
  add     eax, 4

 .Exit:
  pop     esi
  pop     ecx
  pop     edx

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func free( ref pointer )
;
free:
  push    ebp
  mov     ebp, esp

  %define .pointer ebp + 8

  push    ecx
  push    edx
  push    esi
  push    edi

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
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

; ========
; File I/O
; ========

segment .text use32

;
; func file fcreate( string fileName )
;
fcreate:
  push    ebp
  mov     ebp, esp

  %define .fileName ebp + 8

  push    ecx
  push    edx
  push    esi

  xor     eax, eax

  ; File name not allowed to be null
  mov     esi, [.fileName]
  test    esi, esi
  jz      .Exit

  ; File name not allowed to be a length of 0
  mov     ecx, [esi - 4]
  test    ecx, ecx
  jz      .Exit

  ; First character in file name must not be null byte (0)
  cmp     byte [esi], 0
  je      .Exit

  ; HANDLE CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess,
  ;   DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  ;   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
  ;   HANDLE hTemplateFile )
  push    dword 0                            ; hTemplateFile
  push    dword FILE_ATTRIBUTE_NORMAL        ; dwFlagsAndAttributes
  push    dword CREATE_ALWAYS                ; dwCreationDisposition
  push    dword 0                            ; lpSecurityAttributes
  push    dword 0                            ; dwShareMode
  push    dword GENERIC_READ | GENERIC_WRITE ; dwDesiredAccess
  push    esi                                ; lpFileName
  call    [CreateFileA]

  cmp     eax, INVALID_HANDLE_VALUE
  jne     .Exit
  xor     eax, eax

 .Exit:
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func file fupdate( string fileName )
;
fupdate:
  push    ebp
  mov     ebp, esp

  %define .fileName ebp + 8

  push    ecx
  push    edx
  push    esi

  xor     eax, eax

  ; File name not allowed to be null
  mov     esi, [.fileName]
  test    esi, esi
  jz      .Exit

  ; File name not allowed to be a length of 0
  mov     ecx, [esi - 4]
  test    ecx, ecx
  jz      .Exit

  ; First character in file name must not be null byte (0)
  cmp     byte [esi], 0
  je      .Exit

  ; HANDLE CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess,
  ;   DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  ;   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
  ;   HANDLE hTemplateFile )
  push    dword 0                            ; hTemplateFile
  push    dword FILE_ATTRIBUTE_NORMAL        ; dwFlagsAndAttributes
  push    dword OPEN_EXISTING                ; dwCreationDisposition
  push    dword 0                            ; lpSecurityAttributes
  push    dword 0                            ; dwShareMode
  push    dword GENERIC_READ | GENERIC_WRITE ; dwDesiredAccess
  push    esi                                ; lpFileName
  call    [CreateFileA]

  cmp     eax, INVALID_HANDLE_VALUE
  jne     .Exit
  xor     eax, eax

 .Exit:
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func file fopen( string fileName )
;
fopen:
  push    ebp
  mov     ebp, esp

  %define .fileName ebp + 8

  push    ecx
  push    edx
  push    esi

  xor     eax, eax

  ; File name not allowed to be null
  mov     esi, [.fileName]
  test    esi, esi
  jz      .Exit

  ; File name not allowed to be a length of 0
  mov     ecx, [esi - 4]
  test    ecx, ecx
  jz      .Exit

  ; First character in file name must not be null byte (0)
  cmp     byte [esi], 0
  je      .Exit

  ; HANDLE CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess,
  ;   DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  ;   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
  ;   HANDLE hTemplateFile )
  push    dword 0                            ; hTemplateFile
  push    dword FILE_ATTRIBUTE_NORMAL        ; dwFlagsAndAttributes
  push    dword OPEN_EXISTING                ; dwCreationDisposition
  push    dword 0                            ; lpSecurityAttributes
  push    dword 0                            ; dwShareMode
  push    dword GENERIC_READ                 ; dwDesiredAccess
  push    esi                                ; lpFileName
  call    [CreateFileA]

  cmp     eax, INVALID_HANDLE_VALUE
  jne     .Exit
  xor     eax, eax

 .Exit:
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func fclose( @file handle )
;
fclose:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 8

  push    ecx
  push    edx
  push    esi

  mov     esi, [.handle]
  test    esi, esi
  jz      .Exit

  push    dword [esi]
  call    [CloseHandle]

  test    eax, eax
  jz      .Exit
  mov     dword [esi], 0

 .Exit:
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func ufsize fread( file handle; @dest; ufsize size )
;
fread:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 16
  %define .dest   ebp + 12
  %define .size   ebp + 8

  push    dword 0
  %define .result ebp - 4

  push    ecx
  push    edx
  push    esi
  push    edi

  mov     esi, [.handle]
  mov     edi, [.dest]

  xor     eax, eax
  test    esi, esi
  jz      .Exit
  test    edi, edi
  jz      .Exit

  ; BOOL ReadFile( HANDLE hFile, LPVOID lpBuffer,
  ;   DWORD nNumberOfBytesToRead, LPDWORD lpNumberofBytesRead,
  ;   LPOVERLAPPED lpOverlapped )
  push    dword 0          ; lpOverlapped
  lea     eax, [.result]
  push    eax              ; lpNumberofBytesRead
  push    dword [.size]    ; nNumberOfBytesToRead
  push    dword [.dest]    ; lpBuffer
  push    dword [.handle]  ; hFile
  call    [ReadFile]

  test    eax, eax
  jz      .Exit
  mov     eax, [.result]

 .Exit:
  pop     edi
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 12

;
; func bool freadch( file handle; @char dest )
;
freadch:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 12
  %define .dest   ebp + 8

  push    dword 0
  %define .result ebp - 4

  push    ecx
  push    edx
  push    esi
  push    edi

  mov     esi, [.handle]
  mov     edi, [.dest]

  xor     eax, eax
  test    esi, esi
  jz      .Exit
  test    edi, edi
  jz      .Exit

  ; BOOL ReadFile( HANDLE hFile, LPVOID lpBuffer,
  ;   DWORD nNumberOfBytesToRead, LPDWORD lpNumberofBytesRead,
  ;   LPOVERLAPPED lpOverlapped )
  push    dword 0          ; lpOverlapped
  lea     eax, [.result]
  push    eax              ; lpNumberofBytesRead
  push    dword 1          ; nNumberOfBytesToRead
  push    dword [.dest]    ; lpBuffer
  push    dword [.handle]  ; hFile
  call    [ReadFile]

  cmp     eax, 1
  je      .Exit
  xor     eax, eax

 .Exit:
  pop     edi
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func ufsize fwrite( file handle; @source; ufsize size )
;
fwrite:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 16
  %define .source ebp + 12
  %define .size   ebp + 8

  push    dword 0
  %define .result ebp - 4

  push    ecx
  push    edx
  push    esi
  push    edi

  mov     esi, [.handle]
  mov     edi, [.source]

  xor     eax, eax
  test    esi, esi
  jz      .Exit
  test    edi, edi
  jz      .Exit

  ; BOOL WriteFile( HANDLE hFile, LPVOID lpBuffer,
  ;   DWORD nNumberOfBytesToWrite, LPDWORD lpNumberofBytesWritten,
  ;   LPOVERLAPPED lpOverlapped )
  push    dword 0          ; lpOverlapped
  lea     eax, [.result]
  push    eax              ; lpNumberofBytesWritten
  push    dword [.size]    ; nNumberOfBytesToWrite
  push    dword [.source]  ; lpBuffer
  push    dword [.handle]  ; hFile
  call    [WriteFile]

  test    eax, eax
  jz      .Exit
  mov     eax, [.result]

 .Exit:
  pop     edi
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 12

;
; func bool fwritech( file handle; char source )
;
fwritech:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 12
  %define .source ebp + 8

  push    dword 0
  %define .result ebp - 4

  push    ecx
  push    edx
  push    esi
  push    edi

  mov     esi, [.handle]
  mov     edi, [.source]

  xor     eax, eax
  test    esi, esi
  jz      .Exit
  test    edi, edi
  jz      .Exit

  ; BOOL WriteFile( HANDLE hFile, LPVOID lpBuffer,
  ;   DWORD nNumberOfBytesToWrite, LPDWORD lpNumberofBytesWritten,
  ;   LPOVERLAPPED lpOverlapped )
  push    dword 0          ; lpOverlapped
  lea     eax, [.result]
  push    eax              ; lpNumberofBytesWritten
  push    dword 1          ; nNumberOfBytesToWrite
  push    dword [.source]  ; lpBuffer
  push    dword [.handle]  ; hFile
  call    [WriteFile]

  cmp     eax, 1
  je      .Exit
  xor     eax, eax

 .Exit:
  pop     edi
  pop     esi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func bool fseek( file handle; ufsize offset )
;
fseek:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 12
  %define .offset ebp + 8

  push    ecx
  push    edx

  ; DWORD SetFilePointer( HANDLE hFile, LONG lDistanceToMove,
  ;   PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod )
  push    dword FILE_BEGIN ; dwMoveMethod
  push    dword 0          ; lpDistanceToMoveHigh
  push    dword [.offset]  ; lpDistanceToMove
  push    dword [.handle]  ; hFile
  call    [SetFilePointer]

  cmp     eax, INVALID_SET_FILE_POINTER
  jne     .OffsetValid
  xor     eax, eax
  jmp     .Exit
 .OffsetValid:
  mov     eax, 1

 .Exit:
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func bool fpos( file handle; @ufsize pos )
;
fpos:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 12
  %define .pos    ebp + 8

  push    ecx
  push    edx
  push    edi

  xor     eax, eax

  mov     edi, [.pos]
  test    edi, edi
  jz      .Exit

  ; DWORD SetFilePointer( HANDLE hFile, LONG lDistanceToMove,
  ;   PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod )
  push    dword FILE_CURRENT ; dwMoveMethod
  push    dword 0            ; lpDistanceToMoveHigh
  push    dword 0            ; lpDistanceToMove
  push    dword [.handle]    ; hFile
  call    [SetFilePointer]

  mov     dword [edi], eax

  cmp     eax, INVALID_FILE_SIZE
  jne     .PosValid
  xor     eax, eax
  jmp     .Exit
 .PosValid:
  mov     eax, 1

 .Exit:
  pop     edi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 8

;
; func bool fsize( file handle; @ufsize size )
;
fsize:
  push    ebp
  mov     ebp, esp

  %define .handle ebp + 12
  %define .size   ebp + 8

  push    ecx
  push    edx
  push    edi

  xor     eax, eax

  mov     edi, [.size]
  test    edi, edi
  jz      .Exit

  ; DWORD GetFileSize( HANDLE hFile, LPDWORD lpFileSizeHigh )
  push    dword 0         ; lpFileSizeHigh
  push    dword [.handle] ; hFile
  call    [GetFileSize]

  cmp     eax, INVALID_FILE_SIZE
  jne     .SizeValid
  xor     eax, eax
  mov     [edi], eax
  jmp     .Exit
 .SizeValid:
  mov     [edi], eax
  mov     eax, 1

 .Exit:
  pop     edi
  pop     edx
  pop     ecx

  mov     esp, ebp
  pop     ebp
  ret 4

;
; func bool fseteof( file handle )
;
fseteof:
  push    ebp
  mov     ebp, esp

  %define .handle  ebp + 8

  push    ecx
  push    edx

  ; BOOL SetEndOfFile( HANDLE hFile )
  push    dword [.handle]
  call    [SetEndOfFile]

  test    eax, eax
  jz      .Exit
  mov     eax, 1

 .Exit:
  pop     edx
  pop     ecx

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

  push    dword 0
  %define .tempWritten ebp - 4

  push    ecx
  push    edx
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

 .Exit:
  pop     esi
  pop     edx
  pop     ecx

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

  push    dword 0
  push    dword 0
  push    dword 0
  %define .valueStr ebp - 12

  push    dword 0
  %define .tempWritten ebp - 16

  push    ebx
  push    ecx
  push    edx
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
  pop     edx
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

  argc: dd 0
  argv: dd 0

; =======
; Strings
; =======

%endif
