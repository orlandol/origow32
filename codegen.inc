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
  mov     esi, [ebx * 8 + encode86Table + TOKEN86_ENCODE]

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

  mov     ecx, [.baseReg]
  mov     edx, [.indexReg]
  xor     ebx, ebx
  mov     eax, ecx
  and     ecx, mask86Operand

  ; UNTESTED: Process a 16-bit address
  cmp     ecx, x86Reg16
  jne     .NotAddr16

  ; Scale not allowed for 16-bit addresses
  cmp     dword [.scale], 0
  jne     .Exit

  ; Initiaqlize 16-bit prefix
  mov     cl, [addr16Prefix]
  mov     [.addrPrefix], cl
  mov     ecx, [addr16Field]
  mov     [.addrField], ecx

  ; Validate and initialize 16-bit base register
  cmp     eax, x86RegBX
  jne     .Addr16NotBX
  or      ebx, mrX86RegBX
  jmp     .Addr16DoneBase
 .Addr16NotBX:
  cmp     eax, x86RegBP
  jne     .Addr16NotBP
  or      ebx, mrX86RegBP
  jmp     .Addr16DoneBase
 .Addr16NotBP:
  test    eax, eax
  jz      .Addr16DoneBase
  jmp     .Exit
 .Addr16DoneBase:

  ; Validate and initialize 16-bit index register
  cmp     eax, x86RegSI
  jne     .Addr16NotSI
  or      ebx, mrX86RegSI
  jmp     .Addr16DoneIndex
 .Addr16NotSI:
  cmp     eax, x86RegDI
  jne     .Addr16NotDI
  or      ebx, mrX86RegDI
  jmp     .Addr16DoneIndex
 .Addr16NotDI:
  test    eax, eax
  jz      .Addr16DoneIndex
  jmp     .Exit
 .Addr16DoneIndex:

  cmp     ebx, (mrX86RegBX | mrX86RegBP | mrX86RegSI | mrX86RegDI)
  ja      .Exit
  mov     al, [ebx + x86Addr16Table]
  cmp     al, 0b11_111_111
  je      .Exit

  mov     ecx, [.displacement]

  ; Special case: No base register, and no index register
  test    ebx, ebx
  jnz     .Addr16NotDispOnly
  mov     edx, valInt16
  mov     al, 0b00_000_110
  jmp     .EncodeInstruction
 .Addr16NotDispOnly:

  ; Special case: Except BP as base register, ignore displacement 0
  cmp     ebx, mrX86RegBP
  je      .Addr16SkipDisp0
  test    ecx, ecx
  jnz     .Addr16SkipDisp0
  xor     edx, edx
  jmp     .EncodeInstruction
 .Addr16SkipDisp0:

  mov     edx, valInt8
  add     al, 0b01_000_000
  cmp     ecx, -128
  jle     .Addr16NotDisp8
  cmp     ecx, 127
  jge     .Addr16NotDisp8
  add     al, 0b01_000_000
  jmp     .EncodeInstruction
 .Addr16NotDisp8:
  mov     edx, valInt16
  jmp     .EncodeInstruction
 .NotAddr16:

  ;;;
  ; UNTESTED: Process a 32-bit address
  cmp     ecx, x86Reg32
  jne     .NotAddr32

  ; Initialize 32-bit prefix
  mov     bl, [addr32Prefix]
  mov     [.addrPrefix], bl
  mov     ebx, [addr32Field]
  mov     [.addrField], ebx

  mov     ebx, edx
  and     ebx, mask86Operand

  mov     edi, [.scale]

  and     eax, mask86SubOperand
  mov     ah, 0b00_100_000

  ; Process index and scale
  cmp     dword [.baseReg], x86RegESP
  je      .Addr32ESPBase
  test    edx, edx
  jz      .Addr32SkipIndex
 .Addr32ESPBase:

  ; Index can't be register esp
  cmp     edx, x86RegESP
  je      .Exit
  ; Index must be a 32-bit register
  cmp     ebx, x86Reg32
  jne     .Exit

  ; Initialize the SIB and ModRM bytes
  and     edx, mask86SubOperand
  and     ah, 0b11_000_111
  shl     dl, 3
  or      dl, al
  mov     ah, dl
  mov     al, 0b00_000_100
 .Addr32SkipIndex:

  ; Set Scale bytes in the SIB
  cmp     edi, 1
  je      .Addr32EncodeSIB
  add     ah, 0b01_000_000
  cmp     edi, 2
  je      .Addr32EncodeSIB
  add     ah, 0b01_000_000
  cmp     edi, 4
  je      .Addr32EncodeSIB
  add     ah, 0b01_000_000
  cmp     edi, 8
  je      .Addr32EncodeSIB
  jmp     .Exit

 .Addr32EncodeSIB:
  mov     [.instruction + INSTR_SIB], ah
  or      dword [.emitFields], HAS_SIB
  jmp     .Addr32DoneScale

 .Addr32DoneScale:

  mov     ecx, [.displacement]
  xor     edx, edx

  ; TODO: Special case ebp

  cmp     dword [.baseReg], x86RegEBP
  je      .Addr32CheckDisp
  test    ecx, ecx
  jnz     .Addr32SkipDisp0
  jmp     .EncodeInstruction
 .Addr32CheckDisp:
  test    ecx, ecx
  jnz     .Addr32SkipDisp0
  mov     edx, valInt8
 .Addr32SkipDisp0:

  mov     edx, valInt8
  add     al, 0b01_000_000
  cmp     ecx, -128
  jle     .Addr32NotDisp8
  cmp     ecx, 127
  jge     .Addr32NotDisp8
  add     al, 0b01_000_000
  jmp     .EncodeInstruction
 .Addr32NotDisp8:
  mov     edx, valInt32
  jmp     .EncodeInstruction
 .NotAddr32:

  ; addr16Field != HAS_GROUP4_PREFIX when the target CPU mode is 16-bits
  cmp     dword [addr16Field], HAS_GROUP4_PREFIX
  jne     .Not16BitDisp
  mov     ecx, [.displacement]
  mov     edx, valInt16
  jmp     .EncodeInstruction
 .Not16BitDisp:

  ; addr32ield != HAS_GROUP4_PREFIX when the target CPU mode is 32-bits
  cmp     dword [addr32Field], HAS_GROUP4_PREFIX
  jne     .Not32BitDisp
  mov     ecx, [.displacement]
  mov     edx, valInt32
  jmp     .EncodeInstruction
 .Not32BitDisp:

  ; Prepare instruction buffer for emit
 .EncodeInstruction:
  or      [.instruction + INSTR_MODRM], al

  mov     al, [.addrPrefix]
  mov     [.instruction + INSTR_GROUP4_PREFIX], al
  mov     eax, [.addrField]
  or      [.emitFields], eax

  cmp     edx, valInt8
  jne     .NotEncodeDisp8
  mov     [.instruction + INSTR_DISP1], ecx
  or      dword [.emitFields], HAS_DISP1
  jmp     .XformInstruction
 .NotEncodeDisp8:

  cmp     edx, valInt16
  jne     .NotEncodeDisp16
  mov     [.instruction + INSTR_DISP1], ecx
  or      dword [.emitFields], (HAS_DISP1 | HAS_DISP2)
  jmp     .XformInstruction
 .NotEncodeDisp16:

  cmp     edx, valInt32
  jne     .NotEncodeDisp32
  mov     [.instruction + INSTR_DISP1], ecx
  or      dword [.emitFields], (HAS_DISP1 | HAS_DISP2 | HAS_DISP3 | HAS_DISP4)
  jmp     .XformInstruction
 .NotEncodeDisp32:

 .XformInstruction:
  ; TODO: Apply operand/operator transformations

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

; =====================
; Code generator tables
; =====================

segment .data use32

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

%endif
