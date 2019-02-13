%ifndef INC_X86
%define INC_X86

  %include "tokens.inc"
  %include "codegen.inc"

  %define param86MemW   1
  %define param86RegRMW 2
  %define param86UintW  3

  %define encode86Op1   1
  %define encode86Op2   2
  %define encode86Op3   4
  %define encode86ModRM 8

segment .data use32

; ====================================
;            Keyword item
; ------------------------------------
; char[8] keyword
; ------------------------------------
; Resulting offset can be recalculated
;   to point into token86Table
; ====================================
  keyword86Table:
    db 'call',0,0,0,0
    db 'push',0,0,0,0

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
; uint8 encodeFields, param1Operator, param2Operator, param3Operator
; ------------------------------------------------------------------
;                     if paramXToken == token
;              then paramXToken is compared directly
;         else paramXToken is compared to the token group
; ==================================================================
  encode86Table:
    encode86Call:
      dd x86Mem, 24
      dd 0, 0
      dd 0, 0
      db 0b1111_1111, 0, 0, 0b00_010_000
      db encode86Op1 | encode86ModRM, param86MemW, 0, 0

      dd x86Reg16, 24
      dd 0, 0
      dd 0, 0
      db 0b1111_1111, 0, 0, 0b11_010_000
      db encode86Op1 | encode86ModRM, param86RegRMW, 0, 0

      dd x86Reg32, 0
      dd 0, 0
      dd 0, 0
      db 0b1111_1111, 0, 0, 0b11_010_000
      db encode86Op1 | encode86ModRM, param86RegRMW, 0, 0

    encode86Push:
      dd valUint, 0, 0
      dd 0, 0, 0
      dd 0, 0, 0
      db 0b0110_1000, 0, 0, 0
      db encode86Op1, param86UintW, 0, 0

%endif
