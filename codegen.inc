%ifndef INC_CODEGEN
%define INC_CODEGEN

  %include "debug.inc"
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

  %define .instruction ebp - (16 + INSTR_BUFFER_SIZE)
  times (INSTR_BUFFER_SIZE / 4) push dword 0

  push    ebx
  push    ecx
  push    edx
  push    esi
  push    edi

  mov     ebx, [.op]
  test    ebx, ebx
  jz      .Exit
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

  ;mov     eax, [.baseReg]
  ;and     eax, mask86Operand

  ;cmp     eax, x86Reg32
  ;jne     .NotReg32
  cmp     dword [cpuBits], 32
  jne     .NotAddr32
  lea     eax, [.instruction]
  push    eax
  lea     eax, [.emitFields]
  push    eax
  xor     eax, eax
  mov     al, [esi + ENCODE86_XFORM1]
  push    dword eax
  push    dword [.segReg]
  push    dword [.baseReg]
  push    dword [.indexReg]
  push    dword [.scale]
  push    dword [.displacement]
  call    x86EncodeAddr32
  test    eax, eax
  jz      .Exit
  jmp     .TransformInstruction
 .NotAddr32:

  ;cmp     eax, x86Reg16
  ;jne     .NotReg16
  cmp     dword [cpuBits], 16
  jne     .NotAddr16
  cmp     dword [.scale], 0
  jne     .Exit
  lea     eax, [.instruction]
  push    eax
  lea     eax, [.emitFields]
  push    eax
  xor     eax, eax
  mov     al, [esi + ENCODE86_XFORM1]
  push    dword eax
  push    dword [.segReg]
  push    dword [.baseReg]
  push    dword [.indexReg]
  push    dword [.displacement]
  call    x86EncodeAddr16
  test    eax, eax
  jz      .Exit
  jmp     .TransformInstruction
 .NotAddr16:

  jmp     .Exit

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
;   xform, segReg, baseReg, indexReg, scale, int displacement )
;
x86EncodeAddr32:
  push    ebp
  mov     ebp, esp

  %define .instruction  ebp + 36
  %define .emitFields   ebp + 32
  %define .xform        ebp + 28
  %define .segReg       ebp + 24
  %define .baseReg      ebp + 20
  %define .indexReg     ebp + 16
  %define .scale        ebp + 12
  %define .displacement ebp + 8

  %define .result ebp - 4
  push    dword 0

  mov     ecx, [.baseReg]
  mov     ebx, ecx
  mov     edx, [.indexReg]
  mov     edi, [.instruction]
  mov     esi, [.emitFields]
  test    edi, edi
  jz      .Exit
  test    esi, esi
  jz      .Exit

  ; Special case: Displacement only
  or      ecx, edx
  jnz     .NotDispOnly
  cmp     dword [.scale], 0
  ja      .Exit
  or      dword [edi + INSTR_MODRM], 0b00_000_101
  mov     eax, [.displacement]
  mov     [edi + INSTR_DISP1], eax
  or      dword [esi], \
    (HAS_DISP1 | HAS_DISP2 | HAS_DISP3 | HAS_DISP4 | HAS_MODRM)
  jmp     .EncodeSeg
 .NotDispOnly:

  ; Init ModRM from instruction buffer
  mov     al, [edi + INSTR_MODRM]
  or      al, 0b00_000_101
  ; Init SIB to no index and no base
  mov     ah, 0b00_100_101

  ; If baseReg, modify ModRM
  test    ebx, ebx
  jz      .NoBaseReg
  mov     ecx, ebx
  and     ebx, mask86SubOperand
  and     ecx, mask86Operand
  cmp     ecx, x86Reg32
  jne     .Exit
  and     al, 0b11_111_000
  or      al, bl
  mov     [edi + INSTR_MODRM], al
  or      dword [esi], HAS_MODRM
  mov     ebx, [.baseReg]
 .NoBaseReg:

