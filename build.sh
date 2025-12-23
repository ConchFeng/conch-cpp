#!/bin/bash
# Build script for Conch Cross-Platform Project

set -e

echo "🚀 Starting Conch Cross-Platform Build..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir -p build
fi

cd build

# Auto-detect system settings (OS, compiler) and generate a default profile.
# The --force flag ensures it overwrites any existing profile to avoid errors.
conan profile detect --force

# Install dependencies with Conan
echo "📦 Installing dependencies with Conan..."
conan install .. --build=missing

# Configure CMake with Conan toolchain
echo "🔧 Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake

# Build
echo "🔨 Building project..."
cmake --build . --config Release -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "✅ Build complete! Binaries are in build/bin/"
