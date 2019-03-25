%ifndef INC_WINPE
%define INC_WINPE

segment .text use32

;
; Windows Portable Executable
;

;
; func @peFile CreatePE( string peName )
;
CreatePE:
  push    ebp
  mov     ebp, esp

  %define .peName ebp + 8

 .Exit:
  mov     esp, ebp
  pop     ebp
  ret 4

;
; func ClosePE( @peFile )
;
ClosePE:
  push    ebp
  mov     ebp, esp

  %define .peFile ebp + 8

 .Exit:
  mov     esp, ebp
  pop     ebp
  ret 4

%endif

segment .data use32

  dosStub:
    incbin "dosstub.exe"
  dosStubEnd:

  %define SIZEOF_DOSSTUB ((dosStubEnd - dosStub) + 1)