; BUG: Base shows as register EAX when index register is specified - Begin
  ; If baseReg is ESP, or indexReg is specified, modify ModRM and SIB
  cmp     ebx, x86RegESP
  je      .PromoteModRM
  test    edx, edx
  jnz     .PromoteModRM
  jmp     .SkipPromoteModRM
 .PromoteModRM:
  mov     bl, al
  and     al, 0b11_111_000
  or      al, 0b00_000_100
  and     bl, 0b00_000_111
  and     ah, 0b11_111_000
  or      ah, bl
  mov     [edi + INSTR_MODRM], al
  mov     [edi + INSTR_SIB], ah
  or      dword [esi], (HAS_MODRM | HAS_SIB)
 .SkipPromoteModRM:

  ; Bug: x86Encode32 succeeds if scale is specified without an index register
  ; If indexReg, set index and scale
  test    edx, edx
  jz      .ValidateScale
  mov     ebx, edx
  and     edx, mask86SubOperand
  and     ebx, mask86Operand
  cmp     ebx, x86Reg32
  jne     .Exit
  ; ...index
  and     ah, 0b11_000_111
  shl     dl, 3
  or      ah, dl
  ; ...scale
  mov     ebx, [.scale]
  cmp     ebx, 1
  jbe     .DoneScale
  add     ah, 0b01_000_000
  cmp     ebx, 2
  je      .DoneScale
  add     ah, 0b01_000_000
  cmp     ebx, 4
  je      .DoneScale
  add     ah, 0b01_000_000
  cmp     ebx, 8
  je      .DoneScale
  jmp     .Exit
 .ValidateScale:
  cmp     dword [.scale], 0
  ja      .Exit
  jmp     .NoIndexReg
 .DoneScale:
  mov     [edi + INSTR_MODRM], al
  mov     [edi + INSTR_SIB], ah
  or      dword [esi], (HAS_MODRM | HAS_SIB)
 .NoIndexReg:
; BUG: Base shows as register EAX when index register is specified - End

  ; If displacement, or baseReg is EBP, modify ModRM
  mov     ebx, [.displacement]
  cmp     dword [.baseReg], x86RegEBP
  je      .EncodeDisp
  test    ebx, ebx
  jz      .DoneDisp
 .EncodeDisp:
  mov     [edi + INSTR_DISP1], ebx
  add     al, 0b01_000_000
  cmp     ebx, -128
  jl      .NotDisp8
  cmp     ebx, 127
  jg      .NotDisp8
  or      [edi + INSTR_MODRM], al
  or      dword [esi], HAS_DISP1
  jmp     .DoneDisp
 .NotDisp8:
  add     al, 0b01_000_000
  or      [edi + INSTR_MODRM], al
  or       dword [esi], (HAS_DISP1 | HAS_DISP2 | HAS_DISP3 | HAS_DISP4)
 .DoneDisp:

 ; Transform operand
 .TransformOperand:

 ; Encode segReg, and skip assumed segment registers
 .EncodeSeg:
  mov     ecx, [.segReg]
  mov     ebx, ecx
  and     ecx, mask86Operand
  and     ebx, mask86SubOperand
  cmp     ecx, x86SReg
  jne     .SkipSeg
  mov     al, [ebx + x86SegPrefixes]
  mov     [edi + INSTR_GROUP2_PREFIX], al
  or      dword [esi], HAS_GROUP2_PREFIX
 .SkipSeg:

  ; Encode address size prefix
  mov     al, [addr32Prefix]
  test    al, al
  jz      .SkipPrefix
  mov     [edi + INSTR_GROUP3_PREFIX], al
  mov     eax, [addr32Field]
  or      [esi], eax
 .SkipPrefix:

  mov     dword [.result], 1

 .Exit:
  mov     eax, [.result]

  mov     esp, ebp
  pop     ebp
  ret 32

;
; func bool x86EncodeAddr16( Instruction instruction, uint emitFields,
;   xform, segReg, baseReg, indexReg, int displacement )
;
x86EncodeAddr16:
  push    ebp
  mov     ebp, esp

  %define .instruction  ebp + 32
  %define .emitFields   ebp + 28
  %define .xform        ebp + 24
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
  ret 28

; =====================
; Code generator tables
; =====================

segment .data use32

  cpuBits: dd 32

  addr16Prefix: db 0x67
  addr16Field:  dd HAS_GROUP4_PREFIX

  addr32Prefix: db 0
  addr32Field:  dd 0

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
