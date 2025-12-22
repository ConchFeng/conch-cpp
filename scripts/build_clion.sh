#!/bin/bash
# Build script for CLion on Linux

set -e

echo "========================================"
echo "Conch Cross-Platform - CLion Build"
echo "========================================"

# Create build directory for CLion
mkdir -p build_clion
cd build_clion

echo ""
echo "[1/4] Installing Conan dependencies..."
conan install .. --build=missing

echo ""
echo "[2/4] Configuring CMake for CLion..."
cmake .. -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo ""
echo "[3/4] Building project (Debug)..."
cmake --build . -- -j$(nproc)

echo ""
echo "[4/4] Build complete!"
echo ""
echo "Build directory: build_clion/"
echo "Binaries: build_clion/bin/"
echo "Compile commands: build_clion/compile_commands.json"
echo ""
echo "To open in CLion:"
echo "  1. Open CLion"
echo "  2. File -> Open -> Select project root"
echo "  3. CLion will detect CMakeLists.txt automatically"
echo ""

cd ..
