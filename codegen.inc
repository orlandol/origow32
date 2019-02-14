%ifndef INC_CODEGEN
%define INC_CODEGEN

  %include "tokens.inc"

; =================
; Emit declarations
; =================

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

  %define param86RegW 1
  %define param86ImmW 2

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


; ===========================
; Code generator declarations
; ===========================

;
; func bool x86GenMemParam( x86Instruction instruction,
;   uint baseReg, indexReg, scale, dispType, int displacement )
;
x86GenMemParam:
  push    ebp
  mov     ebp, esp

  %define .instruction  ebp + 28
  %define .baseReg      ebp + 24
  %define .indexReg     ebp + 20
  %define .scale        ebp + 16
  %define .dispType     ebp + 12
  %define .displacement ebp + 8

  %define .result ebp - 4
  push    dword 0

  %define .memRegs ebp - 8
  push    dword 0

  %define .modRM ebp - 12
  push    dword 0b00_000_000

  push    ebx
  push    ecx
  push    edx
  push    esi
  push    edi

  ; Validate parameters
  mov     eax, [.baseReg]
  mov     ecx, [.indexReg]
  mov     edx, [.dispType]
  mov     ebx, eax
  mov     esi, eax
  and     eax, maskGroup

  or      esi, ecx
  or      esi, edx
  jz      .Exit

  mov     edi, [.instruction]
  test    edi, edi
  jz      .Exit

  ; TODO - Default standalone displacement size depends on addressMode

  ; UNTESTED - Process 16-bit memory address
  cmp     eax, x86Reg16
  jne     .NotReg16

  cmp     dword [.scale], 0
  jne     .Exit

  xor     esi, esi

  cmp     ebx, x86RegBX
  jne     .NotBX
  or      esi, mrX86RegBX
  jmp     .CheckIndex
 .NotBX:
  cmp     ebx, x86RegBP
  jne     .NotBP
  or      esi, mrX86RegBP
  jmp     .CheckIndex
 .NotBP:
  jmp     .Exit

 .CheckIndex:
  cmp     ecx, x86RegSI
  jne     .NotSI
  or      esi, mrX86RegSI
  jmp     .EncodeRegs
 .NotSI:
  cmp     ecx, x86RegDI
  jne     .NotDI
  or      esi, mrX86RegDI
  jmp     .EncodeRegs
 .NotDI:
  jmp     .Exit

 .EncodeRegs:
  xor     eax, eax
  cmp     esi, (mrX86RegBX | mrX86RegSI)
  je      .CheckDisp16
  inc     eax
  cmp     esi, (mrX86RegBX | mrX86RegDI)
  je      .CheckDisp16
  inc     eax
  cmp     esi, (mrX86RegBP | mrX86RegSI)
  je      .CheckDisp16
  inc     eax
  cmp     esi, (mrX86RegBP | mrX86RegDI)
  je      .CheckDisp16
  inc     eax
  cmp     esi, mrX86RegSI
  je      .CheckDisp16
  inc     eax
  cmp     esi, mrX86RegDI
  je      .CheckDisp16
  inc     eax
  cmp     esi, mrX86RegBP
  je      .CheckDisp16
  test    esi, esi
  jz      .CheckDisp16
  inc     eax
  cmp     esi, mrX86RegBX
  je      .CheckDisp16
  jmp     .Exit

 .CheckDisp8:
  test    edx, edx
  jnz     .HasDisp
  mov     edx, valInt
  mov     dword [.displacement], 0

 .CheckDisp16:
  mov     esi, [.displacement]
  test    edx, edx
  jz      .EncodeMem16
  add     eax, 0b01_000_000
  cmp     esi, -128
  jl      .NotDisp8
  cmp     esi, 127
  jg      .NotDisp8
  jmp     .EncodeMem16
 .NotDisp8:
  add     eax, 0b01_000_000

 .EncodeMem16:
  
  jmp     .Exit
 .NotReg16:

  ; TODO - Process 32-bit memory address

 .Exit:
  mov     eax, [.result]

  pop     edi
  pop     esi
  pop     edx
  pop     ecx
  pop     ebx

  mov     esp, ebp
  pop     ebp
  ret 24

;
; func bool x86GenOpRegReg( uint opToken, destReg, srcReg )
;
x86GenOpRegReg:
  push    ebp
  mov     ebp, esp

  %define .opToken ebp + 16
  %define .destReg ebp + 12
  %define .srcReg ebp + 8

 .Exit:
  mov     esp, ebp
  pop     ebp
  ret 12

%endif
