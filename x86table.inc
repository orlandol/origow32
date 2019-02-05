%ifndef INC_X86
%define INC_X86

  %include "tokens.inc"
  %include "codegen.inc"

  %define param86MemW 1

  %define encode86Op1   1
  %define encode86Op2   1
  %define encode86Op3   1
  %define encode86ModRM 4

segment .data use32

; ========================
;       Keyword item
; ------------------------
; char[8] keyword
; uint token, encodeOffset
; ========================
  keyword86Table:
    db 'call',0,0,0,0
    dd x86Call, encode86Call

; ========================
;        Token item
; ------------------------
; uint token, encodeOffset
; ========================
  token86Table:
    dd x86Call, encode86Call
    dd x86Push, encode86Push

; ==================================================================
;                            Encode item
; ------------------------------------------------------------------
; uint param1Token, nextParam1
; uint param2Token, nextParam2
; uint param3Token, nextParam3
; uint8 opcode1, opcode2, opcode3, modrm
; uint8 param1Operator, param2Operator, param3Operator, encodeFields
; ------------------------------------------------------------------
;                     if paramXToken == token
;                 then paramXToken is an exact match
;                    else paramXToken is a mask
; ==================================================================
  encode86Table:
    encode86Call:
      dd x86Mem, 0
      dd 0, 0
      dd 0, 0
      db 0b1111_1111, 0, 0, 0b00_010_000
      db param86MemW, 0, 0, encode86Op1 | encode86ModRM

    encode86Push:
      dd valUint, 0, 0

%endif
