@echo off
title Banking Management System
cd /d "%~dp0"
echo.
echo  Starting Banking Management System...
echo.
if exist banking_system.exe (
    banking_system.exe
) else (
    echo  [ERROR] banking_system.exe not found!
    echo  Please run build.bat first to compile the project.
    pause
)
