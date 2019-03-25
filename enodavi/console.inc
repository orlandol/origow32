%ifndef INC_CONSOLE
%define INC_CONSOLE

  %define CONSOLE_FULLSCREEN          1
  %define CONSOLE_FULLSCREEN_HARDWARE 2

  ;
  ; BOOL WINAPI GetConsoleDisplayMode( LPDWORD lpModeFlags )
  ;
  extern GetConsoleDisplayMode
  import GetConsoleDisplayMode kernel32.dll

  %define CONSOLE_FULLSCREEN_MODE 1
  %define CONSOLE_WINDOWED_MODE   2

  ;
  ; BOOL WINAPI SetConsoleDisplayMode(
  ;   HANDLE hConsoleOutput, DWORD dwFlags,
  ;   PCOORD lpNewScreenBufferDimensions )
  ;
  extern SetConsoleDisplayMode
  import SetConsoleDisplayMode kernel32.dll

  %define COORD_X 0
  %define COORD_Y 2
  %define SIZEOF_COORD 4

  %define CHAR_INFO_ASCIICHAR  0
  %define CHAR_INFO_ATTRIBUTES 2
  %define SIZEOF_CHAR_INFO 4

  %define SMALL_RECT_LEFT   0
  %define SMALL_RECT_TOP    2
  %define SMALL_RECT_RIGHT  4
  %define SMALL_RECT_BOTTOM 6
  %define SIZEOF_SMALL_RECT 8

  ;
  ; BOOL WINAPI SetConsoleCursorPosition(
  ;   HANDLE hConsoleOutput, COORD dwCursorPosition )
  ;
  extern SetConsoleCursorPosition
  import SetConsoleCursorPosition kernel32.dll

  ;
  ; BOOL WINAPI SetConsoleScreenBufferSize(
  ;   HANDLE hConsoleOutput, COORD dwSize )
  ;
  extern SetConsoleScreenBufferSize
  import SetConsoleScreenBufferSize kernel32.dll

  ;
  ; BOOL WINAPI SetConsoleWindowInfo( HANDLE hConsoleOutput,
  ;   BOOL bAbsolute, const SMALL_RECT *lpConsoleWindow )
  ;
  extern SetConsoleWindowInfo
  import SetConsoleWindowInfo kernel32.dll

  %define CONSOLE_FULLSCREEN_MODE 1
  %define CONSOLE_WINDOWED_MODE   2

  ;
  ; BOOL WINAPI SetConsoleDisplayMode( HANDLE hConsoleOutput,
  ;   DWORD dwFlags, PCOORD lpNewScreenBufferDimensions )
  ;
  extern SetConsoleDisplayMode
  import SetConsoleDisplayMode kernel32.dll

  ;
  ; BOOL WINAPI SetConsoleCursorPosition(
  ;   HANDLE hConsoleOutput, COORD dwCursorPosition )
  ;
  extern SetConsoleCursorPosition
  import SetConsoleCursorPosition kernel32.dll

  %define FG_BLACK        0
  %define FG_BLUE         1
  %define FG_GREEN        2
  %define FG_CYAN         3
  %define FG_RED          4
  %define FG_MAGENTA      5
  %define FG_BROWN        6
  %define FG_LIGHTGRAY    7
  %define FG_GRAY         8
  %define FG_LIGHTBLUE    9
  %define FG_LIGHTGREEN   10
  %define FG_LIGHTCYAN    11
  %define FG_LIGHTRED     12
  %define FG_LIGHTMAGENTA 13
  %define FG_YELLOW       14
  %define FG_WHITE        15

  %define BG_BLACK     0
  %define BG_NAVYBLUE  16
  %define BG_GREEN     32
  %define BG_TEAL      48
  %define BG_MAROON    64
  %define BG_PURPLE    80
  %define BG_OLIVE     96
  %define BG_SILVER    112
  %define BG_GRAY      128
  %define BG_BLUE      144
  %define BG_LIME      160
  %define BG_CYAN      176
  %define BG_RED       192
  %define BG_MAGENTA   208
  %define BG_YELLOW    224
  %define BG_WHITE     240

  %define FG_INTENSE   8
  %define BG_INTENSE   128

  ;
  ; BOOL WINAPI SetConsoleTextAttribute(
  ;   HANDLE hConsoleOutput, WORD wAttributes )
  ;
  extern SetConsoleTextAttribute
  import SetConsoleTextAttribute kernel32.dll

  ;
  ; BOOL WINAPI WriteConsoleA( HANDLE  hConsoleOutput,
  ;   const VOID *lpBuffer, DWORD nNumberOfCharsToWrite,
  ;   LPDWORD lpNumberOfCharsWritten, LPVOID lpReserved )
  ;
  extern WriteConsoleA
  import WriteConsoleA kernel32.dll

  ;
  ; BOOL WINAPI WriteConsoleOutputA( HANDLE hConsoleOutput,
  ;   const CHAR_INFO *lpBuffer, COORD dwBufferSize,
  ;   COORD dwBufferCoord, PSMALL_RECT lpWriteRegion )
  ;
  extern WriteConsoleOutputA
  import WriteConsoleOutputA kernel32.dll

  ;
  ; BOOL WINAPI ReadConsoleOutput( HANDLE hConsoleOutput,
  ;   PCHAR_INFO lpBuffer, COORD dwBufferSize,
  ;   COORD dwBufferCoord, PSMALL_RECT lpReadRegion )
  ;
  extern ReadConsoleOutputA
  import ReadConsoleOutputA kernel32.dll

  ;
  ; BOOL WINAPI WriteConsoleOutputA( HANDLE hConsoleOutput,
  ;   const CHAR_INFO *lpBuffer, COORD dwBufferSize,
  ;   COORD dwBufferCoord, PSMALL_RECT lpWriteRegion )
  ;
  extern WriteConsoleOutputA
  import WriteConsoleOutputA kernel32.dll

  ;
  ; BOOL WINAPI SetConsoleCP( UINT wCodePageID )
  ;
  extern SetConsoleCP
  import SetConsoleCP kernel32.dll

  ;
  ; BOOL WINAPI SetConsoleOutputCP( UINT wCodePageID )
  ;
  extern SetConsoleOutputCP
  import SetConsoleOutputCP kernel32.dll

  ;
  ; BOOL WINAPI SetConsoleCtrlHandler(
  ;   PHANDLER_ROUTINE HandlerRoutine, BOOL Add )
  ;
  extern SetConsoleCtrlHandler
  import SetConsoleCtrlHandler kernel32.dll

%endif
