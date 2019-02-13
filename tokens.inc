
; ===============
; Character codes
; ===============
;
;  %define chSpace     (1 << 0)
;  %define chPrint     (1 << 1)
;  %define chAlpha     (1 << 2)
;  %define chBin       (1 << 3)
;  %define chOct       (1 << 4)
;  %define chHex       (1 << 5)
;  %define chDigit     (1 << 6)
;  %define chIdent     (1 << 7)

; ===========
; Token masks
; ===========

  %define maskGroup    0xFE00
  %define maskSubtoken 0x01FF

; ============
; Token groups
; ============

  %define tkGeneral  ( 0 << 9)
  %define rsvdIdent  ( 1 << 9)
  %define valLiteral ( 2 << 9)
  %define operSymbol ( 3 << 9)
  %define setSymbol  ( 4 << 9)
  %define x86General ( 5 << 9)
  %define x86Ident   ( 6 << 9)
  %define x86SReg    ( 7 << 9)
  %define x86Reg8    ( 8 << 9)
  %define x86Reg16   ( 9 << 9)
  %define x86Reg32   (10 << 9)

; =========================
; Operator precedence masks
; =========================

  %define maskPrec    0x01E0
  %define maskOper    0x001F

; ==========================
; Operator precedence groups
; ==========================

  %define prec00      (0 << 5)
  %define prec01      (1 << 5)
  %define prec02      (2 << 5)
  %define prec03      (3 << 5)
  %define prec04      (4 << 5)
  %define prec05      (5 << 5)
  %define prec06      (6 << 5)
  %define prec07      (7 << 5)
  %define prec08      (8 << 5)
  %define prec09      (9 << 5)
  %define prec10     (10 << 5)
  %define prec11     (11 << 5)
  %define prec12     (12 << 5)
  %define prec13     (13 << 5)
  %define prec14     (14 << 5)
  %define prec15     (15 << 5)

; ==============
; General tokens
; ==============

  %define tkEOF          (tkGeneral + 1)
  %define tkEOL          (tkGeneral + 2)
  %define tkIdent        (tkGeneral + 3)
  %define tkDot          (tkGeneral + 4)
  %define tkDotDot       (tkGeneral + 5)
  %define tkLParen       (tkGeneral + 6)
  %define tkRParen       (tkGeneral + 7)
  %define tkLBrace       (tkGeneral + 8)
  %define tkRBrace       (tkGeneral + 9)
  %define tkLDoubleBrace (tkGeneral + 10)
  %define tkRDoubleBrace (tkGeneral + 11)
  %define tkComma        (tkGeneral + 12)
  %define tkColon        (tkGeneral + 13)
  %define tkSemicolon    (tkGeneral + 14)
  %define tkAt           (tkGeneral + 15)

; ==================================
; Reserved high level keyword tokens
; ==================================

  %define rsvdProgram    (rsvdIdent + 1)
  %define rsvdEnum       (rsvdIdent + 2)
  %define rsvdStruct     (rsvdIdent + 3)
  %define rsvdObject     (rsvdIdent + 4)
  %define rsvdPublic     (rsvdIdent + 5)
  %define rsvdVisible    (rsvdIdent + 6)
  %define rsvdInternal   (rsvdIdent + 7)
  %define rsvdExtends    (rsvdIdent + 8)
  %define rsvdPrivate    (rsvdIdent + 9)
  %define rsvdInterface  (rsvdIdent + 10)
  %define rsvdImplements (rsvdIdent + 11)
  %define rsvdMethod     (rsvdIdent + 12)
  %define rsvdSelf       (rsvdIdent + 13)
  %define rsvdFunc       (rsvdIdent + 14)
  %define rsvdAsm        (rsvdIdent + 15)
  %define rsvdNoRet      (rsvdIdent + 16)
  %define rsvdNoFrame    (rsvdIdent + 17)
  %define rsvdReturn     (rsvdIdent + 18)
  %define rsvdVar        (rsvdIdent + 19)
  %define rsvdRun        (rsvdIdent + 20)
  %define rsvdEnd        (rsvdIdent + 21)
  %define rsvdIf         (rsvdIdent + 22)
  %define rsvdThen       (rsvdIdent + 23)
  %define rsvdThenIf     (rsvdIdent + 24)
  %define rsvdElseIf     (rsvdIdent + 25)
  %define rsvdElse       (rsvdIdent + 26)
  %define rsvdEndIf      (rsvdIdent + 27)
  %define rsvdWhile      (rsvdIdent + 28)
  %define rsvdNext       (rsvdIdent + 29)
  %define rsvdBreak      (rsvdIdent + 30)
  %define rsvdEndWhile   (rsvdIdent + 31)
  %define rsvdIn         (rsvdIdent + 32)
  %define rsvdExit       (rsvdIdent + 33)

