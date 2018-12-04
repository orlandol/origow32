@echo.
@echo Running sysecho...
sysecho > test.txt

@echo.
@echo Comparing test output to expected output...
fc /b test.txt expected.txt

@echo.
@if errorlevel == 0 echo Test passed.
@if not errorlevel == 0 echo TEST FAILED.
