%ifndef INC_CODEGEN
%define INC_CODEGEN

  %include "tokens.inc"

; ====================================
;            Keyword item
; ------------------------------------
; char[8] keyword
; ------------------------------------
; Resulting offset can be recalculated
;   to point into token86Table
; ====================================
  %define SIZEOF_KEYWORD86_ENTRY 8

; ========================
;        Token item
; ------------------------
; uint op, encode
; ========================
  %define TOKEN86_OP     0
  %define TOKEN86_ENCODE 4

  %define SIZEOF_TOKEN86_ITEM 8

; ==================================================================
;                            Encode item
; ------------------------------------------------------------------
; uint   operand1, operand2, operand3, nextFormat
; uint8  opcode1, opcode2, opcode3, modrm
; uint   emitFields
; uint8  xform1, xform2, xform3, xformOp
; uint32 rsvd
; ------------------------------------------------------------------
;                     if operandX == token
;              then operandX is compared directly
;         else operandX is compared to the token group
; ==================================================================
  %define ENCODE86_OPERAND1   0
  %define ENCODE86_OPERAND2   4
  %define ENCODE86_OPERAND3   8
  %define ENCODE86_NEXTFORMAT 12
  %define ENCODE86_OPCODE1    16
  %define ENCODE86_OPCODE2    17
  %define ENCODE86_OPCODE3    18
  %define ENCODE86_MODRM      19
  %define ENCODE86_EMITFIELDS 20
  %define ENCODE86_XFORM1     24
  %define ENCODE86_XFORM1     25
  %define ENCODE86_XFORM1     26
  %define ENCODE86_XFORM1     27
  %define ENCODE86_RSVD       28

  %define SIZEOF_ENCODE86_ITEM 32

; ==================
; Instruction Buffer
; ==================

  %define INSTR_BUFFER_SIZE 32

  %define INSTR_GROUP1_PREFIX 0
  %define INSTR_GROUP2_PREFIX 1
  %define INSTR_GROUP3_PREFIX 2
  %define INSTR_GROUP4_PREFIX 3
  %define INSTR_OPCODE1       4
  %define INSTR_OPCODE2       5
  %define INSTR_OPCODE3       6
  %define INSTR_MODRM         7
  %define INSTR_SIB           8
  %define INSTR_DISP1         9
  %define INSTR_DISP2         10
  %define INSTR_DISP3         11
  %define INSTR_DISP4         12
  %define INSTR_IMM1          13
  %define INSTR_IMM2          14
  %define INSTR_IMM3          15
  %define INSTR_IMM4          16

  %define HAS_GROUP1_PREFIX (1 << (31 - INSTR_GROUP1_PREFIX))
  %define HAS_GROUP2_PREFIX (1 << (31 - INSTR_GROUP2_PREFIX))
  %define HAS_GROUP3_PREFIX (1 << (31 - INSTR_GROUP3_PREFIX))
  %define HAS_GROUP4_PREFIX (1 << (31 - INSTR_GROUP4_PREFIX))
  %define HAS_OPCODE1       (1 << (31 - INSTR_OPCODE1))
  %define HAS_OPCODE2       (1 << (31 - INSTR_OPCODE2))
  %define HAS_OPCODE3       (1 << (31 - INSTR_OPCODE3))
  %define HAS_MODRM         (1 << (31 - INSTR_MODRM))
  %define HAS_SIB           (1 << (31 - INSTR_SIB))
  %define HAS_DISP1         (1 << (31 - INSTR_DISP1))
  %define HAS_DISP2         (1 << (31 - INSTR_DISP2))
  %define HAS_DISP3         (1 << (31 - INSTR_DISP3))
  %define HAS_DISP4         (1 << (31 - INSTR_DISP4))
  %define HAS_IMM1          (1 << (31 - INSTR_IMM1))
  %define HAS_IMM2          (1 << (31 - INSTR_IMM2))
  %define HAS_IMM3          (1 << (31 - INSTR_IMM3))
  %define HAS_IMM4          (1 << (31 - INSTR_IMM4))

; =======
; Emitter
; =======

segment .text use32

