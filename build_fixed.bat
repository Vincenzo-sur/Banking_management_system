@echo off
title Building Banking Management System
cd /d "%~dp0"

echo.
echo  ================================================
echo    Banking Management System - Build Script
echo  ================================================
echo.

REM ── Try g++ from PATH first ─────────────────────────────────
where g++ >nul 2>&1
if %errorlevel%==0 (
    echo  [INFO] Found g++ in PATH. Compiling...
    goto :compile_gpp
)

REM ── Try WinLibs location (installed via winget) ────────────
set WINLIBS=C:\Users\%USERNAME%\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\g++.exe
if exist "%WINLIBS%" (
    echo  [INFO] Found WinLibs g++. Compiling...
    "%WINLIBS%" -std=c++17 -Wall -O2 main.cpp account.cpp bank.cpp admin.cpp filemanager.cpp -o banking_system.exe
    goto :check_result
)

echo  [ERROR] No C++ compiler found. Please install g++ first.
pause
goto :eof

:compile_gpp
g++ -std=c++17 -Wall -O2 main.cpp account.cpp bank.cpp admin.cpp filemanager.cpp -o banking_system.exe

:check_result
if %errorlevel%==0 (
    echo.
    echo  [SUCCESS] Build complete!
    echo  Run:  banking_system.exe   OR   double-click run.bat
    echo.
) else (
    echo  [ERROR] Compilation failed. Check errors above.
)
pause
