@echo off
nasm -fobj -IC:\Origow32\ %1.asm
alink -c -oPE -subsys console %1.obj
