@echo off
nasm -fobj %1.asm
alink -c -oPE -subsys console %1.obj