;
; func bool emit( file outHandle, InstructionBuffer instruction, uint flags )
;
emit:
  push    ebp
  mov     ebp, esp

  %define .outHandle   ebp + 16
  %define .instruction ebp + 12
  %define .flags       ebp + 8

  sub     esp, INSTR_BUFFER_SIZE
  %define .instrBuffer ebp - INSTR_BUFFER_SIZE

  push    ebx
  push    ecx
  push    edx
  push    esi
  push    edi

  xor     eax, eax

  mov     esi, [.instruction]
  mov     edx, [.flags]

  test    esi, esi
  jz      .Exit
  test    edx, edx
  jz      .Exit

  xor     ecx, ecx
  lea     edi, [.instrBuffer]

 .CopyLoop:
  test    edx, edx
  jz      .DoneCopying
  mov     al, [esi]
  inc     esi
  add     edx, edx
  jnc     .SkipWrite
  mov     [edi + ecx], al
  inc     ecx
 .SkipWrite:
  jmp     .CopyLoop
 .DoneCopying:

  push    dword [.outHandle]
  push    edi
  push    ecx
  call    fwrite

  cmp     eax, ecx
  je      .Succeeded
  xor     eax, eax
  jmp     .Exit
 .Succeeded:
  mov     eax, 1

 .Exit:
  pop     edi
  pop     esi
  pop     edx
  pop     ecx
  pop     ebx

  mov     esp, ebp
  pop     ebp

  ret 12

; ===============
; Code generators
; ===============

  %define addrWPrefix 0x67
  %define addrWField HAS_GROUP4_PREFIX

segment .text use32

;
; func bool x86GenOpMem( file handle, uint op, segReg, baseReg,
;   indexReg, scale, int displacement, uint memSize )
;
x86GenOpMem:
  push    ebp
  mov     ebp, esp

  %define .handle       ebp + 36
  %define .op           ebp + 32
  %define .segReg       ebp + 28
  %define .baseReg      ebp + 24
  %define .indexReg     ebp + 20
  %define .scale        ebp + 16
  %define .displacement ebp + 12
  %define .memSize      ebp + 8

  %define .result ebp - 4
  push    dword 0

  %define .addrPrefix ebp - 8
  push    dword 0

  %define .addrField ebp - 12
  push    dword 0

  %define .emitFields ebp - 16
  push    dword 0

  %define .instruction ebp - (16 - INSTR_BUFFER_SIZE)
  times (INSTR_BUFFER_SIZE / 4) push dword 0

  push    ebx
  push    ecx
  push    edx
  push    esi
  push    edi

  mov     ebx, [.op]
  cmp     ebx, lastX86Op
  ja      .Exit
  and     ebx, maskSubtoken
  mov     esi, [ebx * 8 + token86Table + TOKEN86_ENCODE]
  test    esi, esi
  jz      .Exit

 .SearchInstruction:
  cmp     dword [esi + ENCODE86_OPERAND1], x86Mem
  jne     .NextInstruction
  cmp     dword [esi + ENCODE86_OPERAND2], 0
  jne     .NextInstruction
  jmp     .FoundInstruction
 .NextInstruction:
  mov     esi, [esi + ENCODE86_NEXTFORMAT]
  test    esi, esi
  jz      .Exit
  cmp     esi, encode86TableEnd
  jae     .Exit
  jmp     .SearchInstruction
 .FoundInstruction:

  mov     eax, [esi + ENCODE86_OPCODE1]
  mov     [.instruction + INSTR_OPCODE1], eax
  mov     eax, [esi + ENCODE86_EMITFIELDS]
  mov     [.emitFields], eax

  mov     eax, [.baseReg]
  and     eax, maskGroup

  cmp     eax, x86Reg32
  jne     .NotReg32
  lea     eax, [.instruction]
  push    eax
  lea     eax, [.emitFields]
  push    eax
  push    dword [.segReg]
  push    dword [.baseReg]
  push    dword [.indexReg]
  push    dword [.scale]
  push    dword [.displacement]
  call    x86EncodeAddr32
  test    eax, eax
  jz      .Exit
  jmp     .TransformInstruction
 .NotReg32:

  cmp     eax, x86Reg16
  jne     .NotReg16
  cmp     dword [.scale], 0
  jne     .Exit
  lea     eax, [.instruction]
  push    eax
  lea     eax, [.emitFields]
  push    eax
  push    dword [.segReg]
  push    dword [.baseReg]
  push    dword [.indexReg]
  push    dword [.displacement]
  call    x86EncodeAddr16
  test    eax, eax
  jz      .Exit
  jmp     .TransformInstruction
 .NotReg16:

 .EncodeInstruction:

 .TransformInstruction:

 .EmitInstruction:
  push    dword [.handle]
  lea     eax, [.instruction]
  push    eax
  push    dword [.emitFields]
  call    emit
  mov     [.result], eax

 .Exit:
  mov     eax, [.result]

  pop     edi
  pop     esi
  pop     edx
  pop     ecx
  pop     ebx

  mov     esp, ebp
  pop     ebp
  ret 32