; ====================
; Literal value tokens
; ====================

  %define valFalse  (valLiteral + 0)
  %define valTrue   (valLiteral + 1)
  %define valInt    (valLiteral + 2)
  %define valInt8   (valLiteral + 3)
  %define valUint   (valLiteral + 4)
  %define valUint8  (valLiteral + 5)
  %define valString (valLiteral + 6)
  %define valChar   (valLiteral + 7)

; ===============
; Operator tokens
; ===============

  %define opPostInc  (operSymbol + prec01 + 0)
  %define opPostDec  (operSymbol + prec01 + 1)
  %define opPreInc   (operSymbol + prec02 + 0)
  %define opPreDec   (operSymbol + prec02 + 1)
  %define unaryNeg   (operSymbol + prec02 + 2)
  %define unaryIsNot (operSymbol + prec02 + 3)
  %define unaryNot   (operSymbol + prec02 + 4)
  %define opMul      (operSymbol + prec04 + 0)
  %define opDiv      (operSymbol + prec04 + 1)
  %define opMod      (operSymbol + prec04 + 2)
  %define opAdd      (operSymbol + prec05 + 0)
  %define opSub      (operSymbol + prec05 + 1)
  %define opShl      (operSymbol + prec06 + 0)
  %define opShr      (operSymbol + prec06 + 1)
  %define opSShr     (operSymbol + prec06 + 2)
  %define opRol      (operSymbol + prec06 + 3)
  %define opSRol     (operSymbol + prec06 + 4)
  %define opRor      (operSymbol + prec06 + 5)
  %define opSRor     (operSymbol + prec06 + 6)
  %define opLT       (operSymbol + prec08 + 0)
  %define opLTEq     (operSymbol + prec08 + 1)
  %define opGT       (operSymbol + prec08 + 2)
  %define opGTEq     (operSymbol + prec08 + 3)
  %define opEq       (operSymbol + prec09 + 0)
  %define opNotEq    (operSymbol + prec09 + 1)
  %define opAnd      (operSymbol + prec10 + 0)
  %define opXor      (operSymbol + prec11 + 0)
  %define opOr       (operSymbol + prec12 + 0)
  %define opAndIs    (operSymbol + prec13 + 0)
  %define opOrIs     (operSymbol + prec14 + 0)

; ====================
; Assignment operators
; ====================

  %define setTo      (setSymbol + 0)
  %define setNot     (setSymbol + 1)
  %define setAdd     (setSymbol + 2)
  %define setSub     (setSymbol + 3)
  %define setMul     (setSymbol + 4)
  %define setDiv     (setSymbol + 5)
  %define setMod     (setSymbol + 6)
  %define setShl     (setSymbol + 7)
  %define setShr     (setSymbol + 8)
  %define setSShr    (setSymbol + 9)
  %define setRol     (setSymbol + 10)
  %define setSRol    (setSymbol + 11)
  %define setRor     (setSymbol + 12)
  %define setSRor    (setSymbol + 13)
  %define setAnd     (setSymbol + 14)
  %define setXor     (setSymbol + 15)
  %define setOr      (setSymbol + 16)

; =====================
; Memory register flags
; =====================

  ; 16-bit memory flags
  %define mrX86RegBX  8
  %define mrX86RegBP  4
  %define mrX86RegSI  2
  %define mrX86RegDI  1

