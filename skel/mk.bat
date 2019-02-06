@echo off
nasm -wno-other -fobj -IC:\ORIGOW32\ %1.asm
alink -c -oPE -subsys console %1.obj
