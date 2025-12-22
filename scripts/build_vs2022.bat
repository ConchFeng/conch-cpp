@echo off
REM Build script for Visual Studio 2022 on Windows

echo ========================================
echo Conch Cross-Platform - VS2022 Build
echo ========================================

REM Create build directory for VS2022
if not exist "build_vs2022" mkdir build_vs2022
cd build_vs2022

echo.
echo [1/4] Installing Conan dependencies...
conan install .. --build=missing -s compiler="Visual Studio" -s compiler.version=17

echo.
echo [2/4] Generating Visual Studio 2022 solution...
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake ^
    -DCMAKE_BUILD_TYPE=Release

echo.
echo [3/4] Building project (Release)...
cmake --build . --config Release -j

echo.
echo [4/4] Build complete!
echo.
echo Solution file: build_vs2022\ConchCrossPlatform.sln
echo Binaries: build_vs2022\bin\Release\
echo.
echo To open in Visual Studio 2022:
echo   start ConchCrossPlatform.sln
echo.

cd ..
