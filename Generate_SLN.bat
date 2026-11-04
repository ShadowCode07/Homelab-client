@echo off


REM
set CMAKE_CMD=cmake -G "Visual Studio 18 2026" -A x64 -B ./build .


REM 
echo %CMAKE_CMD%
%CMAKE_CMD%

pause