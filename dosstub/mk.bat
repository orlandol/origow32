@echo off
nasm -wno-other -fobj %1.asm
alink -c -oEXE %1.obj
