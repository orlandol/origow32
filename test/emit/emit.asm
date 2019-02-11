
  CPU 386

  BITS 32

segment .text use32

  %include "rtl.inc"
  %include "codegen.inc"

run:
..start:
  push    ebp
  mov     ebp, esp

  call    InitRTL

  push    binName
  call    fcreate
  mov     [binFile], eax

  test    eax, eax
  jnz     .FileCreated
  push    fcreateFailed
  call    echostring
  push    dword 1
  call    [ExitProcess]
 .FileCreated:

  mov     byte [testInst + INSTR_GROUP1_PREFIX], INSTR_GROUP1_PREFIX
  mov     byte [testInst + INSTR_GROUP2_PREFIX], INSTR_GROUP2_PREFIX
  mov     byte [testInst + INSTR_GROUP3_PREFIX], INSTR_GROUP3_PREFIX
  mov     byte [testInst + INSTR_GROUP4_PREFIX], INSTR_GROUP4_PREFIX
  mov     byte [testInst + INSTR_OPCODE1], INSTR_OPCODE1
  mov     byte [testInst + INSTR_OPCODE2], INSTR_OPCODE2
  mov     byte [testInst + INSTR_OPCODE3], INSTR_OPCODE3
  mov     byte [testInst + INSTR_MODRM], INSTR_MODRM
  mov     byte [testInst + INSTR_SIB], INSTR_SIB
  mov     byte [testInst + INSTR_DISP1], INSTR_DISP1
  mov     byte [testInst + INSTR_DISP2], INSTR_DISP2
  mov     byte [testInst + INSTR_DISP3], INSTR_DISP3
  mov     byte [testInst + INSTR_DISP4], INSTR_DISP4
  mov     byte [testInst + INSTR_IMM1], INSTR_IMM1
  mov     byte [testInst + INSTR_IMM2], INSTR_IMM2
  mov     byte [testInst + INSTR_IMM3], INSTR_IMM3
  mov     byte [testInst + INSTR_IMM4], INSTR_IMM4

  push    dword [binFile]
  push    testInst
  push    dword [testFlags]
  call    emit

  test    eax, eax
  jnz     .emitSucceeded
  push    emitFailed
  call    echostring
  push    dword 2
  call    [ExitProcess]
 .emitSucceeded:

 .Exit:
  push    binFile
  call    fclose

  mov     esp, ebp
  pop     ebp

  push    dword 0
  call    [ExitProcess]

segment .data use32

  declstring fcreateFailed, 13,10,'fcreate FAILED',13,10
  declstring emitFailed, 13,10,'emit FAILED',13,10
  declstring binName, 'test.bin'

  binFile: dd 0

  testFlags: dd HAS_GROUP1_PREFIX |\
                HAS_GROUP2_PREFIX |\
                HAS_GROUP3_PREFIX |\
                HAS_GROUP4_PREFIX |\
                HAS_OPCODE1       |\
                HAS_OPCODE2       |\
                HAS_OPCODE3       |\
                HAS_MODRM         |\
                HAS_SIB           |\
                HAS_DISP1         |\
                HAS_DISP2         |\
                HAS_DISP3         |\
                HAS_DISP4         |\
                HAS_IMM1          |\
                HAS_IMM2          |\
                HAS_IMM3          |\
                HAS_IMM4

  testInst: times INSTR_BUFFER_SIZE db 0

section .bss use32
