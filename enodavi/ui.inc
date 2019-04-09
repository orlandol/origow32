%ifndef INC_UI
%define INC_UI

  %define VIEW_X      0
  %define VIEW_Y      4
  %define VIEW_WIDTH  8
  %define VIEW_HEIGHT 12
  %define VIEW_DRAW   16

segment .text use32

;
; func InitUI()
;
InitUI:
  push    ebp
  mov     ebp, esp



 .Exit:
  mov     esp, ebp
  pop     ebp
  ret

%endif
