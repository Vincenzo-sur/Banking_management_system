@echo off
REM ============================================================
REM  build.bat  –  Compile the Banking Management System
REM  Supports:  MinGW (g++),  MSVC (cl),  or manual guidance
REM ============================================================

echo.
echo  ================================================
echo    Banking Management System - Build Script
echo  ================================================
echo.

REM ── Try MinGW g++ ────────────────────────────────────────────
where g++ >nul 2>&1
if %errorlevel%==0 (
    echo  [INFO] Found g++ compiler. Compiling...
    g++ -std=c++17 -Wall -Wextra -O2 ^
        main.cpp account.cpp bank.cpp admin.cpp filemanager.cpp ^
        -o banking_system.exe
    if %errorlevel%==0 (
        echo.
        echo  [SUCCESS] Build complete! Run: banking_system.exe
    ) else (
        echo  [ERROR] Compilation failed. Check errors above.
    )
    goto :eof
)

REM ── Try MSVC cl ──────────────────────────────────────────────
where cl >nul 2>&1
if %errorlevel%==0 (
    echo  [INFO] Found MSVC compiler. Compiling...
    cl /EHsc /std:c++17 /W4 /Fe:banking_system.exe ^
        main.cpp account.cpp bank.cpp admin.cpp filemanager.cpp
    if %errorlevel%==0 (
        echo.
        echo  [SUCCESS] Build complete! Run: banking_system.exe
    ) else (
        echo  [ERROR] Compilation failed. Check errors above.
    )
    goto :eof
)

REM ── No compiler found ─────────────────────────────────────────
echo  [ERROR] No C++ compiler found.
echo.
echo  To install a compiler:
echo    Option A (MinGW): https://winlibs.com/
echo      - Download winlibs-x86_64 ZIP, extract to C:\mingw64
echo      - Add C:\mingw64\bin to your PATH environment variable
echo      - Then re-run this script
echo.
echo    Option B (MSVC): Install Visual Studio Build Tools
echo      - https://visualstudio.microsoft.com/downloads/
echo      - Select "C++ build tools" workload
echo.
echo    Option C (Online): Use https://replit.com or Godbolt
echo      - Paste all source files and compile online

pause
