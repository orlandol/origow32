@echo off
rtlecho > test.txt

fc /LB0 /B test.txt expected.txt

if errorlevel == 0 echo TEST PASSED
if not errorlevel == 0 echo TEST FAILED
