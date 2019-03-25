
  CPU 8086

  BITS 16

segment code use16
..start:
  mov     ax, data
  mov     ds, ax
  mov     dx, errorString
  mov     ah, 0x09
  int     0x21
  mov     ax, 0x4C00
  int     0x21

segment data use16
  errorString: db 'This program requires Windows to run.',13,10,'$'
