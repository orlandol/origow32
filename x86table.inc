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
; uint param1Token, param2Token, param3Token, nextFormat
; uint8 opcode1, opcode2, opcode3, modrm
; uint encodeFields
; uint8 param1Operator, param2Operator, param3Operator, rsvd
; ------------------------------------------------------------------
;                     if paramXToken == token
;              then paramXToken is compared directly
;         else paramXToken is compared to the token group
; ==================================================================
  encode86Table:
  encode86Call:
    dd valUint, 0, 0, $+16
    db 0b1110_1000, 0, 0, 0
    dd HAS_OPCODE1
    db 0, 0, 0, 0

    dd x86Reg32, 0, 0, $+16
    db 0b1111_1111, 0, 0, 0b11_010_000
    dd HAS_OPCODE1 | HAS_MODRM
    db param86AddMod321, 0, 0, 0

    dd x86Mem, 0, 0, 0
    db 0b1111_1111, 0, 0, 0b00_010_000
    dd HAS_OPCODE1 | HAS_MODRM
    db 0, 0, 0, 0

  encode86Push:
    dd x86Reg16, 0, 0, $+16
    db 0b1111_1111, 0, 0, 0b11_110_000
    db HAS_OPCODE1 | HAS_MODRM
    db param86AddMod321, 0, 0, 0

    dd x86Reg32, 0, 0, $+16
    db 0b1111_1111, 0, 0, 0b11_110_000
    db HAS_OPCODE1 | HAS_MODRM
    db param86AddMod321, 0, 0, 0

    dd valUint, 0, 0, $+16
    db 0b0110_1000, 0, 0, 0
    db HAS_OPCODE1
    db param86OpcodeS8, 0, 0, 0


%endif
