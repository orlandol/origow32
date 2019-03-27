
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"
  %include "console.inc"

extern GetLastError
import GetLastError kernel32.dll

run:
..start:
  push    ebp
  mov     ebp, esp

  %define displayMode ebp - 4
  push    dword 0

  %define bufDims ebp - (4 + SIZEOF_COORD)
  push    word 0
  push    word 0

  %define winDims ebp - (4 + SIZEOF_COORD + SIZEOF_SMALL_RECT)
  push    dword 0
  push    dword 0

  %define cursorPos ebp - (8 + SIZEOF_COORD + SIZEOF_SMALL_RECT)
  push    dword 0

  call    InitRTL

  push    437
  call    [SetConsoleCP]

  push    437
  call    [SetConsoleOutputCP]

  mov     word [winDims + SMALL_RECT_LEFT], 0
  mov     word [winDims + SMALL_RECT_TOP], 0
  mov     word [winDims + SMALL_RECT_RIGHT], 79
  mov     word [winDims + SMALL_RECT_BOTTOM], 24

  ; BOOL WINAPI SetConsoleWindowInfo( HANDLE hConsoleOutput,
  ;   BOOL bAbsolute, const SMALL_RECT *lpConsoleWindow )
  lea     eax, [winDims]
  push    eax
  push    dword TRUE
  push    dword [stdOut]
  call    [SetConsoleWindowInfo]

  mov     word [bufDims + COORD_X], 80
  mov     word [bufDims + COORD_Y], 25

  ; BOOL WINAPI SetConsoleScreenBufferSize(
  ;   HANDLE hConsoleOutput, COORD dwSize )
  push    dword [bufDims]
  push    dword [stdOut]
  call    [SetConsoleScreenBufferSize]

  mov     word [cursorPos + COORD_X], 13
  mov     word [cursorPos + COORD_Y], 2

  ; BOOL WINAPI SetConsoleCursorPosition(
  ;   HANDLE hConsoleOutput, COORD dwCursorPosition )
  push    dword [cursorPos]
  push    dword [stdOut]
  call    [SetConsoleCursorPosition]

  ; BOOL WINAPI WriteConsoleOutputA( HANDLE hConsoleOutput,
  ;   const CHAR_INFO *lpBuffer, COORD dwBufferSize,
  ;   COORD dwBufferCoord, PSMALL_RECT lpWriteRegion )
  lea     eax, [winDims]
  push    eax
  push    dword [winDims]
  push    dword [bufDims]
  push    viewBuf
  push    dword [stdOut]
  call    [WriteConsoleOutputA]

 .Exit:
  jmp .Exit
  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

  viewBuf:
    ; StatusBar mockup - begin
    dw 255, (FG_GRAY | BG_TEAL)
    dw 'O', (FG_LIGHTGRAY | BG_TEAL), 'r', (FG_LIGHTGRAY | BG_TEAL)
    dw 'i', (FG_LIGHTGRAY | BG_TEAL), 'g', (FG_LIGHTGRAY | BG_TEAL)
    dw 'o', (FG_LIGHTGRAY | BG_TEAL), 'W', (FG_LIGHTGRAY | BG_TEAL)
    dw '3', (FG_LIGHTGRAY | BG_TEAL), '2', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), '|', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), 'L', (FG_LIGHTGRAY | BG_TEAL)
    dw '1', (FG_LIGHTGRAY | BG_TEAL), 255, (FG_LIGHTGRAY | BG_TEAL)
    dw 'C', (FG_LIGHTGRAY | BG_TEAL), '1', (FG_LIGHTGRAY | BG_TEAL)
    dw '4', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), '|', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), 'C', (FG_BLACK | BG_TEAL)
    dw 'S', (FG_BLACK | BG_TEAL), 'N', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), '|', (FG_LIGHTGRAY | BG_TEAL)
    times 43 dw 255, (FG_LIGHTGRAY | BG_TEAL)
    dw '|', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), 255, (FG_LIGHTGRAY | BG_TEAL)
    dw '3', (FG_LIGHTGRAY | BG_TEAL), ':', (FG_LIGHTGRAY | BG_TEAL)
    dw '1', (FG_LIGHTGRAY | BG_TEAL), '4', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL), 'P', (FG_LIGHTGRAY | BG_TEAL)
    dw 'M', (FG_LIGHTGRAY | BG_TEAL)
    dw 255, (FG_LIGHTGRAY | BG_TEAL)
    ; StatusBar mockup - end

    ; EditView ControlBar mockup - begin
    times 33 dw 205, (FG_LIGHTGRAY | BG_BLUE)
    dw '[', (FG_LIGHTGRAY | BG_BLUE), 255, (FG_LIGHTGRAY | BG_BLUE)
    dw '*', (FG_LIGHTGRAY | BG_BLUE), 'o', (FG_LIGHTGRAY | BG_BLUE)
    dw 'r', (FG_LIGHTGRAY | BG_BLUE), 'i', (FG_LIGHTGRAY | BG_BLUE)
    dw 'g', (FG_LIGHTGRAY | BG_BLUE), 'o', (FG_LIGHTGRAY | BG_BLUE)
    dw '.', (FG_LIGHTGRAY | BG_BLUE), 'r', (FG_LIGHTGRAY | BG_BLUE)
    dw 'e', (FG_LIGHTGRAY | BG_BLUE), 't', (FG_LIGHTGRAY | BG_BLUE)
    dw 255, (FG_LIGHTGRAY | BG_BLUE), ']', (FG_LIGHTGRAY | BG_BLUE)
    times 29 dw 205, (FG_LIGHTGRAY | BG_BLUE)
    dw 255, (FG_LIGHTGRAY | BG_BLUE), 254, (FG_CYAN | BG_BLUE)
    dw 255, (FG_LIGHTGRAY | BG_BLUE), 187, (FG_LIGHTGRAY | BG_BLUE)
    ; EditView ControlBar mockup - end

    ; EditView syntax highlighting mockup - begin
    dw 'p', (FG_YELLOW | BG_BLUE), 'r', (FG_YELLOW | BG_BLUE)
    dw 'o', (FG_YELLOW | BG_BLUE), 'g', (FG_YELLOW | BG_BLUE)
    dw 'r', (FG_YELLOW | BG_BLUE), 'a', (FG_YELLOW | BG_BLUE)
    dw 'm', (FG_YELLOW | BG_BLUE), 255, (FG_BLUE | BG_BLUE)
    dw 'O', (FG_WHITE | BG_BLUE), 'r', (FG_WHITE | BG_BLUE)
    dw 'i', (FG_WHITE | BG_BLUE), 'g', (FG_WHITE | BG_BLUE)
    dw 'o', (FG_WHITE | BG_BLUE)
    times 66 dw 255, (FG_BLUE | BG_BLUE)
    dw 255, (FG_BLUE | BG_GRAY)
    times 79 dw 255, (FG_BLUE | BG_BLUE)
    dw 176, (FG_LIGHTGRAY | BG_BLUE)
   %rep    20
    times 79 dw 255, (FG_BLUE | BG_BLUE)
    dw 176, (FG_LIGHTGRAY | BG_BLUE)
   %endrep
    ; EditView syntax highlighting mockup - end

    ; EditView horizontal ScrollBar mockup - begin
    dw 255, (FG_BLUE | BG_GRAY)
    times 78 dw 176, (FG_LIGHTGRAY | BG_BLUE)
    dw 178, (FG_LIGHTGRAY | BG_BLUE)
    ; EditView horizontal ScrollBar mockup - end

section .bss use32