; ===================================
; x86 Instruction format byte offsets
; ===================================

  %define x86ifInstPrefix     0
  %define x86ifAddrSizePrefix 1
  %define x86ifOperSizePrefix 2
  %define x86ifSegOverride    3
  %define x86ifOpcode1        4
  %define x86ifOpcode2        5
  %define x86ifModRM          6
  %define x86ifSIB            7
  %define x86ifDisp0          8
  %define x86ifDisp1          9
  %define x86ifDisp2          10
  %define x86ifDisp3          11
  %define x86ifImm0           12
  %define x86ifImm1           13
  %define x86ifImm2           14
  %define x86ifImm3           15

; ==================
; x86 general tokens
; ==================

  %define x86Mem (x86General + 1)

; ==================================
; x86 assembler reserved word tokens
; ==================================

  ; Begin temporary definitions
  %define x86Call   (x86Ident + 0)
  %define x86Push   (x86Ident + 1)
  ; End temporary definitions

  %define x86Cbw    (x86Ident +  0)
  %define x86Cwd    (x86Ident +  1)
  %define x86Clc    (x86Ident +  2)
  %define x86Cld    (x86Ident +  3)
  %define x86Cli    (x86Ident +  4)
  %define x86Cmc    (x86Ident +  5)
  %define x86Int3   (x86Ident +  6)
  %define x86Iret   (x86Ident +  7)
  %define x86Lahf   (x86Ident +  8)
  %define x86Nop    (x86Ident +  9)
  %define x86Popf   (x86Ident + 10)
  %define x86Pushf  (x86Ident + 11)
  %define x86Ret    (x86Ident + 12)
  %define x86Retf   (x86Ident + 13)
  %define x86Sahf   (x86Ident + 14)
  %define x86Stc    (x86Ident + 15)
  %define x86Std    (x86Ident + 16)
  %define x86Sti    (x86Ident + 17)
  %define x86Xlat   (x86Ident + 18)
  %define x86Xlatb  (x86Ident + 19)

  %define x86Adc    (x86Ident + 20)
  %define x86Add    (x86Ident + 21)
  %define x86And    (x86Ident + 22)
;  %define x86Call   (x86Ident + 23)
  %define x86Cmp    (x86Ident + 24)
  %define x86Dec    (x86Ident + 25)
  %define x86Div    (x86Ident + 26)
  %define x86IDiv   (x86Ident + 27)
  %define x86IMul   (x86Ident + 28)
  %define x86In     (x86Ident + 29)
  %define x86Inc    (x86Ident + 30)
  %define x86Int    (x86Ident + 31)

 %define x86JCC    (x86Ident + 32)
  %define x86Jo     (x86Ident + 32)
  %define x86Jno    (x86Ident + 33)
  %define x86Jb     (x86Ident + 34)
  %define x86Jc     x86Jb
  %define x86Jnae   x86Jb
  %define x86Jnb    (x86Ident + 35)
  %define x86Jnc    x86Jnb
  %define x86Jae    x86Jnb
  %define x86Je     (x86Ident + 36)
  %define x86Jz     x86Je
  %define x86Jne    (x86Ident + 37)
  %define x86Jnz    x86Jne
  %define x86Jbe    (x86Ident + 38)
  %define x86Jna    x86Jbe
  %define x86Jnbe   (x86Ident + 39)
  %define x86Ja     x86Jnbe
  %define x86Js     (x86Ident + 40)
  %define x86Jns    (x86Ident + 41)
  %define x86Jp     (x86Ident + 42)
  %define x86Jpe    x86Jp
  %define x86Jnp    (x86Ident + 43)
  %define x86Jpo    x86Jnp
  %define x86Jl     (x86Ident + 44)
  %define x86Jnge   x86Jl
  %define x86Jnl    (x86Ident + 45)
  %define x86Jge    x86Jnl
  %define x86Jle    (x86Ident + 46)
  %define x86Jng    x86Jle
  %define x86Jnle   (x86Ident + 47)
  %define x86Jg     x86Jnle

  %define x86Rep    (x86Ident + 48)
  %define x86Repe   (x86Ident + 49)
  %define x86Repne  (x86Ident + 50)
  %define x86Repz   (x86Ident + 51)
  %define x86Repnz  (x86Ident + 52)

  %define x86Lodsb  (x86Ident + 53)
  %define x86Lodsw  (x86Ident + 54)
  %define x86Movsb  (x86Ident + 55)
  %define x86Movsw  (x86Ident + 56)
  %define x86Stosb  (x86Ident + 57)
  %define x86Stosw  (x86Ident + 58)
  %define x86Cmpsb  (x86Ident + 59)
  %define x86Cmpsw  (x86Ident + 60)
  %define x86Scasb  (x86Ident + 61)
  %define x86Scasw  (x86Ident + 62)

  %define x86Jcxz   (x86Ident + 63)
  %define x86Jmp    (x86Ident + 64)
  %define x86Lds    (x86Ident + 65)
  %define x86Les    (x86Ident + 66)
  %define x86Lea    (x86Ident + 67)
  %define x86Loop   (x86Ident + 68)
  %define x86Loope  (x86Ident + 69)
  %define x86Loopne (x86Ident + 70)
  %define x86Loopz  (x86Ident + 71)
  %define x86Loopnz (x86Ident + 72)
  %define x86Mov    (x86Ident + 73)
  %define x86Mul    (x86Ident + 74)
  %define x86Neg    (x86Ident + 75)
  %define x86Not    (x86Ident + 76)
  %define x86Or     (x86Ident + 77)
  %define x86Out    (x86Ident + 78)
  %define x86Pop    (x86Ident + 79)

