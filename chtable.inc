%ifndef INC_CHTABLE
%define INC_CHTABLE

  ; Character codes
  %define chSpace     (1 << 0)
  %define chPrint     (1 << 1)
  %define chAlpha     (1 << 2)
  %define chBin       (1 << 3)
  %define chOct       (1 << 4)
  %define chHex       (1 << 5)
  %define chDigit     (1 << 6)
  %define chIdent     (1 << 7)

section .data use32

chTable:
  times 9 db 0           ; ASCII 0..9
  db (chSpace | chPrint) ; TAB
  times 4 db chSpace     ; LF VT FF CR
  times 18 db 0          ; ASCII 14..31
  db (chSpace | chPrint) ; SPACE
  times 15 db chPrint    ; ! " # $ % & ' ( ) * + , - . /
  times 2 db (chPrint | chBin | chOct | chHex | chDigit) ; 0 1
  times 6 db (chPrint | chOct | chHex | chDigit) ; 2 3 4 5 6 7
  times 2 db (chPrint | chHex | chDigit) ; 8 9
  times 7 db chPrint     ; : ; < = > ? @
  times 6 db (chPrint | chAlpha | chHex | chIdent) ; A..F
  times 20 db (chPrint | chAlpha | chIdent) ; G..Z
  times 4 db chPrint     ; [ \ ] ^
  db (chPrint | chIdent) ; _
  db chPrint             ; `
  times 6 db (chPrint | chAlpha | chHex | chIdent) ; a..f
  times 20 db (chPrint | chAlpha | chIdent) ; g..z
  times 4 db chPrint     ; { | } ~
  times 128 db 0         ; ASCII 127..255

%endif
