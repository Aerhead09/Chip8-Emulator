@echo off
setlocal

set SRC_DIR=src
set BIN_DIR=bin
set SDL_DIR=libs\SDL3
set TARGET=%BIN_DIR%\chip8.exe

where g++ >nul 2>nul
if errorlevel 1 (
    echo [ERROR] g++ tidak ditemukan di PATH.
    exit /b 1
)

if not exist %BIN_DIR% mkdir %BIN_DIR%

echo Compiling...

g++ -std=c++17 -I %SRC_DIR% -I %SDL_DIR%\include ^
    %SRC_DIR%\main.cpp %SRC_DIR%\chip8.cpp ^
    -L %SDL_DIR%\lib ^
    -lmingw32 -lSDL3 ^
    -o %TARGET%

if errorlevel 1 (
    echo [GAGAL] Build error.
    exit /b 1
)

echo [SUKSES] %TARGET%
endlocal