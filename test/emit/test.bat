@echo off
emit
fc /b /LB0 test.bin expected.bin
if %errorlevel% == 0 echo Test passed
if not %errorlevel% == 0 echo Test FAILED
