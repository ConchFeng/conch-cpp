#!/bin/bash
# Build script for Xcode on macOS

set -e

echo "========================================"
echo "Conch Cross-Platform - Xcode Build"
echo "========================================"

# Create build directory for Xcode
mkdir -p build_xcode
cd build_xcode

echo ""
echo "[1/4] Installing Conan dependencies..."
conan install .. --build=missing

echo ""
echo "[2/4] Generating Xcode project..."
cmake .. -G "Xcode" \
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release

echo ""
echo "[3/4] Building project (Release)..."
cmake --build . --config Release -- -jobs $(sysctl -n hw.ncpu)

echo ""
echo "[4/4] Build complete!"
echo ""
echo "Xcode project: build_xcode/ConchCrossPlatform.xcodeproj"
echo "Binaries: build_xcode/bin/Release/"
echo ""
echo "To open in Xcode:"
echo "  open ConchCrossPlatform.xcodeproj"
echo ""

cd ..