;
; func bool x86EncodeAddr32( Instruction instruction, uint emitFields,
;   segReg, baseReg, indexReg, scale, int displacement )
;
x86EncodeAddr32:
  ; 1) If not (baseReg | indexReg), modify modRM as standalone
  ;       displacement, then go transform operand
  ; 2) Init SIB with no index, and no base
  ; 3) If baseReg, modify modRM
  ; 4) If baseReg is ESP, or indexReg, modify SIB and modRM
  ; 5) If indexReg is ESP, return false
  ; 6) If indexReg, set scale
  ; 7) If displacement, or baseReg is EBP, modify modRM
  ; 8) Transform operand(?)
  ; 9) Encode operand

  push    ebp
  mov     ebp, esp

  %define .instruction  ebp + 32
  %define .emitFields   ebp + 28
  %define .segReg       ebp + 24
  %define .baseReg      ebp + 20
  %define .indexReg     ebp + 16
  %define .scale        ebp + 12
  %define .displacement ebp + 8

  %define .result ebp - 4
  push    dword 0

 .Exit:
  mov     eax, [.result]

  mov     esp, ebp
  pop     ebp
  ret 28

;
; func bool x86EncodeAddr16( Instruction instruction, uint emitFields,
;   segReg, baseReg, indexReg, int displacement )
;
x86EncodeAddr16:
  push    ebp
  mov     ebp, esp

  %define .instruction  ebp + 28
  %define .emitFields   ebp + 24
  %define .segReg       ebp + 20
  %define .baseReg      ebp + 16
  %define .indexReg     ebp + 12
  %define .displacement ebp + 8

  %define .result ebp - 4
  push    dword 0

 .Exit:
  mov     eax, [.result]

  mov     esp, ebp
  pop     ebp
  ret 24

; =====================
; Code generator tables
; =====================

segment .data use32

  cpuBits: dd 32

  x86Addr16Table:
    db 0b11_111_111 ;  0 - Invalid
    db 0b00_000_101 ;  1 - [DI]
    db 0b00_000_100 ;  2 - [SI]
    db 0b11_111_111 ;  3 - Invalid
    db 0b00_000_110 ;  4 - [BP + <DISP8 | DISP16>] if not [DISP16]
    db 0b00_000_011 ;  5 - [BP + DI]
    db 0b00_000_010 ;  6 - [BP + SI]
    db 0b11_111_111 ;  7 - Invalid
    db 0b00_000_111 ;  8 - [BX]
    db 0b00_000_001 ;  9 - [BX + DI]
    db 0b00_000_000 ; 10 - [BX + SI]
    db 0b11_111_111 ; 11 - Invalid
    db 0b11_111_111 ; 12 - Invalid
    db 0b11_111_111 ; 13 - Invalid
    db 0b11_111_111 ; 14 - Invalid
    db 0b11_111_111 ; 15 - Invalid

  keyword86Table:
    db 'call',0,0,0,0
    db 'push',0,0,0,0

  token86Table:
    dd x86Call, encode86Call
    dd x86Push, encode86Push

  encode86Table:
  encode86Call:
    dd x86Mem, 0, 0, 0
    db 0b1111_1111, 0, 0, 0b00_010_000
    dd HAS_OPCODE1 | HAS_MODRM
    db 0, 0, 0, 0
    dd 0

  encode86Push:
    dd valUint, 0, 0, 0
    db 0b0110_1000, 0, 0, 0
    dd HAS_OPCODE1
    db 0, 0, 0, 0
    dd 0
  encode86TableEnd:

  x86SegPrefixes: db 0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65, 0, 0

%endif