;  %define x86Push   (x86Ident + 80)
  %define x86Rcl    (x86Ident + 81)
  %define x86Rcr    (x86Ident + 82)
  %define x86Rol    (x86Ident + 83)
  %define x86Ror    (x86Ident + 84)
  %define x86Sal    (x86Ident + 85)
  %define x86Sar    (x86Ident + 86)
  %define x86Shl    (x86Ident + 87)
  %define x86Shr    (x86Ident + 88)
  %define x86Sbb    (x86Ident + 89)
  %define x86Sub    (x86Ident + 90)
  %define x86Test   (x86Ident + 91)
  %define x86Xchg   (x86Ident + 92)
  %define x86Xor    (x86Ident + 93)

; ====================================
; x86 segment/selector register tokens
; ====================================

  %define x86SRegES (x86SReg + 0)
  %define x86SRegCS (x86SReg + 1)
  %define x86SRegSS (x86SReg + 2)
  %define x86SRegDS (x86SReg + 3)

; =========================
; x86 8-bit register tokens
; =========================

  %define x86RegAL (x86Reg8 + 0)
  %define x86RegCL (x86Reg8 + 1)
  %define x86RegDL (x86Reg8 + 2)
  %define x86RegBL (x86Reg8 + 3)
  %define x86RegAH (x86Reg8 + 4)
  %define x86RegCH (x86Reg8 + 5)
  %define x86RegDH (x86Reg8 + 6)
  %define x86RegBH (x86Reg8 + 7)

; ==========================
; x86 16-bit register tokens
; ==========================

  %define x86RegAX (x86Reg16 + 0)
  %define x86RegCX (x86Reg16 + 1)
  %define x86RegDX (x86Reg16 + 2)
  %define x86RegBX (x86Reg16 + 3)
  %define x86RegSP (x86Reg16 + 4)
  %define x86RegBP (x86Reg16 + 5)
  %define x86RegSI (x86Reg16 + 6)
  %define x86RegDI (x86Reg16 + 7)

; ==========================
; x86 32-bit register tokens
; ==========================

  %define x86RegEAX (x86Reg32 + 0)
  %define x86RegECX (x86Reg32 + 1)
  %define x86RegEDX (x86Reg32 + 2)
  %define x86RegEBX (x86Reg32 + 3)
  %define x86RegESP (x86Reg32 + 4)
  %define x86RegEBP (x86Reg32 + 5)
  %define x86RegESI (x86Reg32 + 6)
  %define x86RegEDI (x86Reg32 + 7)
